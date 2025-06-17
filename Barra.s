    // Barra.s — Efecto “Barra Creciente y Decreciente”
    .text
    .global ElBarraASMB       // Nuevo entry point
    .global QUIT
    .global DELAY
    .global DELAY_4
    .global DisplayBinary
    .global LedOutput
    .global Delay
    .global Clear

ElBarraASMB:
    PUSH {R0,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear

    MOV   R4, #0               // Init DISPLAY
    LDR   R11, =DELAY_4        // Dirección de DELAY_4
    LDR   R6, [R11]            // R6 = DELAY_4 (valor)
    LDR   R9, =DELAY           // Dirección de DELAY
    STR   R6, [R9]             // DELAY = DELAY_4

    LDR   R10, =QUIT           // Dirección de QUIT
    LDR   R8, =bar_table       // Dirección de nuestra tabla

while_loop:
    MOV   R5, #0               // índice i = 0

for_loop:
    LDR   R7, [R10]            // comprueba QUIT
    CMP   R7, #1
    BEQ   break_loop

    // Mostrar en texto y en LEDs
    MOV   R0, R4
    MOV   R1, #3
    BL    DisplayBinary
    MOV   R0, R4
    BL    LedOutput

    // Cargar next DISPLAY desde la tabla
    LDRB  R4, [R8, R5]

    // Retardo
    LDR   R6, [R9]
    MOV   R0, R6
    BL    Delay

    ADD   R5, R5, #1
    CMP   R5, #16
    BLT   for_loop            // 16 pasos por ciclo

    // Si QUIT sigue 0, repetir ciclo
    LDR   R7, [R10]
    CMP   R7, #0
    BEQ   while_loop

break_loop:
    STR   R6, [R11]           // guardamos DELAY
    MOV   R0, #0
    STR   R0, [R10]           // QUIT = 0 (reset)
    POP   {R0,R4,R5,R6,R7,R8,R9,R10,R11,PC}

    .data
bar_table:
    .byte 0x01,0x03,0x07,0x0F
    .byte 0x1F,0x3F,0x7F,0xFF
    .byte 0x7F,0x3F,0x1F,0x0F
    .byte 0x07,0x03,0x01,0x00

    .end
