#include "serial_app.h"
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>




/*
* 
*/
serial_app::serial_app(QWidget *parent): QMainWindow(parent)
{
	init();
	test();
}



/*
* 析構
*/
serial_app::~serial_app()
{
	delete gpri_timer_rx_q_op;

}


/*
* 初始化
*/
void serial_app::test(void)
{

#ifdef SERIAL_APP_TX_ADD

	serial_tx_s l_tx;
	serial_rx_s l_rx;
	int i;

	l_tx.len = 150;
	for(i=0;i<l_tx.len;i++)
	{
	l_tx.buf[i] = 0x12 + i;
	}
	add_tx_queue(l_tx);
	add_tx_queue(l_tx);
	add_tx_queue(l_tx);
	add_tx_queue(l_tx);

#endif


}



/*
* 初始化
*/
void serial_app::init(void)
{

	gpri_timer_rx_q_op = new QTimer(this);
	
	connect( gpri_timer_rx_q_op,SIGNAL(timeout()), this, SLOT(slot_timer_op_rx_q_op()) );

	gpri_timer_rx_q_op->start(AUTO_TIMER_SERIAL_APP_RX_Q_TIME);    /*每ms*/


}


/*
* 刷新
*/
void serial_app::refresh(void)
{
	/**/


}


/*
*  往發送隊列里 添加數據
*/
bool serial_app::add_tx_queue(serial_tx_s data)
{
	drive.add_tx_queue(data);
	return true;
}



/*
 * 定時循環周期處理接收隊列
*/
bool serial_app::slot_timer_op_rx_q_op(void)
{
	bool l_result = false;
	
	serial_rx_s l_rx;
	QString l_size_str;
	QString l_first_byte_str;

	if( drive.queue_rx.isEmpty() == false )
	{
		l_rx =  drive.queue_rx.dequeue();
		l_size_str = tr("%1").arg(l_rx.len);
		l_first_byte_str = tr("%1").arg(l_rx.buf[0]);
		qDebug() << "serial app rx op success" << l_size_str << "byte,and first byte is " << l_first_byte_str;

		l_result = true;		 
	 }
	else
	{
		l_result = false;		 
	}

	return l_result;

}






