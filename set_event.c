#include "main.h"
extern unsigned char time[9];
unsigned char address = 0;
unsigned char hour = 00, min = 00, field = 0;
unsigned char flag = 0;
unsigned long delay = 0;
int count = 0;
char status[2][3] = {"AM", "PM"};
char index = 0;
extern State current_state;

void set_event(char key)
{
    // Display header for time setting
    clcd_print(" HH:MM A/P ", LINE1(0));
    
    // Initialize time variables from stored time only once
    if (flag == 0)
    {
        hour = time[0] - '0';
        hour = hour * 10 + time[1] - '0';
        min = time[3] - '0';
        min = min * 10 + time[4] - '0';
        char ampm = read_ds1307(HOUR_ADDR);
        if (ampm & (1 << 5))
        {
            index = 1; // Set PM if bit 5 is set
        }  
        else
        {
            index=0;
        }
        flag = 1;
    }
    
    // Increase selected field value when SW1 is pressed
    if (key == SW1)
    {
        if (field == 0) // Increase hour
        {
            if (++hour > 12)
            {
                hour = 1;
            }
        }
        if (field == 1) // Increase minutes
        {
            if (++min > 59)
            {
                min = 0;
            }
        }
        if (field == 2) // Toggle AM/PM
        {
            index = !index;
        }
    }
    
    // Move to the next field when SW2 is pressed
    if (key == SW2)
    {
        if (++field > 2)
            field = 0;
    }
    
    // Display time on LCD, with blinking effect for selected field
    if (delay++ < 500)
    {
        clcd_putch(' ', LINE2(0));
        clcd_putch('0' + hour / 10, LINE2(1));
        clcd_putch('0' + hour % 10, LINE2(2));
        clcd_putch(':', LINE2(3));
        clcd_putch('0' + min / 10, LINE2(4));
        clcd_putch('0' + min % 10, LINE2(5));
        clcd_print(status[index], LINE2(7));
    }
    else
    {
        // Blink selected field for user feedback
        if (field == 0) // Blink hour
        {
            clcd_putch(' ', LINE2(1));
            clcd_putch(' ', LINE2(2));
        }
        else if (field == 1) // Blink minute
        {
            clcd_putch(' ', LINE2(4));
            clcd_putch(' ', LINE2(5));
        }
        else if (field == 2) // Blink AM/PM
        {
            clcd_putch(' ', LINE2(7));
            clcd_putch(' ', LINE2(8));
        }
    }
    
    // Reset delay counter for blinking effect
    if (delay >= 1000)
        delay = 0;
    
    // Store event time when SW4 is pressed
    if (key == SW4)
    {
        if (count == 0) // First entry
        {
            store_external_eeprom(hour, min, status, count);
            count++;
        }
        else
        {
            int flag = 0;
            int i;
            for (i = 0; (i < count) && (!flag); i++)
            {
                // Read stored event time from EEPROM
                char e_hr = (read_eeprom(i * 6) - '0') * 10 + (read_eeprom(i * 6 + 1) - '0');
                char e_min = (read_eeprom(i * 6 + 2) - '0') * 10 + (read_eeprom(i * 6 + 3) - '0');
                char e_ap = read_eeprom(i * 6 + 4);
                
                // Compare stored time with new time
                if (e_ap < status[index][0])
                    continue;
                else if (e_ap > status[index][0])
                {
                    shift_eeprom(i);
                    store_external_eeprom(hour, min, status, i);
                    count++;
                    flag = 1;
                }
                else
                {
                    if ((e_hr % 12) < (hour % 12))
                        continue;
                    else if ((e_hr % 12) > (hour % 12))
                    {
                        shift_eeprom(i);
                        store_external_eeprom(hour, min, status, i);
                        count++;
                        flag = 1;
                    }
                    else
                    {
                        if (e_min < min)
                            continue;
                        else if (e_min > min)
                        {
                            shift_eeprom(i);
                            store_external_eeprom(hour, min, status, i);
                            count++;
                            flag = 1;
                        }
                        else
                        {
                            flag = 1;
                            break;
                        }
                    }
                }
            }
            if (!flag) // Store at the last position
            {
                store_external_eeprom(hour, min, status, i);
                count++;
            }
        }
    }
    
    // Exit without saving when SW5 is pressed
    if (key == SW5)
    {
        CLEAR_DISP_SCREEN;
        index=0;
        flag=0;
        current_state = e_default;
        return;
    }
}

// Function to store event time in external EEPROM
void store_external_eeprom(char hour, char min, char status[2][3], int position)
{
    char arr[6];
    arr[0] = (hour / 10) + '0';
    arr[1] = (hour % 10) + '0';
    arr[2] = (min / 10) + '0';
    arr[3] = (min % 10) + '0';
    arr[4] = status[index][0];
    arr[5] = status[index][1];

    // Write time to EEPROM at the specified position
    for (int i = 0; i < 6; i++)
    {
        write_eeprom(((position) * 6) + i, arr[i]);
    }
}

// Function to shift EEPROM data to make space for a new event
void shift_eeprom(int pos)
{
    for (int i = count; i > pos; i--)
    {
        for (int j = 0; j < 6; j++)
        {
            char data = read_eeprom(((i - 1) * 6) + j); 
            write_eeprom((i * 6) + j, data);         
        }
    }
}
