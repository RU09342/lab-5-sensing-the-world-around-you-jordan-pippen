//  Lab 5: Sensing the World Around You
//         -Sensors and Signal Conditioning
//              -Using ADC12 with MSP430F5529
//
//
//   Mathew Philippou and Michael Lonetto
//   Rowan University
//   November 2017
//   Embedded Systems Section 2
//   Professor Tang and T.A. Russel Trafford

#include <msp430.h>

void initializeUART(void)       // from Milestone
{
           P4SEL |= BIT4;          // UART TX
           P4SEL |= BIT5;          // UART RX
           UCA1CTL1 |= UCSWRST;    // Resets state machine
           UCA1CTL1 |= UCSSEL_2;   // SMCLK
           UCA1BR0 = 6;            // 9600 Baud Rate
           UCA1BR1 = 0;            // 9600 Baud Rate
           UCA1MCTL |= UCBRS_0;    // Modulation
           UCA1MCTL |= UCBRF_13;   // Modulation
           UCA1MCTL |= UCOS16;     // Modulation
           UCA1CTL1 &= ~UCSWRST;   // Initializes the state machine
           UCA1IE |= UCRXIE;       // Enables USCI_A0 RX Interrupt
}

void initializeADC12(void){
    P6SEL |= 0x01; //Configure 6.0 to be used as ADC input
    ADC12CTL0 = ADC12SHT02 // 32 ADC12CLK cycles in sampling period
                + ADC12ON; // Enables ADC12_A
    ADC12CTL1 = ADC12SHP;  // SAMPCON signal is source from the sampling timer
    ADC12CTL0 |= ADC12ENC; // Enables conversion
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    initializeUART();           // Calls upon UART function
    initializeADC12();          // Calls upon ADC12 function
    __enable_interrupt();       // enables interrupt below

    //Start sampling
    while (1)
    {
        __delay_cycles(1000);
        ADC12CTL0 |= ADC12SC;        // Start sampling/conversion
        __bis_SR_register(GIE);
        __no_operation();            // For debugger
    }

}


#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV,USCI_UCTXIFG)){
    case 0: break; //if vector is zero, there is no interrupt
    case 2:
        if(UCA1RXBUF == 100)  // Input 'd' from keyboard to get data
                   UCA1TXBUF = ADC12MEM0 >> 4; // Shifts ADC12 value 4 bits and transfers
                                               // it sends it through UART to be received by REALTERM
        else
               UCA1TXBUF = 1; // Will send 01 through UART to be received by REALTERM
                              // Meant to show that the wrong button was pressed
        break;

    }

}
