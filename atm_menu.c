#include<stdio.h>
int main()
{
    int atm_menu;
    printf("enter choice");
    scanf("%d",&atm_menu);
    switch(atm_menu)
    {
        case 1:
            printf("check balance");
            break;
        case 2:
            printf("deposit money");
            break;
        case 3:
            printf("withdraw money");
            break;
        case 4:
            printf("exit");
            break;
        default:
            printf("invalid");
    }
    return 0;
}