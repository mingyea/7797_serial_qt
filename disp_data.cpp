#include "disp_data.h"


/*發送顯示區域的隊列*/
QQueue<serial_tx_s> queue_disp_tx;	
/*接收顯示區域的隊列*/
QQueue<serial_rx_s> queue_disp_rx;





disp_data::disp_data()
{
    init();
    test();
}


disp_data::~disp_data()
{
	
}


/*
 * 初始化
*/
void disp_data::test()
{


}

/*
 * 初始化
*/
void disp_data::init()
{


}



/*
 * 串行
*/
void disp_data::disp_data_init(void)
{

}


/*
 * 往發送顯示區域添加數據
*/
bool disp_data::disp_data_tx_aera_write(serial_tx_s data)
{

	queue_disp_tx.enqueue(data);
    return true;
}


/*
 * 往接收顯示區域添加數據
*/
bool disp_data::disp_data_rx_aera_write(serial_rx_s data)
{
	queue_disp_rx.enqueue(data);
    return true;
}



/* 讀取隊列（針對發送區）
 * 
*/
bool disp_data::disp_data_tx_aera_read(serial_tx_s *p_data)
{
	bool l_result = false;

	if( queue_disp_tx.isEmpty()  ==  false)
	{
		*p_data = queue_disp_tx.dequeue();
		
        qDebug() << "disp tx " <<  "bytes " ;

		l_result = true;

	}
	else
	{
		l_result = false;
	}

	return l_result;

}




/* 讀取隊列（針對接收區）
 *
*/
bool disp_data::disp_data_rx_aera_read(serial_rx_s *p_data)
{
	bool l_result = false;

	if( queue_disp_rx.isEmpty()  ==  false)
	{
		*p_data = queue_disp_rx.dequeue();

        qDebug() << "disp rx "  << "bytes " ;

		l_result = true;

	}
	else
	{
		l_result = false;
	}

	return l_result;

}




