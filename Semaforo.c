#include <avr/io.h>
#include <avr/interrupt.h>

int count = 0;
int segundos = 0;
int boton_presionado = 0;
int state = 0;      /*  0: Estado 0 (inicial) - LDPV y LDPD encendidas y LDVD y LDPP apagadas
                        1: Estado 1 - LDPV parpadeando, LDPD encendida y LDVD y LDPP apagadas
                        2: Estado 2 - LDVD y LDPD encendidas y LDPV y LDPP apagadas
                        3: Estado 3 - LDVD y LDPP encendidas y LDPD y LDPV apagadas
                        4: Estado 4 - LDPP parpadeando, LDVD encendida y LDPV y LDPD apagadas
                        5: Estado 5 - LDPD y LDVD encendidas y LDPV y LDPP apagadas */
int main()
{
    // Configuracion puertos
    DDRB = 0x33;    // Salidas - pines 0, 1, 4 y 5
    DDRD = 0x00;    // Entradas - todos como entradas, particularmente interesa el pin 3

    // Habilitar interrupciones
    SREG = 0x80;    // Habilitacion global de interrupciones
    GIMSK = 0x80;   // Habilita INT1
    MCUCR = 0x04;   // External Interrupt 1 activado por un cambio en INT1 (Pin D3)

    // Configuracion timers
    TIMSK = 0x01;   // Habilitar comparacion con A
    TCCR0A = 0x02;  // Configurar modo de operacion normal, sin PWM
    TCCR0B = 0x05;   // Seleccionar frecuencia de reloj con prescaler de 1024 (frecuencia efectiva de 7812.5 Hz)
    OCR0A = 0x4E;   // Se almacena el numero a comparar con el timer para generar la excepcion 
    
    //  FSM del programa
    while (1)
    {
        switch (state)
        {
            case 0:         
                PORTB = 0x21;   // Estado 0 (inicial) - LDPV y LDPD encendidas y LDVD y LDPP apagadas                
                if (segundos < 10)
                {
                    state = 0;
                    if (count == 100)
                    {
                        segundos = segundos + 1;
                        count = 0;
                    }
                }
                else
                {
                    if (boton_presionado == 1)
                    {
                        state = 1;  
                        boton_presionado = 0;
                    }
                    else
                    {
                        state = 0;
                    }
                    count = 0;
                    segundos = 0;
                }
                break;
            
            case 1:                
                if (segundos < 3)               // Hacer que LDPV parpadee y LDPD encendida
                {
                    state = 1;      
                    if (count<50)
                    {
                        PORTB = 0x20;
                    }
                    if (count >= 50)
                    {
                        PORTB = 0x21;
                    }
                    if (count == 100)
                    {
                        segundos = segundos + 1;
                        count = 0;
                    }
                }
                else
                {
                    state = 2;
                    count = 0;
                    segundos = 0;
                }
                break;

            case 2:
                PORTB = 0x22;   // LDPD y LDVD encendidas
                if (count == 100)   // Medir 1 segundo
                {
                    state = 3;
                    count = 0;
                    segundos = 0;
                }
                else
                {
                    state = 2;
                }                
                break;
            case 3:
                PORTB = 0x12;   // LDPP y LDVD encendidas
                state = 3;
                if (count == 100)       // Medir 10 segundos
                {
                    segundos = segundos + 1;
                    count = 0;
                }
                if (segundos == 10)
                {
                    state = 4;
                    count = 0;
                    segundos = 0;
                }                
                break;

            case 4:
                if (segundos < 3)               // Hacer que LDPP parpadee y LDVD encendida
                {   
                    state = 4;
                    if (count<50)
                    {
                        PORTB = 0x02;
                    }
                    if (count>=50)
                    {
                        PORTB = 0x12;
                    }
                    if (count == 100)
                    {
                        segundos = segundos + 1;
                        count = 0;
                    }
                }
                else
                {
                    state = 5;
                    count = 0;
                    segundos = 0;
                }
                break;
            
            case 5:
                state = 5;
                PORTB = 0x22;
                if (count == 100)   // Medir 1 segundo
                {
                    state = 0;
                    count = 0;
                    segundos = 0;
                }
                break;
        }        
    }
}

ISR(INT1_vect)          // Interrupcion externa 1 - Ocurre al presionarse alguno de los botones
{
    boton_presionado = 1;
}

ISR(TIMER0_COMPA_vect)  // Se ejecuta esta interrupcion cada 10 ms
{
    count = count + 1;
}