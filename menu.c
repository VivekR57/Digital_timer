#include "main.h"  // Include main header file

extern State current_state;  // External variable representing the current system state

/**
 * Function to handle menu navigation and selection
 * @param key: The key pressed on the keypad
 */
void menu(char key)
{
    static char menu[][16] = {"SET/VIEW EVENT", "SET TIME/DATE"};  // Menu options
    static char m_ind = 0;  // Menu index 
    static char s_flag = 0; // Selection flag (0 = first option, 1 = second option)

    // Move selection to the first option if SW1 is pressed
    if (key == SW1 && s_flag == 1)
    {
        s_flag = 0;
    }

    // Move selection to the second option if SW2 is pressed
    if (key == SW2 && s_flag == 0)
    {
        s_flag = 1;
    }

    // Confirm selection and update state if SW4 is pressed
    if (key == SW4)
    {
        current_state = (s_flag == 0) ? e_set_view_event : e_set_time_date;
        CLEAR_DISP_SCREEN;
        return;
    }

    // Return to default state if SW5 is pressed
    if (key == SW5)
    {
        current_state = e_default;
        CLEAR_DISP_SCREEN;
        return;
    }

    // Display selection indicator on LCD
    if (s_flag == 0)
    {
        clcd_putch('-', LINE1(0));
        clcd_putch('>', LINE1(1));
        clcd_putch(' ', LINE2(0));
        clcd_putch(' ', LINE2(1));
    }
    else
    {
        clcd_putch(' ', LINE1(0));
        clcd_putch(' ', LINE1(1));
        clcd_putch('-', LINE2(0));
        clcd_putch('>', LINE2(1));
    }

    // Display menu options
    clcd_print(menu[m_ind], LINE1(2));
    clcd_print(menu[m_ind + 1], LINE2(2));
}
