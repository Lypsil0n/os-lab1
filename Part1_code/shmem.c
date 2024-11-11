#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

int main(int argc, char **argv)
{
	struct shm_struct {
		int buffer[10];
		int write_index;
		int read_index;
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1;
	struct shmid_ds *shm_buf;
	srand(time(NULL));
	int delay;

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	shmp->write_index = 0;
	shmp->read_index = 0;
	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
			/* write to shmem */
			while ((shmp->write_index + 1) % 10 == shmp->read_index); /* busy wait until the buffer is empty */
			for(int i = 0; i < 10; i++){
				delay = 100000 + rand() % 500000;
				usleep(delay);
				var1++;
				printf("Sending %d\n", var1); fflush(stdout);
				shmp->buffer[shmp->write_index] = var1;
				shmp->write_index = (shmp->write_index + 1) % 10;
			}
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */
			while ((shmp->read_index + 1) % 10 == shmp->write_index); /* busy wait until there is something */
			for(int i = 0; i < 10; i++){
				delay = 20000 + rand() % 2000000;
				usleep(delay);
				var2 = shmp->buffer[shmp->read_index];
				shmp->read_index = (shmp->read_index + 1) % 10;
				printf("Received %d\n", var2); fflush(stdout);
			}
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}
