#include <sounds.h>
#include <usyscalls.h>

void playNote(int freq, int duration, int waitAfter) {
    sys_play_sound(freq, duration, waitAfter);
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
    playNote(90, 2, 0);
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
    playNote(SI2, 2, 0);

}

void wallSound() {
    playNote(226, 2, 0);
}

void paddleHitSound() {
    playNote(459, 2, 0);
}

void goalSound() {
    playNote(490, 8, 30);
}