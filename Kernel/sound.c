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
    // tempos de 20 ticks
    // primera parte
    playNote(MI, 1, 2);
    playNote(LA, 1, 2);
    playNote(SI, 1, 2);
    playNote(DO2, 3, 2);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 2);
    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(SI, 1, 2);
    playNote(LA, 1, 2);
    playNote(DO2, 6, 2);
    playNote(LA, 8, 20);

    // 2da parte
    playNote(MI, 1, 2);
    playNote(LA, 1, 2);
    playNote(SI, 1, 2);
    playNote(DO2, 3, 2);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 2);
    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(SI, 1, 2);
    playNote(LA, 1, 2);
    playNote(RE2, 20, 12);
    

    //parte tres
    playNote(SI, 1, 2);
    playNote(DO2, 1, 2);
    playNote(SI, 1, 2);
    playNote(RE2, 3, 2);

    playNote(RE2, 1, 2);
    playNote(RE2, 1, 4);

    playNote(RE2, 1, 2);
    playNote(RE2, 1, 2);
    playNote(RE2, 1, 2);
    playNote(RE2, 1, 4);

    playNote(MI2, 1, 2);
    playNote(RE2, 1, 4);

    playNote(RE2, 1, 2);
    playNote(MI2, 1, 2);
    playNote(RE2, 6, 2);
    playNote(DO2, 20, 12);

    //PARTE 4
    playNote(MI, 1, 2);
    playNote(LA, 1, 2);
    playNote(SI, 1, 2);
    playNote(DO2, 3, 2);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 4);

    playNote(DO2, 1, 2);
    playNote(DO2, 1, 2);
    playNote(SI, 1, 2);
    playNote(SI, 1, 4);

    playNote(SI, 2, 2);
    playNote(SI, 2, 4);

    playNote(SI, 1, 2);
    playNote(DO2, 1, 2);
    playNote(LA, 16, 4);

    //BOMBOS
    playNote(90, 1, 3);
    playNote(90, 1, 2);
    playNote(90, 2, 16);
}

void playSimpsons() {
    playNote(DO, 2, 4);
    playNote(MI, 2, 4);
    playNote(FASharp, 2, 4);
    playNote(LA, 2, 3);
    playNote(SOL, 3, 4);
    playNote(MI, 2, 2);
    playNote(DO, 2, 2);
    playNote(LA, 2, 3);
    playNote(FASharp, 1, 2);
    playNote(FASharp, 1, 2);
    playNote(FASharp, 1, 2);
    playNote(SOL, 2, 5);
    playNote(FASharp, 1, 2);
    playNote(FASharp, 1, 2);
    playNote(FASharp, 1, 2);
    playNote(SOL, 1, 3);
    playNote(LA, 6, 6);
    playNote(SI2, 2, 2);

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