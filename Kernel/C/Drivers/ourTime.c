// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <ourTime.h>

extern void _hlt();

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / TICKS_FREQUENCY;
}

void timer_wait(int delta) { 
	int initialTicks = ticks;
	while(ticks - initialTicks < delta) {
		_hlt();
	}
}