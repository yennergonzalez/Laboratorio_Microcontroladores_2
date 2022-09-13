#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int count = 0;
int countb = 0;

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
    TCCR0A = 0x00;  // Configurar modo de operacion normal, sin PWM
    TCCR0B = 0x05;   // Seleccionar frecuencia de reloj con prescaler de 1024 (frecuencia efectiva de 125 Hz)
    OCR0B = 0x7D;   // Se almacena el numero a comparar con el timer para generar la excepcion
    

    
    while (1)
    {
        PORTB = 0x01;   // LDPV encendida indefinidamente mientras no se presione el boton

        if (count == 200)
        {
            
            count = 0;
            if (countb == 2)
            {
                PORTB = 0x10;
                countb = 0;
            }
            else
            {
                countb = countb + 1;
            }

        }
    }
}

ISR(INT1_vect)
{
    PORTB = 0x30;
    _delay_ms(10000);
}


ISR(TIMER0_COMPA_vect)
{
    count = count + 1;
}

