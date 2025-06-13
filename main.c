#include <stdio.h>
#include <stdlib.h>
#include "secuencias.h"
#include "password.h"

int main() {
    if (!verificarPassword()) return 0;

    char opcion;
    char buffer[10]; // para capturar la línea de entrada

    do {
        system("cls");
        printf("===== MENU PRINCIPAL =====\n");
        printf("1. Auto Fantastico\n");
        printf("2. Choque\n");
        printf("3. La carrera\n");
        printf("4. Secuencia propia\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        fgets(buffer, sizeof(buffer), stdin);  // lee la línea completa
        opcion = buffer[0]; // toma solo el primer carácter

        switch (opcion) {
            case '1': correrAutoFantastico(); break;
            case '2': correrChoque(); break;
            case '3': correrCarrera(); break;
            case '4': correrSecuenciaPropia(); break;
            case '0': printf("Saliendo...\n"); break;
            default: printf("Opcion invalida.\n"); break;
        }
    } while (opcion != '0');

    return 0;
}