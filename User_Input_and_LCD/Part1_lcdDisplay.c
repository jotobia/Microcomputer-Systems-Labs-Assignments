#include "project.h"
#include <stdio.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    //turing both LEDS on
    LED1_Write( 1 ); 
    LED2_Write( 1 );
     
    LCD_Start(); //starts LCD    
    LCD_WriteControl(LCD_CLEAR_DISPLAY); //Clears any leftover data from last use 
    LCD_WriteControl(LCD_CURSOR_HOME); //sets cursor to home position
    LCD_PrintString("Welcome!"); //prints Welcome
    CyDelay(2000); //2 second delay
    
    LCD_WriteControl(LCD_CLEAR_DISPLAY); //Clears any leftover data from last use 
    LCD_WriteControl(LCD_CURSOR_HOME); //sets cursor to home position 
    LCD_PrintString("Push A Button :"); //prints Push a button

    char outputStr[256]; 
    int SW1_Count = 0;
    int SW2_Count = 0;
    int SW1_State;
    int SW2_State;
    
    for(;;)
    {   
 
        do
        {
            SW1_State=SW1_Read();
            SW2_State=SW2_Read();
        } while (SW1_State == 1 && SW2_State == 1);
        
        LCD_WriteControl(LCD_CLEAR_DISPLAY); //Clears any leftover data from last use 
        LCD_WriteControl(LCD_CURSOR_HOME); //sets cursor to home position 
        //if button 1 was pressed
        if (SW1_State == 0 )
        {    
            SW1_Count ++;
            LCD_PrintString("Button 1 Count"); 
            LCD_Position(1,0);
            sprintf(outputStr,"in Hex: %X", SW1_Count);
            LCD_PrintString( outputStr ); 
            LED1_Write( 1 ); 
            LED2_Write( 0 );
            CyDelay(100);
            //wait until button 1 is released
            while ( ( SW1_State=SW1_Read() ) == 0 )
            {
                //do nothing
            }
        }
        
        //if button 2 was pressed
        if ( SW2_State == 0 )
        {
            SW2_Count ++;
            LCD_PrintString("Button 2 Count"); 
            LCD_Position(1,0);
            sprintf(outputStr,"in Hex: %X", SW2_Count);
            LCD_PrintString( outputStr ); //prints Welcome
            LED1_Write( 0 ); 
            LED2_Write( 1 );
            CyDelay(100);
            //wait until button 2 is released
            while ( ( SW2_State=SW2_Read() ) == 0 )
            {
                //do nothing
            }
        }

        CyDelay(100);       
    }
}

