#include "main.h"  // Include main header file

unsigned char flag = 0;  // Flag to check if time has been initialized
unsigned char field = 0;  // Field selection (0: Hour, 1: Min, 2: Sec, 3: AM/PM)
unsigned long delay = 0;  // Delay counter for blinking effect
extern unsigned char time[9];  // External time variable
unsigned char hour, min, sec;  // Variables to store time values
extern State current_state;  // External variable for system state
char status[2][3] = {"AM", "PM"};  // AM/PM status strings
char index = 0;  // Index to track AM/PM status

/**
 * Function to set the time using keypad inputs
 * @param key: Key pressed on the keypad
 */
void set_time(char key) 
{
    // Initialize time values from stored time on first function call
    if (flag == 0) 
    {
        hour = time[0] - '0';
        hour = hour * 10 + time[1] - '0';
        min = time[3] - '0';
        min = min * 10 + time[4] - '0';
        sec = time[6] - '0';
        sec = sec * 10 + time[7] - '0';
        char ampm = read_ds1307(HOUR_ADDR);
        if (ampm & (1 << 5))
        {
            index = 1;  // Set PM flag if bit 5 is set
        }  
        flag = 1;
    }

    // Increase selected field value when SW1 is pressed
    if (key == SW1) 
    {
        if (field == 0)  // Increase hour
        {
            if (++hour > 12)
            {
                hour = 1;
            }
        } 
        if (field == 1)  // Increase minutes
        {
            if (++min > 59) 
            {
                min = 0;
            }
        } 
        if (field == 2)  // Increase seconds
        {
            if (++sec > 59) 
            {
                sec = 0;
            }
        } 
        if (field == 3)  // Toggle AM/PM
        { 
            index = !index;
        }
    }

    // Move to the next field when SW2 is pressed
    if (key == SW2) 
    {
        if (field++ == 3) 
        {
            field = 0;
        }
    }

    // Save time to RTC and exit when SW4 is pressed
    if (key == SW4) 
    {
        hour = (((hour / 10) << 4) | (hour % 10)) | (1 << 6) | (index << 5);
        write_ds1307(HOUR_ADDR, hour);

        min = ((min / 10) << 4) | (min % 10);
        write_ds1307(MIN_ADDR, min);

        sec = ((sec / 10) << 4) | (sec % 10);
        write_ds1307(SEC_ADDR, sec);

        CLEAR_DISP_SCREEN;
        current_state = e_default;
        flag = 0;
        return;
    }

    // Exit without saving when SW5 is pressed
    if (key == SW5) 
    {
        CLEAR_DISP_SCREEN;
        current_state = e_default;
        flag = 0;
        return;
    }

    // Display the time format on LCD
    clcd_print(" HH:MM:SS A/P", LINE1(0));

    // Show the time values or blink the selected field
    if (delay++ < 500)
    {
        // Display all fields
        clcd_putch(' ', LINE2(0));
        clcd_putch('0' + hour / 10, LINE2(1));
        clcd_putch('0' + hour % 10, LINE2(2));
        clcd_putch(':', LINE2(3));
        clcd_putch('0' + min / 10, LINE2(4));
        clcd_putch('0' + min % 10, LINE2(5));
        clcd_putch(':', LINE2(6));
        clcd_putch('0' + sec / 10, LINE2(7));
        clcd_putch('0' + sec % 10, LINE2(8));
        clcd_print(status[index], LINE2(10));
    } 
    else
    {
        // Blink the selected field for user feedback
        if (field == 0)  // Blink hour field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch(' ', LINE2(1));
            clcd_putch(' ', LINE2(2));
            clcd_putch(':', LINE2(3));
            clcd_putch('0' + min / 10, LINE2(4));
            clcd_putch('0' + min % 10, LINE2(5));
            clcd_putch(':', LINE2(6));
            clcd_putch('0' + sec / 10, LINE2(7));
            clcd_putch('0' + sec % 10, LINE2(8));
            clcd_print(status[index], LINE2(10));
        } 
        else if (field == 1)  // Blink minute field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch('0' + hour / 10, LINE2(1));
            clcd_putch('0' + hour % 10, LINE2(2));
            clcd_putch(':', LINE2(3));
            clcd_putch(' ', LINE2(4));
            clcd_putch(' ', LINE2(5));
            clcd_putch(':', LINE2(6));
            clcd_putch('0' + sec / 10, LINE2(7));
            clcd_putch('0' + sec % 10, LINE2(8));
            clcd_print(status[index], LINE2(10));
        } 
        else if (field == 2)  // Blink second field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch('0' + hour / 10, LINE2(1));
            clcd_putch('0' + hour % 10, LINE2(2));
            clcd_putch(':', LINE2(3));
            clcd_putch('0' + min / 10, LINE2(4));
            clcd_putch('0' + min % 10, LINE2(5));
            clcd_putch(':', LINE2(6));
            clcd_putch(' ', LINE2(7));
            clcd_putch(' ', LINE2(8));
            clcd_print(status[index], LINE2(10));
        }
        else if (field == 3)  // Blink AM/PM field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch('0' + hour / 10, LINE2(1));
            clcd_putch('0' + hour % 10, LINE2(2));
            clcd_putch(':', LINE2(3));
            clcd_putch('0' + min / 10, LINE2(4));
            clcd_putch('0' + min % 10, LINE2(5));
            clcd_putch(':', LINE2(6));
            clcd_putch('0' + sec / 10, LINE2(7));
            clcd_putch('0' + sec % 10, LINE2(8));
            clcd_putch(' ', LINE2(10));
            clcd_putch(' ', LINE2(11));
        }
    }

    // Reset delay counter after a full cycle
    if (delay == 1000) 
    {
        delay = 0;
    }
}
