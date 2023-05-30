// codigo de osdev
#include <sound.h>

//Play sound using built in speaker
static void play_sound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;

    //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div) );
    outb(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

//make it shutup
static void nosound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void playNote(int freq, int duration, int waitAfter) {
    play_sound(freq);
    timer_wait(duration);
    nosound();
    timer_wait(waitAfter);
}

void playBSong() {
    playNote(MI2, 3, 3);
    playNote(LA2, 3, 3);
    playNote(SI2, 3, 3);
    for (int i = 0; i < 8; i++)
        playNote(DO3, 3, 3);
    playNote(SI2, 3, 2);
    playNote(LA2, 3, 2);
    playNote(DO3, 3, 2);
    playNote(LA2, 5, 0);
}

//Make a beep
void beep() {
    play_sound(226);
    timer_wait(2);
    nosound();

    timer_wait(10);

    play_sound(459);
    timer_wait(4);
    nosound();

    timer_wait(10);

    play_sound(490);
    timer_wait(8);
    nosound();
}