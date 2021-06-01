//
//  bitwise.c
//  
//
//  Created by Jingzhi Yang on 2/22/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
void binary(unsigned int a){
    if (a==1){
        printf("%d",1);
    }else if (a==0){
        printf("%d",0);
    }
    else{
        binary(a/2);
        printf("%d",a%2);
    }
}
void prefix(unsigned int x, unsigned int n){
    //Calculate length of this number in binary representation
    int length=ceil(log(x)/log(2));
    int a=x>>(length-n);
    printf("This is result: %d\n",a);
    printf("This is binary representation: \n");
    binary(a);
    printf("\n");
}
void suffix(unsigned int x,unsigned int n){
    int a=((1<<n)-1)&x;
    printf("This is result: %d\n",a);
    printf("This is binary representation: \n");
    binary(a);
    printf("\n");
}
void toggle(unsigned int x, unsigned int n){
    // if it is rightmost bit let n=0!!!
    int a=x^(1<<n);
    printf("This is result: %d\n",a);
    printf("This is binary representation: \n");
    binary(a);
    printf("\n");
}
int main(){
    unsigned int number=100;
    unsigned int shift=3;
    prefix(number,shift);
    suffix(number,shift);
    toggle(number,shift);
    return 0;
}
