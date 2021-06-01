#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool inarray(char a, char c[],int sizec){
    for (int i=0;i<sizec;i++){
        if(a==c[i]){
            return 1;
        }
    }
    return 0;
}

int main(){
    
    bool a=1;
    char hex[20];
    char validchar[]={'A','B','C','D','E','F','1','2','3','4','5','6','7','8','9','0'};
    while (a){
        printf("Please input your hexdecimal number:\n");
        scanf("%s",hex);
        a=0;
        for (int i=0;i<strlen(hex);i++){
            if ( !inarray ( hex[i],validchar,sizeof(validchar)/sizeof( char ) ) ){
                a=1;
                break;
            }
        }
        if (!a){
            printf("Thank you!\n");
            break;
        }
    }
}
