#include "main.h"

// External declaration of the current state variable
extern State current_state;

// Variables to store day, month, and year values
unsigned char dd, mm, yy;

// External declaration of date array
extern unsigned char date[11];

// Flags and counters
unsigned char flag = 0; // Flag to check if date values have been extracted from the date array
unsigned char field = 0; // Keeps track of the currently selected field (day, month, or year)
unsigned long delay = 0; // Counter for display blinking effect

// Function to set the date
// Parameters:
// - key: The key pressed on the keypad
void set_date(char key)
{
    // Extract date values only once when the function is first called
    if (flag == 0)
    {
        yy = date[2] - '0';
        yy = yy * 10 + date[3] - '0';
        mm = date[5] - '0';
        mm = mm * 10 + date[6] - '0';
        dd = date[8] - '0';
        dd = dd * 10 + date[9] - '0';
        flag = 1;
    }

    // SW1: Increment the selected field (day, month, or year)
    if (key == SW1)
    {
        if (field == 0) // Increment day
        {
            if (++dd > 31)
            {
                dd = 1;
            }
        }
        else if (field == 1) // Increment month
        {
            if (++mm > 12)
            {
                mm = 1;
            }
        }
        else if (field == 2) // Increment year
        {
            if (++yy > 99)
            {
                yy = 0;
            }
        }
    }

    // SW2: Move to the next field (cycles through day, month, and year)
    if (key == SW2)
    {
        if (field++ == 2)
        {
            field = 0;
        }
    }

    // SW4: Save the modified date values to the RTC and return to default state
    if (key == SW4)
    {
        dd = ((dd / 10) << 4) | (dd % 10);
        write_ds1307(DATE_ADDR, dd);

        mm = ((mm / 10) << 4) | (mm % 10);
        write_ds1307(MONTH_ADDR, mm);

        yy = ((yy / 10) << 4) | (yy % 10);
        write_ds1307(YEAR_ADDR, yy);
        CLEAR_DISP_SCREEN;
        current_state = e_default;
        flag = 0;
        return;
    }

    // SW5: Discard changes and return to default state
    if (key == SW5)
    {
        CLEAR_DISP_SCREEN;
        current_state = e_default;
        flag = 0;
        return;
    }

    // Display the date format on the LCD
    clcd_print(" DD-MM-YY ", LINE1(0));

    // Display blinking effect for the selected field
    if (delay++ < 500)
    {
        // Display complete date normally
        clcd_putch(' ', LINE2(0));
        clcd_putch('0' + dd / 10, LINE2(1));
        clcd_putch('0' + dd % 10, LINE2(2));
        clcd_putch('-', LINE2(3));
        clcd_putch('0' + mm / 10, LINE2(4));
        clcd_putch('0' + mm % 10, LINE2(5));
        clcd_putch('-', LINE2(6));
        clcd_putch('0' + yy / 10, LINE2(7));
        clcd_putch('0' + yy % 10, LINE2(8));
    }
    else
    {
        // Blink the selected field by replacing it with spaces
        if (field == 0) // Blink day field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch(' ', LINE2(1));
            clcd_putch(' ', LINE2(2));
            clcd_putch('-', LINE2(3));
            clcd_putch('0' + mm / 10, LINE2(4));
            clcd_putch('0' + mm % 10, LINE2(5));
            clcd_putch('-', LINE2(6));
            clcd_putch('0' + yy / 10, LINE2(7));
            clcd_putch('0' + yy % 10, LINE2(8));
        }
        else if (field == 1) // Blink month field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch('0' + dd / 10, LINE2(1));
            clcd_putch('0' + dd % 10, LINE2(2));
            clcd_putch('-', LINE2(3));
            clcd_putch(' ', LINE2(4));
            clcd_putch(' ', LINE2(5));
            clcd_putch('-', LINE2(6));
            clcd_putch('0' + yy / 10, LINE2(7));
            clcd_putch('0' + yy % 10, LINE2(8));
        }
        else if (field == 2) // Blink year field
        {
            clcd_putch(' ', LINE2(0));
            clcd_putch('0' + dd / 10, LINE2(1));
            clcd_putch('0' + dd % 10, LINE2(2));
            clcd_putch('-', LINE2(3));
            clcd_putch('0' + mm / 10, LINE2(4));
            clcd_putch('0' + mm % 10, LINE2(5));
            clcd_putch('-', LINE2(6));
            clcd_putch(' ', LINE2(7));
            clcd_putch(' ', LINE2(8));
        }
    }
    
    // Reset delay counter to create blinking effect
    if (delay == 1000)
    {
        delay = 0;
    }
}
