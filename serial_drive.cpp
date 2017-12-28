#include "serial_drive.h"
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>

#if 0
serial_drive g_serial_drive;       /*串行--設備和參數*/
#endif

serial_drive::serial_drive(QWidget *parent): QMainWindow(parent)
{
	init();
	test();
}



/*
*  析構函數
*/
serial_drive::~serial_drive()
{
	delete gpri_setting;
	delete gpri_timer_rx_isr;
	delete gpri_timer_tx_isr;
	delete gpri_timer_tx_q_op_master;
	delete gpri_timer_rx_q_op;
}

/*
*  初始化
*/
void serial_drive::test(void)
{

#ifdef SERIAL_DRIVE_TX_ADD

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

	l_rx.len = 150;
	for(i=0;i<l_rx.len;i++)
	{
	l_rx.buf[i] = 0xa2 + i;
	}
	add_rx_queue(l_rx);
	add_rx_queue(l_rx);
	add_rx_queue(l_rx);
	add_rx_queue(l_rx);

#endif


}


/*
*  初始化
*/
void serial_drive::init(void)
{
	gpri_setting = new settings;	

	gpri_timer_rx_isr = new QTimer(this);
	gpri_timer_tx_isr = new QTimer(this);
	gpri_timer_tx_q_op_master = new QTimer(this);
	gpri_timer_rx_q_op = new QTimer(this);

	gpri_timer_rx_isr->setSingleShot(true);

	
	connect( gpri_timer_rx_isr,SIGNAL(timeout()), this, SLOT(slot_timer_op_rx_isr()) );
	connect( gpri_timer_tx_isr,SIGNAL(timeout()), this, SLOT(slot_timer_op_tx_isr()) );
	connect( gpri_timer_tx_q_op_master,SIGNAL(timeout()), this, SLOT(slot_timer_op_tx_q_op_master()) );
	connect( gpri_timer_rx_q_op,SIGNAL(timeout()), this, SLOT(slot_timer_op_rx_q_op()) );
	connect(&device, SIGNAL(readyRead()), this, SLOT(slot_op_rx_isr()) );

	/**/
	connect(&device, SIGNAL(error(QSerialPort::SerialPortError)), this,SLOT(slot_handle_error(QSerialPort::SerialPortError)));


	gpri_timer_tx_isr->start(AUTO_TIMER_SERIAL_TX_ISR_TIME);    /*每ms*/
	gpri_timer_tx_q_op_master->start(AUTO_TIMER_SERIAL_TX_Q_MASTER_TIME);    /*每ms*/
	gpri_timer_rx_q_op->start(AUTO_TIMER_SERIAL_RX_Q_TIME);    /*每ms*/

	/*收發狀態和超時*/
	status = SERIAL_DRIVE_STATUS_IDLE;
	time_count = 0;


}


/*
*  刷新
*/
void serial_drive::refresh(void)
{
	/**/


}


/*
 * 串口設備 真正配置參數;
*/
bool serial_drive::set_parameter(QSerialPort *serial, set_serial_dev_t *set_data)
{
    bool l_result;

    if (serial->setBaudRate(set_data->baud_rate)
            && serial->setDataBits(set_data->data_bits)
            && serial->setParity(set_data->parity)
            && serial->setStopBits(set_data->stop_bits)
            && serial->setFlowControl(set_data->flow_control))
    {
        l_result = true;
    }
    else
        l_result = false;

    return l_result;
}


/*
 * 串口設備  判斷是開啟
*/
bool serial_drive::is_open(void)
{
    bool l_result;

    if(device.isOpen())
    {
        l_result = true;
    }
    else
    {
        l_result = false;
    }
    return l_result;
}


