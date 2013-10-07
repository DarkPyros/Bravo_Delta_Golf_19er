#ifndef TIMER_H
#define	TIMER_H

/* Delays based on 25 MHz timer clock */
/* 29us delay */
#define TIMER_DELAY_MODE_SELECT 	725
/* 12.4us delay */
#define TIMER_DELAY_SCH_UPDATE 		310

#define TIMER_DELAY_ENCODE_DATA 	10

#define TIMER_DELAY_DECODE_DATA 	10

#define TIMER_DELAY_READ_CODEC 		10

#define TIMER_DELAY_WRITE_CODEC 	10

#define TIMER_DELAY_CHANGE_SAMPLING 10	
/* 90us timeout */
#define TIMER_TIMEOUT_SPI 			2250

void TIMER_timer1Start(int);
void TIMER_timer1Stop();

#endif
