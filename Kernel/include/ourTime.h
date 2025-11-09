#ifndef _TIME_H_
#define _TIME_H_

#define TICKS_FREQUENCY 18

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();

void timer_wait(int ticks);

#endif
