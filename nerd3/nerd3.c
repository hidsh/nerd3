/*
 * nerd3.c
 *
 * Created: 2013/05/30 11:00:36
 *  Device: ATMEGA88P
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "mystd.h"

/* -------------------------------------------------
 * definitions, globals and consts
 */
u1 cnt = 0;

/* -------------------------------------------------
 * interrupt service routines
 */
ISR(TIMER1_COMPA_vect)
{
    __di();

    PORTB = 0xFE | (cnt++ % 2);         /* LED(PB0) invert */
    
    __ei();
}
/* -------------------------------------------------
 * initializers
 */
static inline void init_devices(void)
{
    /* TIMER1 (16bit) */
    TCCR1B = (0x01<<CS10) | (1<<WGM12); /* CLKio/1, CTC(Clear Timer on Compare Match) */
                                        /* CLKio: 1 MHz = internal RC osc (factory default) */
    TIMSK1 = 1<<OCIE1A;                 /* Compare A match interrupt */
    OCR1A  = 200;                       /* row a => 200us cycled (T=400us) */
    
    /* PORTB: b7-b1:INPUT, b0:OUTPUT, all:HI */
    DDRB  = 0x01;
    PORTB = 0xFF;
}

/* -------------------------------------------------
 *  main
 */
int main(void)
{
    init_devices();
    __ei();

    while(1)
    {
        //TODO:: Please write your application code 
    }

    return EXIT_SUCCESS;
}
/* this program ends here */
