#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "util.h"

void mostrarBinario(unsigned char valor, int delay) {
    printf("\r");
    for (int i = 7; i >= 0; i--) {
        printf("%c", (valor & (1 << i)) ? 'a' : '.');
    }
    fflush(stdout);
    Sleep(delay);
}

void ajustarVelocidad() {
    if (kbhit()) {
        char tecla = getch();
        if (tecla == 72) { // flecha arriba
            if (velocidad > 200) velocidad -= 100;
        } else if (tecla == 80) { // flecha abajo
            if (velocidad < 5000) velocidad += 100;
        }
    }
}


