#include "HAL_TIMER.h"
#include "../GPIO/HAL_GPIO.h"


volatile TimerCallback tmr1_callback = 0;
volatile TimerCallback tmr2_callback = 0;


void tmr_setup_period(int timer, int ms) 
{
    unsigned int pr;

    // Use 1:256 prescaler
    int prescaler_bits = 0b11;
    float ticks_per_ms = 72000000.0 / 256.0 / 1000.0;

    pr = (unsigned int)(ticks_per_ms * ms);

    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        TMR1 = 0;
        PR1 = pr;
        T1CONbits.TCKPS = prescaler_bits;
        T1CONbits.TON = 1;
    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        TMR2 = 0;
        PR2 = pr;
        T2CONbits.TCKPS = prescaler_bits;
        T2CONbits.TON = 1;
    }
}

int tmr_wait_period(int timer) 
{
    if (timer == TIMER1 && IFS0bits.T1IF == 0) {
        while (IFS0bits.T1IF == 0);
        IFS0bits.T1IF = 0;
        return 0;

    } else if (timer == TIMER2 && IFS0bits.T2IF == 0) {
        while (IFS0bits.T2IF == 0);
        IFS0bits.T2IF = 0;
        return 0;
    }
   
    return 1;
  
}


void tmr_wait_ms(int timer, int ms)
{
    unsigned int pr;

    // Use 1:64 prescaler
    int prescaler_bits = 0b10;
    float ticks_per_ms = 72000000.0 / 64.0 / 1000.0;

    pr = (unsigned int)(ticks_per_ms * 1);

    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        TMR1 = 0;
        PR1 = pr;
        T1CONbits.TCKPS = prescaler_bits;
        T1CONbits.TON = 1;
        for(int i = 0 ; i<ms ; i++)
        {
        while (IFS0bits.T1IF == 0);
        IFS0bits.T1IF = 0;
        }
        
        T1CONbits.TON = 0;

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        TMR2 = 0;
        PR2 = pr;
        T2CONbits.TCKPS = prescaler_bits;
        T2CONbits.TON = 1;
        
        for(int i = 0 ; i<ms ; i++)
        {
        while (IFS0bits.T2IF == 0);
        IFS0bits.T2IF = 0;
        }
        
        T2CONbits.TON = 0;
    }
}


void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;  // Clear interrupt flag

    if (tmr1_callback != 0) {
        tmr1_callback();  // Call the user-defined callback
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;  // Clear interrupt flag

    if (tmr2_callback != 0) {
        tmr2_callback();  // Call the user-defined callback
    }
}

void tmr1_set_callback(TimerCallback cb)
{
    tmr1_callback = cb;
}

void tmr2_set_callback(TimerCallback cb)
{
    tmr2_callback = cb;
}
