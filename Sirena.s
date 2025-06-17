    // Sirena.s — Efecto “Sirena Policial”
    .text
    .global ElSirenaASMB
    .global QUIT
    .global DELAY
    .global DELAY_3
    .global DisplayBinary
    .global LedOutput
    .global Delay
    .global Clear

ElSirenaASMB:
    PUSH {R0-R3, R4-R11, LR}
    BL   Clear

    // Carga DELAY inicial desde DELAY_3
    LDR  R11, =DELAY_3      // addr DELAY_3
    LDR  R6, [R11]          // val DELAY_3
    LDR  R9, =DELAY         // addr DELAY
    STR  R6, [R9]           // DELAY = DELAY_3

    LDR  R10, =QUIT         // addr QUIT

sirena_loop:
    // 1) Oscilación lateral N veces (a la izquierda y a la derecha)
    MOV  R8, #4            // N=4 oscilaciones (izq→der→izq→der)
    MOV  R5, #0            // índice: 0=izq,1=der

osc_loop:
    LDR  R7, [R10]
    CMP  R7, #1
    BEQ  sirena_done

    // Elegir máscara: posición según R5 (0 ó 1)
    CMP  R5, #0
    BEQ  left_led
    MOV  R4, #0x80        // derecha
    B    show_led

left_led:
    MOV  R4, #0x01        // izquierda

show_led:
    MOV  R0, R4
    MOV  R1, #4           // opción visual
    BL   DisplayBinary
    MOV  R0, R4
    BL   LedOutput

    // delay
    LDR  R6, [R9]
    MOV  R0, R6
    BL   Delay

    // alternar R5 (0→1, 1→0)
    EOR  R5, R5, #1

    // decrementa oscilaciones
    SUBS R8, R8, #1
    BGT  osc_loop

    // 2) Parpadeo conjunto M veces
    MOV  R8, #6          // M=6 parpadeos
blink_loop:
    LDR  R7, [R10]
    CMP  R7, #1
    BEQ  sirena_done

    // todos encendidos
    MOV  R4, #0xFF
    MOV  R0, R4
    MOV  R1, #3
    BL   DisplayBinary
    MOV  R0, R4
    BL   LedOutput
    LDR  R6, [R9]
    MOV  R0, R6
    BL   Delay

    // todos apagados
    MOV  R4, #0x00
    MOV  R0, R4
    BL   LedOutput
    MOV  R0, R4
    MOV  R1, #3
    BL   DisplayBinary
    LDR  R6, [R9]
    MOV  R0, R6
    BL   Delay

    SUBS R8, R8, #1
    BGT  blink_loop

    // reiniciar ciclo sirena
    B    sirena_loop

sirena_done:
    // guardar DELAY actual en DELAY_3
    LDR  R6, [R9]
    STR  R6, [R11]
    // reset QUIT
    MOV  R0, #0
    STR  R0, [R10]
    POP  {R0-R3, R4-R11, PC}

    .end
