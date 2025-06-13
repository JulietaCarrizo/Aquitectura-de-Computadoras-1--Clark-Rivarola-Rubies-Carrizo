#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "secuencias.h"
#include "util.h"

int velocidad = 2000;

void correrAutoFantastico() {
    system("cls");
    printf("SECUENCIA: Auto Fantastico (presione 'a' para salir)\n");

    int paso = 0;
    int direccion = 1; // 1 = derecha, 0 = izquierda
    int salir = 0;

    while (!salir) {
        unsigned char valor = 1 << paso;
        mostrarBinario(valor, velocidad);

        if (direccion) paso++;
        else paso--;

        if (paso >= 6) direccion = 0;
        if (paso <= 0) direccion = 1;

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'a') salir = 1;
        }

        ajustarVelocidad();
    }

    mostrarBinario(0, 0); // apaga luces al salir
}


void correrChoque() {
    system("cls");
    printf("SECUENCIA: Choque (presione 'a' para salir)\n");

    // Patrón simétrico: va del borde al centro y vuelve
    unsigned char secuencia[] = {
            0x81, // 10000001
            0x42, // 01000010
            0x24, // 00100100
            0x18, // 00011000 -> choque
            0x24, // 00100100
            0x42, // 01000010
            0x81  // 10000001
    };

    int i = 0, salir = 0;

    while (!salir) {
        mostrarBinario(secuencia[i], velocidad);
        i = (i + 1) % 7;

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'a') salir = 1;
        }

        ajustarVelocidad();
    }

    mostrarBinario(0, 0); // apaga luces al salir
}

void correrCarrera() {
    system("cls");
    printf("SECUENCIA: La carrera (presione 'a' para salir)\n");

    unsigned char secuencia[] = {
            0x80, // 10000000
            0x40, // 01000000
            0x20, // 00100000
            0x10, // 00010000
            0x08, // 00001000
            0x04, // 00000100
            0x02, // 00000010
            0x01  // 00000001
    };

    int i = 0;
    int salir = 0;

    while (!salir) {
        mostrarBinario(secuencia[i], velocidad);
        i = (i + 1) % 8;

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'a') salir = 1;
        }

        ajustarVelocidad();
    }

    mostrarBinario(0, 0); // apaga luces al salir
}


void correrSecuenciaPropia() {
    system("cls");
    printf("SECUENCIA: Propia (presione 'a' para salir)\n");

    unsigned char valores[] = {0xAA, 0x55}; // 10101010 / 01010101
    int i = 0, salir = 0;

    while (!salir) {
        mostrarBinario(valores[i % 2], velocidad);
        i++;

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'a') salir = 1;
        }

        ajustarVelocidad();
    }
    mostrarBinario(0, 0); // apaga luces al salir
}
