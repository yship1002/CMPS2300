#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//Although variable has been changed to 100, in child processes variable stays 100 so even though parent process change it to 110 it doesn't affect child process
//Food for thought:
//After putting all function in one file and call them in turn the output is messed up and no particular order can be found
void testfork(){
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
int main(){
    testfork();
    return 0;
}
