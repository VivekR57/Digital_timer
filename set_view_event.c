#include "main.h"  // Include main header file

extern State current_state;  // External variable for system state

/**
 * Function to handle the Set/View Event menu navigation
 * @param key: The key pressed on the keypad
 */
void set_view_event(char key)
{
    static char sub_menu[][16] = {"SET EVENT", "VIEW EVENT"};  // Menu options
    static char m_ind = 0;  // Menu index (not used)
    static char s_flag = 0; // Selection flag (0 = Set Event, 1 = View Event)

    // Move selection to "SET EVENT" if SW1 is pressed
    if (key == SW1 && s_flag == 1)
    {
        s_flag = 0;
    }

    // Move selection to "VIEW EVENT" if SW2 is pressed
    if (key == SW2 && s_flag == 0)
    {
        s_flag = 1;
    }

    // Confirm selection and update state if SW4 is pressed
    if (key == SW4)
    {
        CLEAR_DISP_SCREEN;
        current_state = (s_flag == 0) ? e_set_event : e_view_event;
        return;
    }

    // Return to default state if SW5 is pressed
    if (key == SW5)
    {
        CLEAR_DISP_SCREEN;
        current_state = e_default;
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
    clcd_print(sub_menu[m_ind], LINE1(2));
    clcd_print(sub_menu[m_ind + 1], LINE2(2));
}
