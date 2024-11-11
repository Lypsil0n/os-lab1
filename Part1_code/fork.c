#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t pid;
    unsigned i;
    unsigned niterations = 100;
    pid = fork();
    
    if (pid == 0) 
    {
        for (i = 0; i < niterations; ++i)
            printf("A = %d, ", i);
            printf("In child => Own pid : %d\n", getpid());
            printf("In child => Parent's pid : %d\n", pid);
     } 
     else 
     {
        pid_t pid2 = fork();
        if (pid2 == 0) 
        {
            for (i = 0; i < niterations; ++i)
                printf("B = %d, ", i);
                printf("In child => Own pid : %d\n", getpid());
                printf("In child => Parent's pid : %d\n", pid);
        }
        else 
        {
            for (i = 0; i < niterations; ++i)
                printf("C = %d, ", i);
                printf("In child => Own pid : %d\n", getpid());
                printf("In child => Parent's pid : %d\n", pid);
            printf("Child 1: %d\n", pid);
            printf("Child 2: %d\n", pid2);
        }
    } 
}
