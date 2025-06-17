#include <stdlib.h>
#include <pthread.h> //biblioteca que se encarga de dividir las tareas de los subprocesos más facilmente
#include <stdio.h>
#include "EasyPIO.h"
#include <ncurses.h>
#include <string.h>

#define PASSWORD "12345"
#define QUIT_KEY 'q'
#define DELAY_INTERVAL 250 // aumentar o disminuir la velocidad
#define DEFAULT_DELAY 1000 // retraso default

unsigned int QUIT;
size_t DELAY; // variable dinamica para utilizada por las flechas arriba y abajo
size_t DELAY_1 = DEFAULT_DELAY;
size_t DELAY_2 = DEFAULT_DELAY;
size_t DELAY_3 = DEFAULT_DELAY;
size_t DELAY_4 = DEFAULT_DELAY;
size_t DELAY_5 = DEFAULT_DELAY;

pthread_mutex_t lock; // mutual exclusion, sincroniza el uso de un recurso compartido entre hilos. Así lo definimos

const unsigned char led[8] = {7, 8, 25, 24, 23, 18, 15, 14}; // definimos los leds

extern void ElSirenaASMB(); // assembly Sirena
extern void ElBarraASMB();  // assembly Barra

void *ElSirenaASMBP(void *arg);
void *ElBarraASMBP(void *arg);

void Delay(size_t a)
{                   // crear un bucle de espera, permite controlar el tiempo de las secuencias de LEDs
    a = a * 100000; // para convertirlo en un valor mayor y poder controlarlo
    while (a--)     // retardo
        ;
}

unsigned int Login()
{
    char *right_password = PASSWORD;
    char input[6]; // Ajuste para 5 caracteres + terminador nulo
    int i = 0;
    char c;
    printf("Ingrese su password de 5 digitos: ");

    while (1)
    {
        c = getchar(); // lee un carácter desde la entrada estándar

        if (c == '\n')
        {
            input[i] = '\0';
            break;
        }
        else if (c == 127 || c == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            if (i < 5)
            {
                printf("*");
                input[i] = c;
                i++;
            }
        }
        if (i == 5)
            break;
    }

    input[i] = '\0';

    if (strcmp(input, right_password) == 0)
    { // comparar cadenas con strcmp
        printf("\n\rAcceso concedido\n\r");
        return 1;
    }
    else
    {
        printf("\n\rAcceso denegado\n\r");
        return 0;
    }
}

void MoveCursorToOrigin()
{ // mover el cursor de la terminal al origen
    unsigned int i = 10;
    while (i--)
        printf("\033[F"); // mueve ekl cursor hacia arriba
}

void Clear()
{                      // limpiar pantalla de la terminal
    printf("\033[2J"); // borra contenido
    MoveCursorToOrigin();
}

