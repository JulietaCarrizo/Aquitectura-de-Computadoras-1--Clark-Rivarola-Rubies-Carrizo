#include <stdio.h>
#include <conio.h>
#include <stdlib.h>   // <- esta es la clave
#include "secuencias.h"
#include "util.h"

int velocidad = 2000;

void correrAutoFantastico() {
    system("cls");
    printf("SECUENCIA: Auto Fantástico\n");

    int i, direccion = 1, paso = 0;
    while (!kbhit() || getch() != 'a') {
        unsigned char valor = 1 << paso;
        mostrarBinario(valor, velocidad);
        if (direccion) paso++; else paso--;
        if (paso == 7) direccion = 0;
        if (paso == 0) direccion = 1;
        ajustarVelocidad();
    }
    mostrarBinario(0, 0); // apagar luces
}

void correrChoque() {
    system("cls");
    printf("SECUENCIA: Choque\n");
    unsigned char secuencia[] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42};
    int i = 0;
    while (!kbhit() || getch() != 'a') {
        mostrarBinario(secuencia[i++ % 6], velocidad);
        ajustarVelocidad();
    }
    mostrarBinario(0, 0);
}

void correrCarrera() {
    system("cls");
    printf("SECUENCIA: La carrera\n");
    unsigned char secuencia[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    int i = 0;
    while (!kbhit() || getch() != 'a') {
        mostrarBinario(secuencia[i++ % 8], velocidad);
        ajustarVelocidad();
    }
    mostrarBinario(0, 0);
}

void correrSecuenciaPropia() {
    system("cls");
    printf("SECUENCIA: Propia (alternancia 0xAA / 0x55)\n");

    unsigned char valores[] = {0xAA, 0x55};
    int i = 0;
    while (!kbhit() || getch() != 'a') {
        mostrarBinario(valores[i++ % 2], velocidad);
        ajustarVelocidad();
    }
    mostrarBinario(0, 0);
}
