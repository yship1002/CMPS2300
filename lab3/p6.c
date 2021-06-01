#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
//printf() function in child process will do nothing after closing the file
void testfork(){
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is child process running!\n");
        close(STDOUT_FILENO);
        printf("I am just here to test stdout!\n");
    }else{
        printf("This is the parent process running!\n");
    }
    printf("This process is done!\n");
}
int main(int agrc, char* argv[]){
    testfork();
    return 0;
}
