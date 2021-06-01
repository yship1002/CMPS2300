#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//No only one of the process can access file descriptor
//If they both write to file the first one will write to the file but the second one will write nothing
void testfork(char* a){
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
int main(int agrc, char* argv[]){
    testfork(argv[1]);
    return 0;
}

