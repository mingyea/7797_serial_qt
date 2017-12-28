#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
*/
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include "language.h"
#include "settings.h"
#include "serial_app.h"
#include "arithmetic.h"
#include "disp_data.h"
#include "cfg.h"


namespace Ui {
        class MainWindow;
    }

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow(void);

    /*槽函數*/
    public slots:       //槽函数声明

    private slots:       //槽函数声明

        /*各語言選擇*/
        void slot_language_sw_to_english(void);
        void slot_language_sw_to_chs(void);
        void slot_language_sw_to_cht(void);

        /*按鍵觸發事件處理-*設置區*/
        void slot_push_button_open_close(void); /*--打開和關閉串行--*/

        /*按鍵觸發事件處理-發送區*/
        void slot_button_tx_start(void);        /*--發送使能--*/

        /*定時器相關的處理--*/
        void slot_timer_op_tx_aera_disp(void);
        void slot_timer_op_rx_aera_disp(void);


    protected:
        void changeEvent(QEvent *e);        /*切記--該名字不能改，是庫函數，改了之後會引起異常，如語言不能切*/

    private:

        Ui::MainWindow *gpri_ui;        /*主ui*/
        serial_app *gpri_serial_app;       /*串行--設備和參數*/
        settings* gpri_setting;          /*所有設置相關的參數*/
		disp_data* gpri_disp;			/*顯示區域*/

        QTimer *gpri_timer_tx_aera;		/*處理發送隊列的定時器--主發隊列*/
        QTimer *gpri_timer_rx_aera;     /*處理接收隊列的定時器*/

        arithmetic* gpri_arithmetic;    /*算法相關*/

        void init(void);           /*初始化*/
        void refresh(void);        /*刷新*/

        void language_sw_to(language_type_e sel_lan);      /*語言選擇*/

        bool open_serial_port(void);       /*打開串行*/
        void close_serial_port(void);      /*關閉串行*/
        void fill_serial_ports_device(void);       /*查找所有串行設備--且加入到列表*/
        void fill_serial_ports_para(void);         /*查找所有串行參數--且加入到列表*/
        void update_serial_para(void);             /*更新serial配置參數---即從disp界面讀取*/

	void disp_tx_aera(serial_tx_s tx_data);


        void test(void);                /*設置*/

};

#endif // MAINWINDOW_H
