#include <stdio.h>
int main()
{
    int account_active;
    float balance,withdrawal_amount;
    printf("enter ur account status(1=active,0==inactive)");
    scanf("%d",&account_active);
    printf("enter balance");
    scanf("%f",&balance);
    printf("enter withdrawal amount");
    scanf("%f",&withdrawal_amount);
    if(account_active==1)
    {
        if(balance>=withdrawal_amount)
        {
            printf("amount withdrawal successfull");
        }else{
            printf("not able to withdraw");
        }
    }else{
        printf("account not active");
    }
    return 0;
}