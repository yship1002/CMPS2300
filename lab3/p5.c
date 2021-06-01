#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
//I think it is useful when you have multiple process running and you want to specify which process you want to wait for
void testfork(){
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is child plsrocess running!\n");
    }else{
        printf("This is the parent process running!\n");
        pid_t rc_wait=waitpid(rc,   NULL,0);
        printf("I am the parent of %d\n",rc_wait);
    }
    printf("This process is done!\n");
}
int main(int agrc, char* argv[]){
    testfork();
    return 0;
}
