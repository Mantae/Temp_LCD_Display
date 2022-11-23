#include "stm32f7xx_hal.h"

void lcd_init (void); //initialize Lcd

void lcd_send_cmd (char cmd); // send command to lcd

void lcd_send_data (char data); //send data to lcd

void lcd_send_string (char *str); //send string to the lcd

void lcd_put_cur(int row, int col); //put cursor at the entered position

void lcd_clear(void); //clear lcd screen

