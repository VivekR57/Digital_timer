#include "main.h"

unsigned char clock_reg[3];    // Stores hours, minutes, and seconds
unsigned char calender_reg[4]; // Stores year, month, date, and day
unsigned char time[9];         // Formatted time string
unsigned char date[11];        // Formatted date string
State current_state = e_default;
extern int i;
extern int count;
char hour, min, current_ap;
char e_hr, e_min, e_ap, aflag;

static void init_config(void)
{
    init_clcd();   // Initialize LCD
    init_i2c();    // Initialize I2C communication
    TRISB = 0x00;  // Set PORTB as output
    init_ds1307(); // Initialize RTC (DS1307)
    init_mkp();    // Initialize matrix keypad
}

void main()
{
    init_config(); // Configure peripherals

    unsigned char sec = 0;
    int default_scr = 0;
    unsigned char key;

    while (1)
    {
        get_time(); // Retrieve current time from RTC
        key = read_matrix_keypad(STATE); // Read keypad input

        if (current_state == e_default)
        {
            // Update screen only if seconds change
            if (sec != clock_reg[2])
            {
                sec = clock_reg[2];
                default_scr++;

                if (default_scr == 7)
                {
                    default_scr = 0;
                    CLEAR_DISP_SCREEN;
                }
                if (default_scr == 5)
                {
                    CLEAR_DISP_SCREEN;
                }
            }

            if (default_scr < 5)
            {
                display_time(); // Show current time
                get_date();     // Retrieve current date from RTC
                display_date(); // Show current date
                default_screen();
            }
            else
            {
                if (count == 0)
                {
                    i = 0;
                    clcd_print("No Events", LINE2(0));
                    clcd_print("Time", LINE1(0));
                }
                else
                {
                    i = i % count;

                    // Extract current hour, minute, and AM/PM
                    hour = (time[0] - '0') * 10 + (time[1] - '0');
                    min = (time[3] - '0') * 10 + (time[4] - '0');
                    current_ap = (clock_reg[0] & 0x20) ? 'P' : 'A';

                    // Read stored alarm details from EEPROM
                    e_hr = (read_eeprom(i * 6) - '0') * 10 + (read_eeprom(i * 6 + 1) - '0');
                    e_min = (read_eeprom(i * 6 + 2) - '0') * 10 + (read_eeprom(i * 6 + 3) - '0');
                    e_ap = read_eeprom(i * 6 + 4);

                    // Display alarm time
                    clcd_print("Alarm", LINE2(0));
                    clcd_putch((e_hr / 10) + '0', LINE2(6));
                    clcd_putch((e_hr % 10) + '0', LINE2(7));
                    clcd_putch(':', LINE2(8));
                    clcd_putch((e_min / 10) + '0', LINE2(9));
                    clcd_putch((e_min % 10) + '0', LINE2(10));
                    clcd_putch(' ', LINE2(11));
                    clcd_putch(e_ap, LINE2(12));
                    clcd_putch('M', LINE2(13));
                }

                clcd_print("Time", LINE1(0));
                clcd_print(time, LINE1(5));

                // Display AM/PM if in 12-hour mode
                if (clock_reg[0] & 0x40)
                {
                    clcd_print((clock_reg[0] & 0x20) ? "PM" : "AM", LINE1(13));
                }
            }

            // Check if alarm should trigger
            if (hour == e_hr && min == e_min && current_ap == e_ap)
            {
                RB0 = 1; // Activate alarm
                aflag = 1;
            }
            else
            {
                if (aflag == 1)
                {
                    i = (i + 1) % count; // Move to next event
                    aflag = 0;
                }
                RB0 = 0; // Deactivate alarm
            }

            if (key == SW4) // Enter menu mode
            {
                current_state = e_menu;
                CLEAR_DISP_SCREEN;
            }
        }
        else if (current_state == e_menu)
        {
            menu(key);
        }
        else if (current_state == e_set_view_event)
        {
            set_view_event(key);
        }
        else if (current_state == e_set_time_date)
        {
            set_time_date(key);
        }
        else if (current_state == e_set_time)
        {
            set_time(key);
        }
        else if (current_state == e_set_date)
        {
            set_date(key);
        }
        else if (current_state == e_view_event)
        {
            view_event(key);
        }
        else if (current_state == e_set_event)
        {
            set_event(key);
        }
    }
}

static void get_time(void)
{
    // Read hours, minutes, and seconds from RTC
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    // Convert to 12-hour or 24-hour format
    if (clock_reg[0] & 0x40) // 12-hour mode
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    else // 24-hour mode
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }

    // Format minutes and seconds
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0'; // Null-terminate the string
}

static void get_date(void)
{
    // Read year, month, date, and day from RTC
    calender_reg[0] = read_ds1307(YEAR_ADDR);
    calender_reg[1] = read_ds1307(MONTH_ADDR);
    calender_reg[2] = read_ds1307(DATE_ADDR);
    calender_reg[3] = read_ds1307(DAY_ADDR);

    // Format date as "YYYY-MM-DD"
    date[0] = '2';
    date[1] = '0';
    date[2] = '0' + ((calender_reg[0] >> 4) & 0x0F);
    date[3] = '0' + (calender_reg[0] & 0x0F);
    date[4] = '-';
    date[5] = '0' + ((calender_reg[1] >> 4) & 0x0F);
    date[6] = '0' + (calender_reg[1] & 0x0F);
    date[7] = '-';
    date[8] = '0' + ((calender_reg[2] >> 4) & 0x0F);
    date[9] = '0' + (calender_reg[2] & 0x0F);
    date[10] = '\0'; // Null-terminate the string
}

void display_date(void)
{
    clcd_print("Date", LINE1(0)); // Display label
    clcd_print(date, LINE1(5));   // Display formatted date
}

void display_time(void)
{
    clcd_print("Time", LINE2(0)); // Display label
    clcd_print(time, LINE2(5));   // Display formatted time

    // Display AM/PM if in 12-hour mode
    if (clock_reg[0] & 0x40)
    {
        clcd_print((clock_reg[0] & 0x20) ? "PM" : "AM", LINE2(13));
    }
}
