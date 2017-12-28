#ifndef SERIAL_DRIVE_H
#define SERIAL_DRIVE_H

#include "settings.h"
#include "cfg.h"
#include <QTimer>
#include <QtWidgets>
#include <QQueue>

/*串行設備的狀態  發和收*/
typedef enum
{
    SERIAL_DRIVE_STATUS_IDLE=0,
    SERIAL_DRIVE_STATUS_TX,
    SERIAL_DRIVE_STATUS_RX,
    SERIAL_DRIVE_STATUS_BUSY
}serial_drive_status_e;

/*串行設備執行的結果--*/
typedef enum
{
    SERIAL_DRIVE_RESULT_INIT =0,
    SERIAL_DRIVE_RESULT_OPEN_ERROR,
    SERIAL_DRIVE_RESULT_CFG_ERROR,
    SERIAL_DRIVE_RESULT_OK,
    SERIAL_DRIVE_RESULT_RESOURCE_ERROR,
    SERIAL_DRIVE_RESULT_OTHER_ERROR,
}serial_drive_result_e;


/*tx 發送相關數據 主要是長度和內容 char數組格式*/
typedef struct
{
    qint64 len;
    char buf[SERIAL_TX_LEN];
}serial_tx_s;


/*rx--接收相關數據 主要是長度和內容 char數組格式*/
typedef struct
{
    qint64 len;
    char buf[SERIAL_RX_LEN];
}serial_rx_s;



class serial_drive : public QMainWindow
{
	Q_OBJECT
		
		public:
		
			explicit serial_drive(QWidget *parent = 0);
			~serial_drive();
			
			QQueue<serial_rx_s> queue_rx;

		private slots:       /*槽函数声明*/

			serial_drive_result_e slot_handle_error(QSerialPort::SerialPortError error);

			/*定時器相關的處理*/
			void slot_timer_op_rx_isr(void);
			void slot_timer_op_tx_isr(void);
			void slot_timer_op_tx_q_op_master(void);
			void slot_timer_op_rx_q_op(void);

			/*接收中斷*/
			void slot_op_rx_isr(void);
			

		private:

				QSerialPort device;

				serial_drive_status_e status;
				int time_count;
				int time_total;
				serial_rx_s rx_data;

				QQueue<serial_tx_s> queue_tx;	

				settings* gpri_setting;          /*所有設置相關的參數*/

				QTimer *gpri_timer_rx_isr;              /**/
				QTimer *gpri_timer_tx_isr;              /**/
				QTimer *gpri_timer_tx_q_op_master;		/*處理發送隊列的定時器--主發隊列*/
				QTimer *gpri_timer_rx_q_op;		/*處理發送隊列的定時器--主發隊列*/

				qint32 get_tx_time_total(qint64 len);

		public:


				void init(void);
				void refresh(void);
				bool set_parameter(QSerialPort *serial, set_serial_dev_t *set_data);
				serial_drive_result_e open(QString *p_error);
				void close(void);
				bool is_open(void);
				bool add_tx_queue(serial_tx_s data);
				bool add_rx_queue(serial_rx_s data);
				qint64 tx_start(serial_tx_s data);


				void test(void);


};




#if 0
serial_drive *get_p_serial_drive(void);
#endif






#endif // SERIAL_DRIVE_H
