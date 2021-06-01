#include <stdio.h>
#include <stdlib.h>
int main(){
    int day[]={1,2,3,4,5,6,7};
    float idle[7];
    int size=sizeof(day)/sizeof(day[0]);
    for (int i=0;i<size;i++){
        printf("How many minutes is your system idle on day %d\n",day[i]);
        scanf("%f",&idle[i]);
    }
    float sum=0;
    int low=0;
    int high=0;
    for (int i=0;i<size;i++){
        sum+=idle[i];
        if (idle[i]>idle[high]){
            high=i;
        }
        else if(idle[i]<idle[low]){
            low=i;
        }
    }
    float average=sum/7;
    float percentage=sum/(24*7*60);
    printf("Your total time of idle time in this week is: %f\n",sum);
    printf("Your percentage of idle time: %f\n",percentage);
    printf("Average daily idle time: %f\n", average);
    printf("The day with highest load: %d\n",low+1);
    printf("The day with lowest load: %d\n",high+1);
}
