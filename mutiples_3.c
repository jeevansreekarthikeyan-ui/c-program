#include <stdio.h>
int main()
{
    int i,n;
    printf("enter the range:");
    scanf("%d",&n);
    for(i=1;i<n;i++)
    {
        if(i%5==0)
        {
            continue;
        }else if(i%3==0)
        {
            printf("%d\n",i);
        }else if(i>3*(n/5))
        {
            break;
        }
    }
    return 0;
}