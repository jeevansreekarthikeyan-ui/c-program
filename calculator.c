#include<stdio.h>
int main()
{
    int operator;
    printf("enter the operator for operation");
    scanf("%d",&operator);

    switch (operator)
    {
    case 1:
        printf("addition (+)");
        break;
    case 2:
        printf("subration(-)");
        break;
    case 3:
        printf("multiplication(*)");
        break;
    case 4:
        printf("division(/)");
        break;
    
    default:
        printf("invalid operation");
        break;
    }
return 0;
}