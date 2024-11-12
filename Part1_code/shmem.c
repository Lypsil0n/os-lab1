#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

const char *semName1 = "my_sema1";
const char *semName2 = "my_sema2";

int main(int argc, char **argv)
{
<<<<<<< HEAD
	sem_t *sem_id1 = sem_open(semName1, O_CREAT, O_RDWR, 1);
	sem_t *sem_id2 = sem_open(semName2, O_CREAT, O_RDWR, 0);
=======
	sem_t *sem_sender = sem_open(sender_name, O_CREAT, O_RDWR, 1);
	sem_t *sem_reciever = sem_open(reciever_name, O_CREAT, O_RDWR, 0);
>>>>>>> a50eea4842b5164eac50c4343cc25b8c39e3a4a0
	int status;
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
			 /* busy wait until the buffer is empty */
			for(int i = 0; i < 100; i++){
				sem_wait(sem_id1);
				delay = 100000 + rand() % 500000;
				usleep(delay);
				var1++;
				printf("Sending %d\n", var1); fflush(stdout);
				shmp->buffer[shmp->write_index] = var1;
				shmp->write_index = (shmp->write_index + 1) % 10;
				sem_post(sem_id2);
			}
		}
		sem_close(sem_id1);
		sem_close(sem_id2);
		wait(status);
		sem_unlink(semName1);
		sem_unlink(semName2);
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */ /* busy wait until there is something */
			for(int i = 0; i < 100; i++){
				sem_wait(sem_id2);
				delay = 20000 + rand() % 2000000;
				usleep(delay);
				var2 = shmp->buffer[shmp->read_index];
				shmp->read_index = (shmp->read_index + 1) % 10;
				printf("Received %d\n", var2); fflush(stdout);
				sem_post(sem_id1);
			}
		}
		sem_close(sem_id1);
		sem_close(sem_id2);
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}