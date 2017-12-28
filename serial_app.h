#ifndef SERIAL_APP_H
#define SERIAL_APP_H

#include "cfg.h"
#include "settings.h"
#include "serial_drive.h"





class serial_app : public QMainWindow
{
	Q_OBJECT
		
		public:
		
			explicit serial_app(QWidget *parent = 0);
			~serial_app();

			serial_drive drive;
				
		private slots:       /*槽函数声明*/

			/*定時器相關的處理*/
			bool slot_timer_op_rx_q_op(void);

		private:

				QQueue<serial_tx_s> queue_tx;	
				QQueue<serial_rx_s> queue_rx;

				QTimer *gpri_timer_rx_q_op;		/*處理發送隊列的定時器--主發隊列*/

		public:

				void init(void);
				void refresh(void);
				
				bool add_tx_queue(serial_tx_s data);


				void test(void);


};







#endif // SERIAL_APP_H
