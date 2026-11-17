#include <stdio.h>
int main()
{
    float time;
    printf("enter time :");
    scanf("%f",&time);
    if(time>=8.10&&time<=8.40)
    {
        printf("thank you");
    }
    else
    {
        printf("Autentication failed");
    }
    return 0;
}