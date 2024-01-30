/*
 * lcd.h
 *
 *  Created on: 9 Kas 2022
 *      Author: onur_
 */

#ifndef LCD_H_
#define LCD_H_

#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7

void lcdcmd(unsigned char);

void lcdchar(unsigned char d);

void lcdnum(int e);

void lcdclr(void);

void lcdgo(char x, char y);
void lcdtemp(int t);

void lcdinit(unsigned char DL, unsigned char N, unsigned char F); //DL, N, F
//DL 4/8
//N 1=1 satýr, 2=2 satýr
//F 7 => 5x7, 10 => 5x10

#endif /* LCD_H_ */
