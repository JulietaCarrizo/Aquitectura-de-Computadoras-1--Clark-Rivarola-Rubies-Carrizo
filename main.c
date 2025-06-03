#include <stdio.h>
#include <stdlib.h>
#include "secuencias.h"
#include "password.h"

int main() {
    if (!verificarPassword()) return 0;

    char opcion;
    do {
        system("cls");
        printf("===== MENU PRINCIPAL =====\n");
        printf("1. Auto Fantástico\n");
        printf("2. Choque\n");
        printf("3. La carrera\n");
        printf("4. Secuencia propia\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        opcion = getchar();
        while (getchar() != '\n'); // limpia buffer

        switch (opcion) {
            case '1': correrAutoFantastico(); break;
            case '2': correrChoque(); break;
            case '3': correrCarrera(); break;
            case '4': correrSecuenciaPropia(); break;
            case '0': printf("Saliendo...\n"); break;
            default: printf("Opción inválida.\n"); break;
        }
    } while (opcion != '0');

    return 0;
}
