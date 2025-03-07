#include "main.h"

// External declaration of event count and current state
extern int count;
extern State current_state;

// Function to view stored events
// Parameters:
// - key: The key pressed on the keypad
void view_event(char key) 
{
    static char index = 0; // Keeps track of the currently displayed event
    
    // If no events are stored, display a message and allow exit
    if (count == 0) 
    {
        clcd_print(" NO EVENTS ", LINE1(3)); // Display "NO EVENTS" on the first line
        clcd_print(" STORED ", LINE2(4));   // Display "STORED" on the second line
        
        // SW4: Return to default state if pressed
        if (key == SW4) 
        {
            CLEAR_DISP_SCREEN;
            current_state = e_default;
            return;
        }
        return; // Exit function since there are no events
    }
    
    // SW2: Move to the next event if not at the last stored event
    if (key == SW2 && index < count - 1) 
    {
        index++; 
    } 
    // SW1: Move to the previous event if not at the first event
    else if (key == SW1 && index > 0) 
    {
        index--;  
    } 
    
    // SW4: Return to default state and reset index
    if (key == SW4) 
    {
        index = 0; // Reset index to start position
        CLEAR_DISP_SCREEN;
        current_state = e_default;
        return;
    }

    // Display event time format on the first LCD line
    clcd_print("  HH:MM AM/PM ", LINE1(0));

    // Display the event details stored in EEPROM on the second line
    clcd_putch(' ', LINE2(0));
    clcd_putch(' ', LINE2(1));
    clcd_putch(read_eeprom(index * 6), LINE2(2));     // Hour tens place
    clcd_putch(read_eeprom(index * 6 + 1), LINE2(3)); // Hour ones place
    clcd_putch(':', LINE2(4));
    clcd_putch(read_eeprom(index * 6 + 2), LINE2(5)); // Minute tens place
    clcd_putch(read_eeprom(index * 6 + 3), LINE2(6)); // Minute ones place
    clcd_putch(' ', LINE2(7));
    clcd_putch(read_eeprom(index * 6 + 4), LINE2(8)); // AM/PM first character (A/P)
    clcd_putch(read_eeprom(index * 6 + 5), LINE2(9)); // AM/PM second character (M)
}
