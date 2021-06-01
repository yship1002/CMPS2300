#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
void testfork6(){
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
void testfork5(){
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
void testfork4(){
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
void testfork3(){
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
void testfork2(char* a){
    FILE* file=fopen(a,"r+");
    if (file==NULL){printf("Error: Can't open the file!");}
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is child process running!\n");
        char temp[100];
        fgets(temp,100,file);
        printf("Printed by Child: %s\n",temp);
    }else{
        printf("This is the parent process running!\n");
        char temp[100];
        fgets(temp,100,file);
        printf("Printed by Parent: %s\n",temp);
    }
    printf("This process is done!\n");
    fclose(file);
}
void testfork1(){
    int a=100;
    int rc=fork();
    if (rc<0){
        printf("Fork failed!\n");
        exit(1);
    }else if(rc==0){
        printf("This is the a value in child process: %d\n",a);
        printf("I change a value to 90!\n");
        a=90;
    }else{
        a=110;
        printf("This is the parent process!\n");
        printf("Current a value: %d\n",a);
    }
    printf("Finally a value is: %d\n",a);
}
int main(int agrc, char* argv[]){
    printf("This is the start of the program!\n");
    testfork1();
    testfork2(argv[1]);
    testfork3();
    testfork4();
    testfork5();
    testfork6();
    printf("This is the end of the program!\n");
    return 0;
}
