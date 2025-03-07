#include "main.h"

extern unsigned char time[9];  // Stores current time as a string
extern unsigned char clock_reg[3];  // Stores clock register values
extern int count;  // Number of stored events in EEPROM
unsigned char hour = 0, min = 0;
int i;

void default_screen(void) 
{
    // Convert time string to integer values for hour and minute
    hour = (time[0] - '0') * 10 + (time[1] - '0'); 
    min = (time[3] - '0') * 10 + (time[4] - '0'); 

    // Determine if the current time is AM or PM based on clock register
    char current_ap = (clock_reg[0] & 0x20) ? 'P' : 'A';  

    // Iterate through stored events in EEPROM
    for (i = 0; i < count; i++)
    {
        // Read stored hour, minute, and AM/PM indicator from EEPROM
        char e_hr = (read_eeprom(i * 6) - '0') * 10 + (read_eeprom(i * 6 + 1) - '0');
        char e_min = (read_eeprom(i * 6 + 2) - '0') * 10 + (read_eeprom(i * 6 + 3) - '0');
        char e_ap = read_eeprom(i * 6 + 4); 

        // Compare stored AM/PM with current AM/PM
        if (e_ap < current_ap)
        {
            continue; // Skip earlier AM/PM periods
        }
        else if (e_ap > current_ap)
        {
            break; // Stop checking as we passed the relevant period
        }
        else 
        {
            // Compare stored hour with current hour (12-hour format)
            if ((e_hr % 12) < (hour % 12))
            {
                continue; // Skip earlier hours
            }
            else if ((e_hr % 12) > (hour % 12))
            {
                break; // Stop checking as we passed the relevant hour
            }
            else 
            {
                // Compare stored minute with current minute
                if (e_min < min)
                {
                    continue; // Skip earlier minutes
                }
                else if (e_min > min)
                {
                    break; // Stop checking as we passed the relevant minute
                }
                else
                {
                    break; // Exact match found, stop checking
                }
            }
        }
    }
}
