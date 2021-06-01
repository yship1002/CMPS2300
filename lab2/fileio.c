#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]){
    
    //Open the file to read data
    FILE* file=fopen(argv[1],"r");
    
    //If file not exist then print error message
    if (file==NULL){printf("ERROR READING FILE!");}

    //Store all the data in idle int array
    char input[100];
    fgets(input,20,file);
    int num_day=atoi(input);
    int idle[num_day];
    for (int i=0;i<num_day;i++){
        fgets(input,20,file);
        idle[i]=atoi(input);
    }
    fclose(file);
    
    //Open the file to write data into
    FILE* file1=fopen(argv[2],"w");
    fprintf(file1,"Here is the report of your computer's idle time!\n");

    //Reuse previous program to calculate idle report of computer
    float sum=0;
    int low=0;
    int high=0;
    
    //Compare and get the highest and lowest idle day and store it in low&high
    for (int i=0;i<num_day;i++){
         sum+=idle[i];
         if (idle[i]>idle[high]){
             high=i;
         }
         else if(idle[i]<idle[low]){
             low=i;
         }
     }
     float average=sum/num_day;
     float percentage=sum/(24*num_day*60);
    
    
    //Write result into the file
     fprintf(file1,"Your total time of idle time in this week is: %f\n",sum);
     fprintf(file1,"Your percentage of idle time: %f\n",percentage);
     fprintf(file1,"Average daily idle time: %f\n", average);
     fprintf(file1,"The day with highest load: %d\n",low+1);
     fprintf(file1,"The day with lowest load: %d\n",high+1);
    

    fclose(file1);
    
     return 0;

     }
