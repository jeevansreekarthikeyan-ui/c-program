#include <stdio.h>
int main()
{
    int ages[]={18,20,22,26,23,32,46};
    int length=sizeof(ages)/sizeof(ages[0]);
    int sum=0,i;
    float avg=0;
    for(i=0;i<length;i++){
        sum+=ages[i];
    }
    printf("\nthe sum of ages is %d",sum);
    avg=sum/length;
    printf("\nthe average of the ages is %f",avg);
    return 0;
}