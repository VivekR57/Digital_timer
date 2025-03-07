# ⏰ Digital Timer Project

The **Digital Timer** project is a microcontroller-based device providing time, date, and alarm functionality with a user-friendly interface. The system utilizes a **PIC microcontroller** and a **DS1307 RTC** chip to manage a **12/24-hour clock**, date tracking, programmable alarms, and an intuitive **menu-driven interface** with a **16x2 character LCD** for display.

---

## 🛠 Project Features

- **Real-Time Clock:** Maintains accurate time and date using **DS1307 RTC**.
- **Dual Time Format:** Supports both 12-hour (**AM/PM**) and 24-hour formats.
- **Date Tracking:** Displays day, month, and year.
- **Alarm System:** Stores and manages multiple programmable alarms.
- **Non-Volatile Storage:** Saves settings and alarms in **EEPROM**.
- **Menu-Driven Interface:** Easy navigation through system functions.
- **User Controls:** Matrix keypad for input and system management.
- **Visual Feedback:** **16x2 LCD** display with clear indicators.
- **Automatic Alarm Sorting:** Chronologically organizes alarm events.
- **Alarm Indication:** Provides visual and audible signals for alarms.

---

## 🧰 Technology Stack

- **Microcontroller:** PIC microcontroller
- **Display:** 16x2 Character LCD
- **Real-Time Clock:** DS1307 RTC chip
- **Input Interface:** Matrix keypad with multiple switches
- **Memory Storage:** External EEPROM
- **Output Indicator:** Alarm output (connected to RB0)
- **Communication:** I2C for RTC and EEPROM interaction

---

## 🔄 Project Workflow

1. **Initialization Phase:** Configure peripherals (LCD, I2C, RTC, keypad) and set the default state.
2. **Main Operation Loop:** Retrieve current time, handle user input, and manage system states.
3. **State Transitions:** Navigate between operational states based on keypad inputs.
4. **Alarm Management:** Compare current time with stored alarms and activate output on match.

---

## 📂 File Structure

- `main.c` - Main control loop and state machine dispatcher.
- `default_screen.c` - Handles the default display of time, date, and upcoming alarms.
- `menu.c` - Implements the top-level menu and navigation.
- `set_view_event.c` - Manages alarm settings and viewing.
- `set_time_date.c` - Time and date setting interface.
- `set_time.c` - Provides a 12-hour time format with **AM/PM** toggle.
- `set_date.c` - Handles date input and validation.
- `set_event.c` - Creates and stores new alarms with automatic sorting.
- `view_event.c` - Allows browsing of stored alarms.
- `mkp.c` - Manages matrix keypad input and interaction.

---

## 🚀 Future Enhancements

- **Multiple Alarm Types:** Different tones for reminders, wake-up, etc.
- **Recurring Alarms:** Daily, weekly, and custom repeat options.
- **Backup Power System:** Battery backup for **RTC** and settings.
- **Extended Display Options:** Support for larger displays and custom icons.
- **Connectivity:** Add **Bluetooth/WiFi** for remote monitoring and control.
- **Environmental Sensors:** Display temperature, humidity, and weather trends.
- **Energy Efficiency:** Sleep modes, display timeout, and power management.

---

## 📧 Contact

For any questions, feel free to reach out:

- **Vivek**
- Email: [gopivivek57@gmail.com](mailto:gopivivek57@gmail.com)
- LinkedIn: [Vivek](https://www.linkedin.com/in/vivek57/)
