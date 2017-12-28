#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtSerialPort/QSerialPort>
#include "cfg.h"

/*串口設備相關*/
typedef struct
{
    /*串口號*/
    QString name;
    /*波持率*/
    qint32 baud_rate;
    QString baud_rate_string;
    /*數據位*/
    QSerialPort::DataBits data_bits;
    QString data_bits_string;
    /*校驗位*/
    QSerialPort::Parity parity;
    QString parity_string;
    /*停止位*/
    QSerialPort::StopBits stop_bits;
    QString stop_bits_string;
    /*流控*/
    QSerialPort::FlowControl flow_control;
    QString flow_control_string;

}set_serial_dev_t;

/*串口發送的設置*/
typedef struct
{
    bool ready_aera_hex_disp;    /*待發區--十六進制顯示*/
    bool sended_aera_hex_disp;   /*己發送區的十六進制顯示*/
    bool end_add_newline_flag;  /*結尾添加新行標志*/
    bool cycle_send;        /*循環發送*/
    bool rts;               /*數據流循環接收*/
    bool dtr;               /*數據流循環發送*/
    qint32 cycle_time;      /*循環發送時的間隔時間--單位ms/次*/
}set_serial_tx_t;



/*串口接收的設置*/
typedef struct
{
    bool recving_hex_disp;      /*接收區十六進制顯示*/
    bool auto_newline;          /*每幀自動換行*/
    bool pause_disp;            /*暫停顯示*/
    bool recv_to_file;          /*接收轉向文件*/
}set_serial_rx_t;



class settings
{
    public:

        settings();

        set_serial_dev_t serial_dev;

        void serial_dev_init(void);
        void serial_dev_write(set_serial_dev_t *p_data);
        void serial_dev_read(set_serial_dev_t *p_data);

    private:

        void init(void);
        void test(void);

};





#endif // SETTINGS_H
