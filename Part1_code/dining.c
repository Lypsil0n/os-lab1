#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t locks[5] = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, 
                              PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, 
                              PTHREAD_MUTEX_INITIALIZER };


struct professorArgs {
    char* name;
    unsigned int id;
};

void* professor(void* arg){
    struct professorArgs* args = (struct professorArgs*)arg;
    char* name = args->name;
    unsigned int childID = args->id;
    int time;
    for(;;){
        printf("%s: thinking -> trying to pick up left chopstick\n", name);
        time = rand() % (5 - 1 + 1) + 1;
        sleep(time);
        printf("%s: trying to pick up left chopstick\n", name);
        pthread_mutex_lock(&locks[(childID - 1) % 5]);
        time = rand() % (8 - 2 + 1) + 2;
        sleep(time);
        printf("%s: got left chopstick -> trying to pick up right chopstick\n", name);
        if(pthread_mutex_trylock(&locks[(childID + 1) % 5]) != 0){
            printf("%s: right chopstick not available\n", name);
            pthread_mutex_unlock(&locks[(childID - 1) % 5]); 
            continue;
        };
        printf("%s: got both chopstick -> eating\n", name);
        time = rand() % (10 - 5 + 1) + 5;
        sleep(time);
        printf("%s: eating -> putting both chopsticks down\n", name);
        pthread_mutex_unlock(&locks[(childID + 1) % 5]);   
        pthread_mutex_unlock(&locks[(childID - 1) % 5]); 
    }
}

int main(int argc, char** argv){
    pthread_t threads[5];
    char* names[5] = {"Tanenbaum", "Bos", "Lamport", "Stallings", "Silberschatz"};
    struct professorArgs* args;
    args = malloc(5 * sizeof(struct professorArgs));
    for (unsigned int i = 0; i < 5; i++){
        args[i].name = names[i];
        args[i].id = i;
        pthread_create(&threads[i], NULL, professor, (void*)&args[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
}