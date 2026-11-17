#include<stdio.h>
int main()
{
    int num,n,sum=0,digit;
    printf("enter the number");
    scanf("%d",&num);
    n=num;
    while(num!=0)
    {
        digit=num%10;
        sum=sum+digit*digit*digit;
        num=num/10;
    }
    if(n==sum)
    {
        printf("%d is amstrong number",n);
    }
    else{
        printf("%d is not amstrong number",n);
    }
    return 0;
}