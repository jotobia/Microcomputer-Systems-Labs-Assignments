#include "project.h"
#include "math.h"
#include <stdio.h>

#define NumSamples 100000
#define SQR(x) ( (x)*(x) )
#define MOE 75

#define mean_squared_Triangle 333
#define mean_squared_Sine 500
#define mean_squared_Rect 1000

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    int32_t sum; // running sum, zero it out each 100,000 samples
    int32_t saved_mean=0; // gets sum/N at the end of each batch
    int64_t sum_of_sqs; // running sum of (f(n) - saved_mean)2
    int32_t min; // capture min(f(n)) reset to INT_MAX each batch
    int32_t max; // capture max(f(n)) reset to INT_MIN each batch
    int64_t mean_sq; // Need 64 bits to avoid overflow
    uint16 mean_sq_16bit;
    uint32_t NUM_SAMPS;
    uint16_t ADCreading;
    char outputStr[256];
    
    ADC_Start();
    VDAC_Start();
    LCD_Start();
    
    for( ; ; )
    {
        min = 10000; //arbitrarily large number
        max = 0;
        sum = 0;
        sum_of_sqs = 0;
        for(NUM_SAMPS = 1; NUM_SAMPS <= NumSamples; NUM_SAMPS++ )
        {
            ADC_IsEndConversion( ADC_WAIT_FOR_RESULT );
            ADCreading = ADC_GetResult16(); //setting ADC reading = current ADC reading.
            VDAC_SetValue(ADCreading >> 4);
            
            sum += ( ADCreading );
            sum_of_sqs += ( SQR(ADCreading-saved_mean) );
            if (ADCreading < min )
                min = ADCreading;
            if (ADCreading > max )
                max = ADCreading;
        }
        saved_mean = (sum ) / NumSamples;
        mean_sq = (sum_of_sqs / NumSamples); // Normalize by number of samples
        mean_sq = (1000*mean_sq/ ( SQR((max - min)/2) )); // Normalize amplitude, so 1.0 -> 1000
        
        
        //displaying data
        LCD_WriteControl(LCD_CLEAR_DISPLAY);
        LCD_WriteControl(LCD_CURSOR_HOME);
        sprintf(outputStr,"m:%d",min);
        LCD_PrintString( outputStr );
        
        LCD_Position( 0,7 );
        sprintf(outputStr,"M:%d",max);
        LCD_PrintString( outputStr );
        
        LCD_Position( 1,0 );
        sprintf(outputStr,"v:%d",saved_mean);
        LCD_PrintString( outputStr );
        
        LCD_Position( 1,7 );
        mean_sq_16bit = mean_sq & 0xFFFF; //converting mean_square into a printable 16-bit value
        sprintf(outputStr,"q:%d",mean_sq_16bit);
        LCD_PrintString( outputStr );
        
        LCD_Position( 1,15 );
        if( mean_sq_16bit >= (mean_squared_Triangle-MOE) && mean_sq_16bit <= (mean_squared_Triangle+MOE)) 
            LCD_PrintString("T");
        else if( mean_sq_16bit >= (mean_squared_Sine-MOE) && mean_sq_16bit <= (mean_squared_Sine+MOE)) 
            LCD_PrintString("S");
        else if( mean_sq_16bit >= (mean_squared_Rect-MOE) && mean_sq_16bit <= (mean_squared_Rect+MOE)) 
            LCD_PrintString("R");
    }
}


