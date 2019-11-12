#include "project.h"
#include <stdio.h>

int main(void)
{
    //turning both LEDS on
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
        //waiting for one buttons to be pressed. While(both buttons are not pressed) 
        do
        {
            SW1_State=SW1_Read(); //reading the state of SW1
            SW2_State=SW2_Read(); //reading the state of SW2
        } while (SW1_State & SW2_State);
        
        LCD_WriteControl(LCD_CLEAR_DISPLAY); //Clears any leftover data from last use 
        LCD_WriteControl(LCD_CURSOR_HOME); //sets cursor to home position 
       
        //if button 1 was pressed
        if (SW1_State == 0 )
        {    
            SW1_Count ++; 
            LCD_PrintString("Button 1 Count"); 
            sprintf(outputStr,"in Hex: %X", SW1_Count); //saves "in Hex: SW2_Count" to outputStr 
        }
        //if button 2 was pressed
        else
        {
            SW2_Count ++;
            LCD_PrintString("Button 2 Count"); 
            sprintf(outputStr,"in Hex: %X", SW2_Count); //saves "in Hex: SW2_Count" to outputStr 
        }
        
        LCD_Position(1,0);
        LCD_PrintString( outputStr ); 
        //toggle both LEDs
        LED1_Write( !SW1_Read() ); 
        LED2_Write( !SW2_Read() );
        
        CyDelay(100); //debounce delay for pushing button
        
        //waiting for both buttons to be unpressed. While(one is pressed and other isn't) 
        do
        {
            SW1_State=SW1_Read(); //reading the state of SW1
            SW2_State=SW2_Read(); //reading the state of SW2
        } while (SW1_State ^ SW2_State);

        CyDelay(100); //debounce delay for releasing button
    }
}

