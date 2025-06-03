#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "password.h"

#define CLAVE "12345"

int verificarPassword() {
    char ingreso[6];
    int intentos = 0;

    while (intentos < 3) {
        printf("Ingrese su password de 5 dígitos: ");
        int i;
        for (i = 0; i < 5; i++) {
            ingreso[i] = getch();
            printf("*");
        }
        ingreso[5] = '\0';
        printf("\n");

        if (strcmp(ingreso, CLAVE) == 0) {
            printf("Bienvenido al sistema.\n");
            return 1;
        } else {
            printf("Password no válida.\n");
            intentos++;
        }
    }

    printf("Demasiados intentos. Abortando...\n");
    return 0;
}