void DisplayBinary(unsigned char DISPLAY, unsigned int option)
{ // convierte los 1 en '*' y los 0 en '_'
    char display[8];
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1)
    { // POINTER deja desplazar bit a bit desde 0x80
        if (POINTER & DISPLAY)
        { // se verifica si el bit correspondiente está encendido
            display[i] = '*';
            i++;
        }
        else
        {
            display[i] = '_';
            i++;
        }
    }

    switch (option)
    {
    case 1:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mAuto Fantastico\033[0m\n\r\n\r");
        break;
    case 2:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Choque\033[0m\n\r\n\r");
        break;
    case 3:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mLa Barra\033[0m\n\r\n\r");
        break;
    case 4:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mLa Sirena\033[0m\n\r\n\r");
        break;
    case 5:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mLa Carrera\033[0m\n\r\n\r");
        break;
    case 0:
        break;
    }

    printf("%s", display);

    if (option)
    {
        printf("\n\r\n\r");
        printf("\033[1;30mPresione la tecla Q para salir\033[0m\n\r");
    }

    MoveCursorToOrigin();
}

void LedOutput(unsigned char DISPLAY)
{
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1)
    { // ciclo para controlar los LEDs
        if (POINTER & DISPLAY)
        {
            digitalWrite(led[i], 1); // se activa
            i++;
        }
        else
        {
            digitalWrite(led[i], 0); // no se activa
            i++;
        }
    }
}

void *keyListener()
{
    while (!QUIT)
    {
        int key = getch();         // leer una tecla
        pthread_mutex_lock(&lock); // bloqueamos que otro hilo acceda a este recurso compartido
        if (key == QUIT_KEY)
            QUIT = 1; // establece QUIT a 1 si se presiona la tecla q
        else if (key == KEY_UP)
        {
            if (DELAY - DELAY_INTERVAL != 0)
                DELAY -= DELAY_INTERVAL; // disminuye
        }
        else if (key == KEY_DOWN)
        {
            DELAY += DELAY_INTERVAL; // aumenta
        }
        pthread_mutex_unlock(&lock); // desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    }
    return NULL;
}

void *AutoFantastico()
{
    Clear();
    // ← ARRANCAMOS guardando la velocidad previa
    pthread_mutex_lock(&lock);
    DELAY = DELAY_1;
    pthread_mutex_unlock(&lock);

    while (!QUIT)
    {
        unsigned char DISPLAY = 0x80;
        for (int i = 0; i < 7; i++)
        {
            if (QUIT)
                break;

            // ← LEEMOS la velocidad al vuelo
            pthread_mutex_lock(&lock);
            size_t d = DELAY;
            pthread_mutex_unlock(&lock);

            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY >>= 1;
            Delay(d);
        }
        for (int i = 0; i < 7; i++)
        {
            if (QUIT)
                break;

            pthread_mutex_lock(&lock);
            size_t d = DELAY;
            pthread_mutex_unlock(&lock);

            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY <<= 1;
            Delay(d);
        }
    }

    // ← AL SALIR, guardamos la velocidad final
    pthread_mutex_lock(&lock);
    DELAY_1 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *ElChoque()
{
    Clear();
    pthread_mutex_lock(&lock);
    DELAY = DELAY_2;
    pthread_mutex_unlock(&lock);

    unsigned int table[] = {0x42, 0x24, 0x18, 0x24, 0x42, 0x81};
    unsigned char DISPLAY = 0x81;

    while (!QUIT)
    {
        int table_size = sizeof(table) / sizeof(table[0]);
        for (int i = 0; i < table_size; i++)
        {
            if (QUIT)
                break;

            pthread_mutex_lock(&lock);
            size_t d = DELAY;
            pthread_mutex_unlock(&lock);

            DisplayBinary(DISPLAY, 2);
            LedOutput(DISPLAY);
            DISPLAY = table[i];
            Delay(d);
        }
    }

    pthread_mutex_lock(&lock);
    DELAY_2 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *correrCarrera()
{
    Clear();
    // 1) Cargo la última velocidad guardada
    pthread_mutex_lock(&lock);
    DELAY = DELAY_5;
    pthread_mutex_unlock(&lock);

    // 2) Tabla de 10 patrones con ambas luces moviéndose:
    unsigned char table[] = {
        0x80, 0xC0, 0xA0, 0x90,
        0x48, 0x28, 0x22, 0x12,
        0x0C, 0x03};
    int steps = sizeof(table) / sizeof(table[0]);

    // 3) Bucle principal: se repite hasta que QUIT == 1
    while (!QUIT)
    {
        // 3a) Recorro toda la tabla de patrones
        for (int i = 0; i < steps; i++)
        {
            if (QUIT)
                break; // chequeo inmediato de ‘q’

            // 3b) Leo la velocidad actual (flechas ↑↓)
            pthread_mutex_lock(&lock);
            size_t d = DELAY;
            pthread_mutex_unlock(&lock);

            // 3c) Muestro y enciendo LEDs
            DisplayBinary(table[i], 5);
            LedOutput(table[i]);

            // 3d) Espero el retardo
            Delay(d);
        }
        // Cuando termina el for, vuelve al inicio del while y repite
    }

    // 4) Al salir (cuando QUIT == 1), guardo velocidad y limpio
    pthread_mutex_lock(&lock);
    DELAY_5 = DELAY; // conservo la última velocidad
    QUIT = 0;        // reseteo para la próxima vez
    pthread_mutex_unlock(&lock);
    return NULL;
}

// Wrapper para llamar a ElSirenaASMB desde pthread
void *ElSirenaASMBP(void *arg)
{
    ElSirenaASMB(); // llama a la rutina Assembly
    return NULL;
}

// Wrapper para llamar a ElBarraASMB desde pthread
void *ElBarraASMBP(void *arg)
{
    ElBarraASMB(); // llama a la rutina Assembly
    return NULL;
}

void *LaSirena()
{
    // 1) Inicializo el display
    Clear();

    // 2) Cargo la velocidad guardada para esta secuencia
    pthread_mutex_lock(&lock);
    DELAY = DELAY_3;
    pthread_mutex_unlock(&lock);

    // 3) Llamo al código Assembly que implementa toda la lógica
    ElSirenaASMB();

    // 4) Al volver de Assembly, guardo la velocidad final y reseteo QUIT
    pthread_mutex_lock(&lock);
    DELAY_3 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *LaBarra()
{
    // 1) Limpio pantalla
    Clear();

    // 2) Cargo la velocidad guardada para la secuencia “Barra”
    pthread_mutex_lock(&lock);
    DELAY = DELAY_4;
    pthread_mutex_unlock(&lock);

    // 3) Llamo a la rutina en Assembly que implementa toda la lógica
    ElBarraASMB();

    // 4) Al volver de Assembly, guardo la velocidad final y reseteo QUIT
    pthread_mutex_lock(&lock);
    DELAY_4 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock);

    return NULL;
}

void App()
{
    unsigned char option[2]; // Ajuste para recibir un solo carácter
    Clear();

    int i = 0, a = 0;

    for (i; i < 3; i++)
    { // funcion para el login, si se intenta 3 veces, aborta
        Delay(2000);
        Clear();
        if (Login())
            break;
    }

    if (i == 3)
        exit(0);

    do
    {
        Delay(2000); // retardo 2s
        DisplayBinary(0, 0);
        Clear();
        printf("\033[?25h"); // muestra el cursor

        printf("------ S E C U E N C I A S  D E  L U C E S ------\n\r");
        printf("1. Auto Fantastico\n\r");
        printf("2. El Choque\n\r");
        printf("3. La barra\n\r");
        printf("4. La sirena\n\r");
        printf("5. La Carrera\n\r");
        printf("0. Salir\n\r");
        printf("-------------------------------------------------\n\r");
        printf("\n\rSeleccione una opcion: ");

        scanf(" %c", &option[0]); // ignora espacios antes de leer el caracter, y va destinado a option[0]
        option[1] = '\0';         // Aseguramos que sea un string válido y la cadena termine despues del primer caracter

        printf("\033[?25l"); // oculta el cursor

        pthread_t threads[2]; // arreglo de 2 hilos

        switch (option[0])
        {
        case '1':
            pthread_create(&threads[0], NULL, keyListener, NULL);    // se crean los hilos
            pthread_create(&threads[1], NULL, AutoFantastico, NULL); // se crean los hilos
            pthread_join(threads[0], NULL);                          // espera a que los hilos terminen
            pthread_join(threads[1], NULL);                          // espera a que los hilos terminen
            break;

        case '2':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElChoque, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '3':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElBarraASMBP, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '4':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElSirenaASMBP, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '5':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, correrCarrera, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '0':
            printf("\n\rSaliendo del Programa..\n\r");
            Delay(2000);
            Clear();
            a = 1;
            break;

        default:
            printf("\n\rIngrese una opcion valida\n\r");
            break;
        }
    } while (a == 0);
}

int main()
{
    pioInit();
    pthread_mutex_init(&lock, NULL); // inicializa el mutex

    for (int i = 0; i < 8; i++)
    {
        pinMode(led[i], OUTPUT);
    }

    initscr();             // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE);  // Habilitar la entrada de teclado especial
    nodelay(stdscr, TRUE); // Configurar getch() para no bloquear
    noecho();              // No mostrar los caracteres ingresados en pantalla

    App();

    endwin();
    pthread_mutex_destroy(&lock); // destruye el mutex creado anteriormente

    return 0;
}
