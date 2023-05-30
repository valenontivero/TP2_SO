// codigo de osdev
#include <sound.h>
#include <lib.h>
#include <time.h>

//Play sound using built in speaker
void play_sound(uint32_t nFrequence) {
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
void nosound() {
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
    playNote(MI, 3, 2);
    playNote(LA, 3, 2);
    playNote(SI, 3, 2);
    playNote(DO2, 2, 3);
    playNote(DO2, 1, 1);
    playNote(DO2, 1, 3);

    playNote(DO2, 2, 1);
    playNote(DO2, 1, 3);

    playNote(DO2, 2, 1);
    playNote(DO2, 1, 3);

    playNote(DO2, 3, 1);

    playNote(SI, 3, 2);
    playNote(LA, 3, 2);
    playNote(DO2, 3, 2);
    playNote(LA, 5, 0);
}

/* DO MI FA# LA SOL MI DO la fa# fa# fa# sol
fa# fa# fa# sol la# si
si re# fa RE# DO# si la fa do# do# do# re#
mi sol# la# DO# si sol#
DO MI FA# LA SOL MI DO la fa# fa# fa# sol
fa# fa# fa# sol la# si */

void playSimpsons() {
    playNote(DO, 3, 2);
    playNote(MI, 3, 2);

}
//Make a beep
void beep(/* int freq, int duration */) {
    // Wall sound
    play_sound(226);
    timer_wait(2);
    nosound();

    timer_wait(10);


    // Paddle hit sound
    play_sound(459);
    timer_wait(4);
    nosound();

    timer_wait(10);

    // Score sound ?
    play_sound(490);
    timer_wait(8);
    nosound();
}