/*
 * 串口設備  配置并開啟串口
*/
serial_drive_result_e serial_drive::open(QString *p_error)
{
    serial_drive_result_e l_result;

    gpri_setting->serial_dev_read(&gpri_setting->serial_dev);
	
    device.setPortName(gpri_setting->serial_dev.name);
    if (device.open(QIODevice::ReadWrite))
    {
        if (set_parameter(&device, &gpri_setting->serial_dev))
        {
            l_result = SERIAL_DRIVE_RESULT_OK;
        }
        else
        {
            close();
            //QMessageBox::critical( QObject::tr("Error"), this->device.errorString());
            *p_error = device.errorString();
            l_result = SERIAL_DRIVE_RESULT_OPEN_ERROR;
        }
    }
    else
    {
        //QMessageBox::critical(this, QObject::tr("Error"), this->device.errorString());
        *p_error = device.errorString();
        l_result = SERIAL_DRIVE_RESULT_CFG_ERROR;
    }

    return l_result;

}


/*
 * 串口設備  關閉串口
*/
void serial_drive::close(void)
{
    device.close();
}




/*
 * 添加除错功能，解决强制拔出时程序崩溃。--其中有個msg未發到主界面
*/
serial_drive_result_e serial_drive::slot_handle_error(QSerialPort::SerialPortError error)
{
    serial_drive_result_e l_result = SERIAL_DRIVE_RESULT_INIT;
    QString errStr = "NO ERROR";
    switch(error)
    {
        case QSerialPort::DeviceNotFoundError:
            errStr = "DeviceNotFoundError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::PermissionError:
            errStr = "PermissionError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::OpenError:
            errStr = "OpenError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::ParityError:
            errStr = "ParityError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::FramingError:
            errStr = "FramingError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::BreakConditionError:
            errStr = "BreakConditionError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::WriteError:
            errStr = "WriteError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::ReadError:
            errStr = "ReadError";
            close();
            break;
        case QSerialPort::ResourceError:
            errStr = "ResourceError";
            qDebug() << errStr;
            /*原來代碼的接口是調用下面語句 */
            //slot_push_button_open_close();
            close();
            break;
        case QSerialPort::UnsupportedOperationError:
            errStr = "UnsupportedOperationError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::UnknownError:
            // do nothing!!!
            errStr = "UnknownError";
            qDebug() << errStr;
    //        slot_push_button_open_close();
            break;
        case QSerialPort::TimeoutError:
            errStr = "TimeoutError";
            qDebug() << errStr;
            close();
            break;
        case QSerialPort::NotOpenError:
            // do nothing.
            errStr = "NotOpenError";
            qDebug() << errStr;
    //                             close();
            break;
        default:
            qDebug() << errStr;
            break;
    }
    if (error == QSerialPort::ResourceError)
    {
        l_result = SERIAL_DRIVE_RESULT_RESOURCE_ERROR;
        //QMessageBox::critical(this, tr("Critical Error"),this->device.errorString());
        //                     close();
    }
    return l_result;
}


/*
 *  添加數據到發送隊列
*/
bool serial_drive::add_tx_queue(serial_tx_s data)
{
	queue_tx.enqueue(data);
	return true;
}


/*
 *  添加數據到接收隊列
*/
bool serial_drive::add_rx_queue(serial_rx_s data)
{
	queue_rx.enqueue(data);
	return true;

}


/*
 *接收中斷
*/
void serial_drive::slot_op_rx_isr(void)
{

	if(status == SERIAL_DRIVE_STATUS_IDLE)
	{
		status = SERIAL_DRIVE_STATUS_RX;
		rx_data.len  =  device.read(rx_data.buf,SERIAL_RX_LEN);
		gpri_timer_rx_isr->setSingleShot(true);
		gpri_timer_rx_isr->start(AUTO_TIMER_SERIAL_RX_ISR_TIME);

	}
	else if(status == SERIAL_DRIVE_STATUS_RX)
	{
		rx_data.len  +=  device.read(&rx_data.buf[rx_data.len],SERIAL_RX_LEN);
		gpri_timer_rx_isr->setSingleShot(true);
		gpri_timer_rx_isr->start(AUTO_TIMER_SERIAL_RX_ISR_TIME);
	}
	else
	{
		
	}

}



