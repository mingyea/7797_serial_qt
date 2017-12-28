#include "settings.h"



static set_serial_dev_t  g_set_sertial_dev;




settings::settings()
{
    //init();
    test();
}


/*
 * 測試
*/
void settings::test()
{

}


#if 0
/*
 * 初始化
*/
void settings::init()
{
    serial.name = "null";
    serial.baud_rate = QSerialPort::BaudRate::Baud19200;
    serial.baud_rate_string  = QString::number(QSerialPort::Baud19200);
    serial.data_bits = QSerialPort::DataBits::Data8;
    serial.data_bits_string = QString::number(QSerialPort::DataBits::Data8);
    serial.parity = QSerialPort::Parity::NoParity;
    serial.parity_string = QString::number(QSerialPort::Parity::NoParity);
    serial.stop_bits = QSerialPort::StopBits::OneStop;
    serial.stop_bits_string  = QString::number(QSerialPort::StopBits::OneStop);
    serial.flow_control = QSerialPort::FlowControl::NoFlowControl;
    serial.flow_control_string = QString::number(QSerialPort::FlowControl::NoFlowControl);

    tx.cycle_send = false;
    tx.cycle_time = 0;
    tx.dtr = false;
    tx.end_add_newline_flag=false;
    tx.ready_aera_hex_disp=true;
    tx.rts = false;
    tx.sended_aera_hex_disp = true;

    rx.auto_newline =true;
    rx.pause_disp=false;
    rx.recving_hex_disp=true;
    rx.recv_to_file=false;

}
#endif



/*
 * 串行
*/
void settings::serial_dev_init(void)
{
	#if 1
	set_serial_dev_t*p_data;
	p_data = &g_set_sertial_dev;

	p_data->name = "null";
	p_data->baud_rate = QSerialPort::BaudRate::Baud19200;
	p_data->baud_rate_string  = QString::number(QSerialPort::Baud19200);
	p_data->data_bits = QSerialPort::DataBits::Data8;
	p_data->data_bits_string = QString::number(QSerialPort::DataBits::Data8);
	p_data->parity = QSerialPort::Parity::NoParity;
	p_data->parity_string = QString::number(QSerialPort::Parity::NoParity);
	p_data->stop_bits = QSerialPort::StopBits::OneStop;
	p_data->stop_bits_string  = QString::number(QSerialPort::StopBits::OneStop);
	p_data->flow_control = QSerialPort::FlowControl::NoFlowControl;
	p_data->flow_control_string = QString::number(QSerialPort::FlowControl::NoFlowControl);
	#endif
}


/*
 * 串行設置的寫入
*/
void settings::serial_dev_write(set_serial_dev_t *p_data)
{
    //memcpy(&g_set_sertial_dev,p_data,sizeof(set_serial_dev_t));

    g_set_sertial_dev.name = p_data->name;
    g_set_sertial_dev.baud_rate = p_data->baud_rate;
    g_set_sertial_dev.baud_rate_string = p_data->baud_rate_string;
    g_set_sertial_dev.data_bits = p_data->data_bits;
    g_set_sertial_dev.data_bits_string = p_data->data_bits_string;
    g_set_sertial_dev.parity = p_data->parity;
    g_set_sertial_dev.parity_string = p_data->parity_string;
    g_set_sertial_dev.stop_bits = p_data->stop_bits;
    g_set_sertial_dev.stop_bits_string = p_data->stop_bits_string;
    g_set_sertial_dev.flow_control = p_data->flow_control;
    g_set_sertial_dev.flow_control_string = p_data->flow_control_string;

	
}


/*
 * 串行設置的讀取
*/
void settings::serial_dev_read(set_serial_dev_t *p_data)
{
     //memcpy(p_data,&g_set_sertial_dev,sizeof(set_serial_dev_t));

     p_data->name = g_set_sertial_dev.name;
     p_data->baud_rate =g_set_sertial_dev.baud_rate;
     p_data->baud_rate_string = g_set_sertial_dev.baud_rate_string;
     p_data->data_bits = g_set_sertial_dev.data_bits;
     p_data->data_bits_string = g_set_sertial_dev.data_bits_string;
     p_data->parity = g_set_sertial_dev.parity;
     p_data->parity_string = g_set_sertial_dev.parity_string;
     p_data->stop_bits = g_set_sertial_dev.stop_bits;
     p_data->stop_bits_string = g_set_sertial_dev.stop_bits_string;
     p_data->flow_control = g_set_sertial_dev.flow_control;
     p_data->flow_control_string = g_set_sertial_dev.flow_control_string;	 
}


