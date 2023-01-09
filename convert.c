//To compile : gcc main.c sqlite3.c -o main.exe
// THIS FILE IS ONLY A TEST AND IT SEEMS TO BE WORKING 

#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

float gTomm(int valg) {
    return valg*0.18;
}

float mmTog(int valmm) {
    return valmm*5.5;
}

int main(int argc, char **argv){
    int conversion = 1;
    while (conversion!=0)
    {
        /* code */
        printf("Choisissez la conversion que vous souhaitez effectuer :\n");
    printf("0- Sortir\n1- g/L en mmol/L\n2- mmol/L en g/L\n");
    scanf("%d",&conversion);
    char condition = 0;
    float val;
    switch (conversion)
    {
    case 1:
        while (condition==0)
        {
            /* code */
            printf("Entrez la valeur en g/L : (0 pour sortir)\n>>>");
            scanf("%f",&val);
            if (val==0)
            {
                break;
            } else {
                printf("%f g/L = %f mmol/L\n",val,gTomm(val));
            }
            
        }
        
        break;

    case 2:
        while (condition==0)
        {
            /* code */
            printf("Entrez la valeur en mmol/L : (0 pour sortir)\n>>>");
            scanf("%f",&val);
            if (val==0)
            {
                break;
            }
            printf("%f mmol/L = %f g/L\n",val,mmTog(val));
            
        }
        
        break;    
    
    default:
        break;
    }
    }
    

  printf("taux en mmol/L : %f\n",gTomm(5));


return 0;
}