/*
 * nerd3.c
 *
 * Created: 2013/05/30 11:00:36
 *  Device: ATMEGA88P
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "mystd.h"
#include "hhk2ascii.h"

/* -------------------------------------------------
 * definitions, globals and consts
 */
u1 HHKScanCode;
u1 ScanBuf[64];                         // 64 keys (0:pressed)

/* -------------------------------------------------
 * initializers
 */
static inline void init_devices(void)
{
    /* HHK SENSE */

    /* TIMER1 (16bit) */
    TCCR1B = (0x01<<CS10) | (1<<WGM12); // CLKio/1, CTC(Clear Timer on Compare Match)
                                        // CLKio: 1 MHz = internal RC osc (factory default)
    TIMSK1 = 1<<OCIE1A;                 // Compare A match interrupt
    OCR1A  = 200;                       // row a => 200us cycled (T=400us)
    
    /* PORTB */
    DDRB  = 0x7F;                       // b7:    IN   #PRESS
                                        // b6:    OUT  #OE
                                        // b5-b0: OUT  bit5-bit0
    PORTB = 0x00;


    /* Bluetooth */

    /* UART0: async, 9600, data=8, stop=1, pari=none */
    UBRR0  = 12;                                       // 1 MHz = internal RC osc (factory default)
    UCSR0A = (1<<U2X0);
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);                  /* enable receiver/transmitter */
}

static inline void init_rams(void)
{
    HHKScanCode = 0;

    for(u1 i=0; i<sizeof(ScanBuf); i++)
        ScanBuf[i] = 0x80;
}

/* -------------------------------------------------
 * sub functions
 */
static inline void uart_send(u1 ch)
{
  while( !(UCSR0A & (1<<UDRE0)) ) ;        /* wait for empty transmit buffer */

  UDR0 = ch;
}


/* -------------------------------------------------
 * main
 */
int main(void)
{
    init_devices();
    init_rams();
    __ei();

    uart_send('A');

    while(1)
    {
        //TODO:: Please write your application code 
    }

    return EXIT_SUCCESS;
}

/* -------------------------------------------------
 * interrupt service routines
 */
ISR(TIMER1_COMPA_vect)
{
    /* HHK key sense */
    u1 sw;

    __di();

    HHKScanCode = (HHKScanCode + 1) & 0x3F;
    PORTB = HHKScanCode | 0x40;         // #OE(PB6) --> HI
    PORTB &= 0x3F;                      // #OE(PB6) --> LO
    sw = PINB & 0x80;                   // #PRESS==LOなら当該キーが押されている, HIならリリースされている
    PORTB |= 0x40;                      // #OE(PC6) --> HI

    if(sw != ScanBuf[HHKScanCode]) {
        ScanBuf[HHKScanCode] = sw;      // update
        if(sw == 0)
            uart_send(hhk2ascii_tbl[HHKScanCode]);
            //ringbuf_write(&TXBuf, HHKScanCode);     // press --> send
    }

    __ei();
}

/* this program ends here */