/*
 * 定時循環周期處理  和發送中斷相關
*/
void serial_drive::slot_timer_op_rx_isr(void)
{

	#if 0
	QString l_len_str;
	QString l_first_byte_str;
	#endif

	if(status==SERIAL_DRIVE_STATUS_RX)
	{

		status = SERIAL_DRIVE_STATUS_IDLE;
		
		#if 0
		l_len_str = tr("%1").arg(rx_data.len );
		l_first_byte_str = tr("%1").arg(rx_data.buf[0]);
		qDebug() << "rx success" << l_len_str << "byte,and first byte is " << l_first_byte_str;
		#endif

		queue_rx.enqueue(rx_data);

	}

	gpri_timer_rx_isr->stop();

}



/*
 * 定時循環周期處理  和發送中斷相關
*/
void serial_drive::slot_timer_op_tx_isr(void)
{

    if(status==SERIAL_DRIVE_STATUS_TX)
    {
        time_count++;
        if(time_count >= time_total)
        {
            time_count = 0;
            time_total = 1;

            status = SERIAL_DRIVE_STATUS_IDLE;
        }

    }



}

/*
 * 定時循環周期處理發送隊列
*/
void serial_drive::slot_timer_op_tx_q_op_master(void)
{
	int l_size;
	serial_tx_s l_tx;
	QString l_size_str;

	//qDebug() <<"serial module tx queue operate successful ";
	if( queue_tx.isEmpty()  ==  false)
	{
		l_size = queue_tx.size();
		//if(status == SERIAL_DRIVE_STATUS_IDLE)
		{
			//l_tx = queue_tx.dequeue();
			l_tx =  queue_tx.head();
			l_size_str = tr("%1").arg(l_size);
			qDebug() << l_size_str;
			//qDebug() << l_tx;

			if( tx_start(l_tx) != 0)
			{
				l_tx = queue_tx.dequeue();
			}

		}

	}

}



/*
 * 定時循環周期處理發送隊列
*/
void serial_drive::slot_timer_op_rx_q_op(void)
{
	#if 0
	//int l_size;
	serial_rx_s l_rx;
	QString l_size_str;
	QString l_first_byte_str;

   	//qDebug() <<"serial module rx queue operate successful ";
	if( queue_rx.isEmpty() == false )
	{
		 //l_size = queue_rx.size();
		 l_rx = queue_rx.dequeue();
		 l_size_str = tr("%1").arg(l_rx.len);
		l_first_byte_str = tr("%1").arg(l_rx.buf[0]);
		qDebug() << "rx queue success" << l_size_str << "byte,and first byte is " << l_first_byte_str;
		 //qDebug() << l_rx;
	 }

	/*
	int intValue;
	QString strValue;
	strValue = tr("%1").arg(intValue);
	*/
	#endif

}


/*
 *發送使能
*/
 qint64 serial_drive::tx_start(serial_tx_s data)
 {
    qint64 l_len ;
    QString l_len_str;

    if(device.isOpen())
    {
      if(status == SERIAL_DRIVE_STATUS_IDLE)
      {
          l_len = device.write(&data.buf[0],(qint64)data.len);
          l_len_str = tr("%1").arg(l_len);
          time_count = 0;
          //time_total = 20;
          time_total = get_tx_time_total(data.len);
          status = SERIAL_DRIVE_STATUS_TX;
          qDebug() << "真正發出" << l_len_str;

      }
      else
      {
          l_len = 0;
      }
    }
    else
    {
        l_len = 0;
    }

    return l_len;
 }


 /*
  * 計算發送的總時間；  其實是局部函數；
*/
qint32 serial_drive::get_tx_time_total(qint64 len)
{
    qint32 l_result;

    l_result = 1000000 / gpri_setting->serial_dev.baud_rate;
    l_result *= 9;      /*9bit include stop bit */
    l_result *= len  ;
    l_result /= 1000;
    l_result /= AUTO_TIMER_SERIAL_TX_ISR_TIME;
    l_result *= 1.5;

    return l_result;
}





#if 0
serial_drive *get_p_serial_drive(void)
{

    return &g_serial_drive;
}
#endif
