#ifndef CLOCK_H
#define CLOCK_H

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAYS 0x07
#define MONTH 0x08
#define YEAR 0x09
#define TIME_ZONE -3

unsigned int seconds();
unsigned int minutes();
unsigned int hours();
unsigned int year();
unsigned int day();
unsigned int month();
void timeToStr(char* dest);
void dateToStr(char* dest);

#endif