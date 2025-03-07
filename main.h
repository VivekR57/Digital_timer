#ifndef MAIN_H
#define	MAIN_H

#include<xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "mkp.h"
#include "e_eprom.h"

typedef enum {
    e_menu,
    e_default,
    e_set_view_event,
    e_set_time_date,
    e_set_time,
    e_set_date,
    e_set_event,
    e_view_event
} State;

static void get_time(void);
static void get_date(void);
void display_date(void);
void display_time(void);
void default_screen(void);



void menu(char key);
void set_time_date(char key);
void set_time(char key);
void set_date(char key);


void set_view_event(char key);

void set_event(char key);
void view_event(char key);
void store_external_eeprom(char hour,char min,char status[2][3], int position);
void shift_eeprom(int position);

#endif	/* MAIN_H */

