#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
//If you use wait() in parent process, it return the process ID of the child process
//If you use wait() in child process, it will return -1
void testfork(){
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is child process running!\n");
        int rc_wait_child=wait(NULL);
        printf("This is what is returned from wait in child process: %d\n",rc_wait_child);
    }else{
        printf("This is the parent process running!\n");
        //int rc_wait=wait(NULL);
        //printf("I am the parent of %d\n",rc_wait);
    }
    printf("This process is done!\n");
}
int main(int agrc, char* argv[]){
    testfork();
    return 0;
}

