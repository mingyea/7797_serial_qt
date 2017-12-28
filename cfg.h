#ifndef CFG_H
#define CFG_H


/*timer*/

#define AUTO_TIMER_TX_AERA_Q_TIME   10        /*單位ms*/
#define AUTO_TIMER_RX_AERA_Q_TIME    10              /*ms*/


/*for serial */
#define AUTO_TIMER_SERIAL_TX_ISR_TIME   10        /*單位ms*/
#define AUTO_TIMER_SERIAL_RX_ISR_TIME   30        /*單位ms*/
#define AUTO_TIMER_SERIAL_TX_Q_MASTER_TIME   50        /*單位ms*/
#define AUTO_TIMER_SERIAL_RX_Q_TIME    10              /*ms*/



#define AUTO_TIMER_SERIAL_APP_RX_Q_TIME    10              /*ms*/




/*serial 1*/
#define SERIAL_TX_LEN   200		/**/
#define SERIAL_RX_LEN   200		/**/








//#define SERIAL_DRIVE_TX_ADD  1
//#define SERIAL_APP_TX_ADD  1
//#define KEY_DOWN_TX_START  1

#endif // CFG_H
