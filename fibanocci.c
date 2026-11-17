#include<stdio.h>
int main()
{
    int i,a=0,b=1,n,c=0;
    printf("enter the number");
    scanf("%d",&n);
    printf("fibanocci series upto %d  ",n);
    for(i=0;i<=n;i++)
    {   
        printf("%d\t",a);
        c=a+b;
        a=b;
        b=c;
    }
return 0;
}