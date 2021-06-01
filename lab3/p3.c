#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
//I think the reason is because there are many ways to run a program You can have arguments, a file as an input to a program so the family of exec() function can accmodate that
void testfork(){
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is child process running!\n");
        char *temp[100];
        temp[0]=strdup("/bin/ls");
        execl(temp[0],NULL);
        printf("%s\n",temp[0]);
    }else{
        printf("This is the parent process running!\n");
    }
    printf("This process is done!\n");
}
int main(int agrc, char* argv[]){
    testfork();
    return 0;
}


