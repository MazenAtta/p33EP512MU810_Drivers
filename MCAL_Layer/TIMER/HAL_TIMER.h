/* 
 * File:   HAL_TIMER.h
 * Author: Mazen Atta
 *
 * Created on April 13, 2025, 3:13 PM
 */

#ifndef HAL_TIMER_H
#define	HAL_TIMER_H


#define TIMER1 1
#define TIMER2 2

typedef void (*TimerCallback)(void);

extern volatile TimerCallback tmr1_callback;
extern volatile TimerCallback tmr2_callback;

void tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);
void tmr_wait_ms(int timer, int ms);
void tmr1_set_callback(TimerCallback cb);
void tmr2_set_callback(TimerCallback cb);


#endif	/* HAL_TIMER_H */

