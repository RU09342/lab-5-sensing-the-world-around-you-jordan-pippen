//  Lab 5: Sensing the World Around You
//         -Sensors and Signal Conditioning
//              -Using ADC10 with MSP430G2553
//
//
//   Mathew Philippou and Michael Lonetto
//   Rowan University
//   November 2017
//   Embedded Systems Section 2
//   Professor Tang and T.A. Russel Trafford

#include <msp430.h>

void initializeUART(void)
{
           P1SEL |= BIT1;          // UART RX
           P1SEL |= BIT2;          // UART TX

           UCA0CTL1 |= UCSSEL_2;   // SMCLK
           UCA0BR0 = 104;          // 9600 baud
           UCA0BR1 = 0;            // 9600 baud
           UCA0MCTL = UCBRS0;      // Modulation
           UCA0CTL1 &= ~UCSWRST;   // **Initialize USCI state machine**
}

void initializeADC10(void){
    P1SEL |= BIT7;       //Configure 1.7 to be used as ADC input
    ADC10CTL1 = INCH_7 + // P1.7
                SHS_1;   //
    ADC10AE0 = BIT7;     // ADC10 on P1.7
    ADC10CTL0 = SREF_0 + // ADC10 External Reference (VEREF+ < VEREF–)
                ADC10SHT_2 + //Sets sampling time
                REFON +  //Reference is turned on
                ADC10ON; //Enables ADC10_A
    ADC10CTL0 |= ENC;    //Enables Conversion.
}

void InitializeTimer(void)
{
    TA0CCTL0 = CCIE;     // Enable interrupt
    TA0CCR0 = 50000;     // CCR0 Period, Samples per second
    TACCR1 = 30000;      // CCR1 Period, Samples per second
    TA0CTL = TASSEL_2 + MC_1; // SMCLK
}

void InitializeClock(void)
{
    DCOCTL = 0;            // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ; // Set BC1 to 1Mhz
    DCOCTL = CALDCO_1MHZ;  // Set DC0 to 1Mhz
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    initializeUART();           // Calls upon UART function
    initializeADC10();          // Calls upon ADC10 function
    InitializeTimer();          // Calls upon Timer function
    InitializeClock();          // Calls upon Clock function

    __enable_interrupt();       // enables interrupt below

        //Start sampling
        while (1)
        {
            __delay_cycles(1000);
            ADC10CTL0 |= ADC10SC;        // Start sampling/conversion
            __bis_SR_register(GIE);
            __no_operation();            // For debugger
        }

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
switch(__even_in_range(IFG2, UCA0TXIFG)){
    case 0: break; //if vector is zero, there is no interrupt
    case 2:
        if(UCA0RXBUF == 100)  // Input 'd' from keyboard to get data
                   UCA0TXBUF = ADC10MEM >> 2; // Shifts ADC10 value 2 bits and transfers
                                               // it sends it through UART to be received by REALTERM
        else
               UCA0TXBUF = 1; // Will send 01 through UART to be received by REALTERM
                              // Meant to show that the wrong button was pressed
        break;
    }
}

//Timer A0 interrupt
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
    __bic_SR_register_on_exit(CPUOFF);
}
