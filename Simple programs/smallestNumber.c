// The user enters five float numbers in an array 
// and the program prints the smallest given number.

/* START OF FILE */

#include "project.h"
#include <stdio.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    
    
    float numbers[5];
    int i = 0;
    float smallestNumber = 0;
    
    printf("\nEnter five numbers: \n");

    for (i=0 ; i<5 ; i++) {
        printf("Enter number %d: ", i+1);
        scanf("%f", &numbers[i]);
    }
    
    smallestNumber = numbers[0];

    for (i=0 ; i<5 ; i++) {
        if (numbers[i] < smallestNumber) {
            smallestNumber = numbers[i];
        }
    }
    
    printf("The smallest number you entered was: %.3f\n", smallestNumber);
    
    for(;;)
    {
        /* Place your application code here. */
    }
}



/* [] END OF FILE */
