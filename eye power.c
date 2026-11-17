#include<stdio.h>
int main()
{
    int eye_power;
    printf("Power level:");
    scanf("%d",&eye_power);
    if(eye_power==0)
    {
        printf("Normal");
    }
    else if(eye_power<0&&eye_power>=-16)
    {
        printf("Short sightedness\n");
        if(eye_power<=-1&&eye_power>=-4)
        {
            printf("Minimum");
        }
        if(eye_power<=-5&&eye_power>=-8)
        {
            printf("Moderate");
        }
        if(eye_power<=-9&&eye_power>=-12)
        {
            printf("High");
        }
        if(eye_power<=-13&&eye_power>=-16)
        {
            printf("Chronic");
        }
    }
    else if (eye_power>0&&eye_power<=16)
    {
       printf("Long sightedness\n");
        if(eye_power>=1&&eye_power<=4)
        {
            printf("Minimum");
        }
        if(eye_power>=5&&eye_power<=8)
        {
            printf("Moderate");
        }
        if(eye_power>=9&&eye_power<=12)
        {
            printf("High");
        }
        if(eye_power>=13&&eye_power<=16)
        {
            printf("Chronic");
        }
    }
    else
    {
        printf("Invalid");
    }
    return 0;
}