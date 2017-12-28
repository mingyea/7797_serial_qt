#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "language.h"
#include "serial_app.h"

#include <QDebug>
#include <QImageReader>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QTimer>
#include <QTranslator>
#include <QLineEdit>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    gpri_ui(new Ui::MainWindow)
{
    gpri_ui->setupUi(this);

    init();

    test();
}


/****************************初始和復位等**********************************/
/*
 * 初始化
*/
void MainWindow::init(void)
{
	/*************下拉菜單中 選中語言項************************/
	/*觸發信號的配置*/
	connect(gpri_ui->action_chinese_s, SIGNAL(triggered()), this, SLOT(slot_language_sw_to_chs()));
	connect(gpri_ui->action_chinese_t, SIGNAL(triggered()), this, SLOT(slot_language_sw_to_cht()));
	connect(gpri_ui->action_english, SIGNAL(triggered()), this, SLOT(slot_language_sw_to_english()));

	/*下拉選項的激活*/
	gpri_ui->action_english->setCheckable(true);
	gpri_ui->action_chinese_s->setCheckable(true);
	gpri_ui->action_chinese_t->setCheckable(true);
	/*加載多國語言包----默認 繁體中文*/
	language_sw_to(language_cht);

	/*************左側 串行設備相關的參數配置************************/
	// gpri_setting = get_p_setting();
	gpri_setting = new  settings;
	gpri_setting->serial_dev_init();     /**/
	gpri_setting->serial_dev_read(&gpri_setting->serial_dev);

	fill_serial_ports_device();   /*找到串設備*/
	fill_serial_ports_para();      /*串行的參數設置*/

	update_serial_para();	

	gpri_ui->label_led->setPixmap(QPixmap(":/led_off")); /*初始是關閉串口狀態*/
	gpri_ui->push_button_open_close->setText(tr("關閉"));
	/*聲明和新建串口*/
	//gpri_serial_app = get_p_serial_drive();
	gpri_serial_app = new serial_app;
	//gpri_serial_app->init();
	/*錯誤機制的處理*/

	connect(gpri_ui->push_button_open_close, SIGNAL(clicked()), this, SLOT(slot_push_button_open_close()));

	/*************各定時器************************/
	gpri_timer_tx_aera = new QTimer(this);
	gpri_timer_rx_aera = new QTimer(this);

	connect( gpri_timer_tx_aera,SIGNAL(timeout()), this, SLOT(slot_timer_op_tx_aera_disp()) );
	connect( gpri_timer_rx_aera,SIGNAL(timeout()), this, SLOT(slot_timer_op_rx_aera_disp()) );

	gpri_timer_tx_aera->start(AUTO_TIMER_TX_AERA_Q_TIME);    /*每50ms*/
	gpri_timer_rx_aera->start(AUTO_TIMER_RX_AERA_Q_TIME);

    /***************各算法***************/
    gpri_arithmetic= new arithmetic() ;

    /*****************發送區相關********************/
	gpri_disp = new disp_data;
     connect(gpri_ui->pushButton_tx_enable, SIGNAL(released()), this, SLOT(slot_button_tx_start()));

}

/*
 * 退出
*/
MainWindow::~MainWindow(void)
{
	//serial_drive l_serial_drive;
	// 更新最终设置并保存
	//updateSettings();
	//doSettings(true, currentSettings);
	// 防止没有关闭串口
	gpri_serial_app->drive.close();
	//l_serial_drive.func_close();
	// 删除申请的对象
	//delete autoSendTimer;
	delete gpri_timer_tx_aera;
	delete gpri_timer_rx_aera;

	delete gpri_setting;
	delete gpri_serial_app;
	delete gpri_disp;
	delete gpri_arithmetic;
	delete gpri_ui;
}


/*
 * 事件更新
*/
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            gpri_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}


/*
 * 刷新
*/
void MainWindow::refresh(void)
{
   //gpri_ui->label->addItem(tr("item1"));
   //gpri_ui->listWidget->addItem(tr("item2"));
   //gpri_ui->listWidget->addItem(tr("item3"));
   //changeEvent(LanguageChange);
    #if 0
    updateSettings();

    if(gpri_serial_app->isOpen())
    {
        gpri_ui->pushButton_open_close->setText(tr("闂滈枆"));
    }
    else
    {
        gpri_ui->pushButton_open_close->setText(tr("闂滈枆"));
    }

    gpri_ui->pushButton_open_close->setText(tr("闂滈枆"));

    slot_push_button_open_close();
    #endif
}





/*****************串口設備*******************/
/*
 * 查找 串口設備
*/
void MainWindow::fill_serial_ports_device(void)
{
    gpri_ui->combobox_port->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        gpri_ui->combobox_port->addItem(list.first(), list);
    }
}

/*
 * 列表框的選擇
*/
static void comboBox_sel_disp(QComboBox *comboBox, QString text)
{
    int index = comboBox->findText(text); //use default exact match
    if(index >= 0)
         comboBox->setCurrentIndex(index);
}

/*
 * 串行設備的參數 列出
*/
void MainWindow::fill_serial_ports_para(void)
{
    // fill baud rate (is not the entire list of available values,
    // desired values??, add your independently)
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    //gpri_ui->combobox_baudrate->addItem(QStringLiteral("Custom"));
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    gpri_ui->combobox_baudrate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    //gpri_ui->baudRateBox->addItem(QStringLiteral("230400"), QSerialPort::Baud230400);
    //gpri_ui->baudRateBox->addItem(QStringLiteral("460800"), QSerialPort::Baud460800);

    // fill data bits
    gpri_ui->combobox_data_len->addItem(QStringLiteral("5"), QSerialPort::Data5);
    gpri_ui->combobox_data_len->addItem(QStringLiteral("6"), QSerialPort::Data6);
    gpri_ui->combobox_data_len->addItem(QStringLiteral("7"), QSerialPort::Data7);
    gpri_ui->combobox_data_len->addItem(QStringLiteral("8"), QSerialPort::Data8);
    //gpri_ui->combobox_data_len->addItem(QStringLiteral("9"), QSerialPort::Data9);

    // fill parity
    gpri_ui->combobox_parity->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    gpri_ui->combobox_parity->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    gpri_ui->combobox_parity->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    gpri_ui->combobox_parity->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    gpri_ui->combobox_parity->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    gpri_ui->combobox_stop_len->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    gpri_ui->combobox_stop_len->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    gpri_ui->combobox_stop_len->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    // fill flow control
    gpri_ui->combobox_flow_ctrl->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    gpri_ui->combobox_flow_ctrl->addItem(QStringLiteral("Hardware"), QSerialPort::HardwareControl);
    gpri_ui->combobox_flow_ctrl->addItem(QStringLiteral("Software"), QSerialPort::SoftwareControl);

    // 还原上次保存的参数
    comboBox_sel_disp(gpri_ui->combobox_baudrate,  gpri_setting->serial_dev.baud_rate_string);
    comboBox_sel_disp(gpri_ui->combobox_data_len,  gpri_setting->serial_dev.data_bits_string );
    comboBox_sel_disp(gpri_ui->combobox_stop_len,  gpri_setting->serial_dev.stop_bits_string );
    comboBox_sel_disp(gpri_ui->combobox_parity,    gpri_setting->serial_dev.parity_string );
    comboBox_sel_disp(gpri_ui->combobox_flow_ctrl, gpri_setting->serial_dev.flow_control_string);

#if 0

    gpri_ui->checkBox_tx_dtr->setChecked(currentSettings.isDtr);
    gpri_ui->checkBox_tx_rts->setChecked(currentSettings.isRts);
    gpri_ui->checkBox_tx_hex_disp->setChecked(currentSettings.isHexDisplayTx);
    gpri_ui->checkBox_tx_hex->setChecked(currentSettings.isHexSend);
    gpri_ui->checkBox_tx_new_line->setChecked(currentSettings.sendNewLineEnabled);
    gpri_ui->checkBox_tx_cycle->setChecked(currentSettings.isTimerSend);
    gpri_ui->lineEdit_tx_cycle_timer->setText(QString::number(currentSettings.timerLength));
    gpri_ui->lineEdit_tx->setText(currentSettings.sendCache);

    gpri_ui->checkBox_rx_hex_disp->setChecked(currentSettings.isHexDisplayRx);
    gpri_ui->checkBox_rx_newline_disp->setChecked(currentSettings.isRxNewLine);
    gpri_ui->checkBox_rx_resource->setChecked(currentSettings.isRxResourcr);

#endif

#if 0

    /* 启动后第一次更新 */
    slot_currentIndexChanged();

#endif

}



/*
 * 更新serial配置參數---即從disp界面讀取
*/
void MainWindow::update_serial_para(void)
{
	gpri_setting->serial_dev.name = gpri_ui->combobox_port->currentText();

	#if 0
	if(gpri_ui->combobox_baudrate->currentIndex() == 4)
	{
		gpri_setting->serial_dev.baud_rate  = gpri_ui->combobox_baudrate->currentText().toInt();
	}
	else
	{
		gpri_setting->serial_dev.baud_rate = static_cast<QSerialPort::BaudRate>(gpri_ui->combobox_baudrate->itemData(gpri_ui->combobox_baudrate->currentIndex()).toInt());
	}
	#else
	gpri_setting->serial_dev.baud_rate = static_cast<QSerialPort::BaudRate>(gpri_ui->combobox_baudrate->itemData(gpri_ui->combobox_baudrate->currentIndex()).toInt());
	#endif
	
	gpri_setting->serial_dev.baud_rate_string =QString::number( gpri_setting->serial_dev.baud_rate);

	gpri_setting->serial_dev.data_bits = static_cast<QSerialPort::DataBits>(gpri_ui->combobox_data_len->itemData(gpri_ui->combobox_data_len->currentIndex()).toInt());
	gpri_setting->serial_dev.data_bits_string = gpri_ui->combobox_data_len->currentText();

	gpri_setting->serial_dev.stop_bits = static_cast<QSerialPort::StopBits>( gpri_ui->combobox_stop_len->itemData(gpri_ui->combobox_stop_len->currentIndex()).toInt());
	gpri_setting->serial_dev.stop_bits_string = gpri_ui->combobox_stop_len->currentText();

	gpri_setting->serial_dev.parity = static_cast<QSerialPort::Parity>( gpri_ui->combobox_parity->itemData(gpri_ui->combobox_parity->currentIndex()).toInt());
	gpri_setting->serial_dev.parity_string = gpri_ui->combobox_parity->currentText();

	gpri_setting->serial_dev.flow_control = static_cast<QSerialPort::FlowControl>( gpri_ui->combobox_flow_ctrl->itemData(gpri_ui->combobox_flow_ctrl->currentIndex()).toInt());
	gpri_setting->serial_dev.flow_control_string = gpri_ui->combobox_flow_ctrl->currentText();

	gpri_setting->serial_dev_write(&gpri_setting->serial_dev);

}



/*
 * 打開或關閉 串行的 事件處理 切換
*/
void MainWindow::slot_push_button_open_close(void)
{
    serial_drive_result_e l_result;
    QString p_str; 
    if(gpri_serial_app->drive.is_open())
    {
        gpri_ui->label_led->setPixmap(QPixmap(":/led_off"));
        gpri_ui->push_button_open_close->setText(tr("關閉"));
       gpri_serial_app->drive.close();
    }
    else
    {
	 
		update_serial_para();	
        l_result = gpri_serial_app->drive.open(&p_str);
        if(l_result ==SERIAL_DRIVE_RESULT_OPEN_ERROR)
        {
            gpri_serial_app->drive.close();
            QMessageBox::critical(this, tr("Error"), p_str);
            gpri_ui->statusBar->showMessage(tr("Open error"));
        }
        else if(l_result ==SERIAL_DRIVE_RESULT_CFG_ERROR)
        {
            QMessageBox::critical(this, tr("Error"), p_str);
            gpri_ui->statusBar->showMessage(tr("Configure error"));
        }
        else if(l_result ==SERIAL_DRIVE_RESULT_OK)
        {
            gpri_ui->label_led->setPixmap(QPixmap(":/led_on"));
            gpri_ui->push_button_open_close->setText(tr("打開"));
        }
        else
        {

        }
    }

    //slot_currentIndexChanged();
}



/***********************各定時器*******************************/
/*
 * 定時循環周期處理發送隊列
*/
void MainWindow::slot_timer_op_tx_aera_disp(void)
{
    serial_tx_s l_tx;

    if(gpri_disp->disp_data_tx_aera_read(&l_tx) == true)
    {

        qDebug() <<"tx aera queue operate successful ";
	 disp_tx_aera(l_tx);
	
    }

}


/*
 * 定時循環周期處理接收隊列
*/
void MainWindow::slot_timer_op_rx_aera_disp(void)
{

    serial_rx_s l_rx;

    if(gpri_disp->disp_data_rx_aera_read(&l_rx) == true)
    {

        qDebug() <<"rx aera queue operate successful ";
    }

}



/***********************發送區*******************************/
/*
*	發送使能
*/
void MainWindow::slot_button_tx_start(void)
{
	QString inputStr = gpri_ui->lineEdit_tx->text();

	#ifdef KEY_DOWN_TX_START			
	serial_tx_s l_tx;
	int i;

	l_tx.len = 150;
	for(i=0;i<l_tx.len;i++)
	{
	l_tx.buf[i] = 0x11 + i;
	}

	gpri_serial_app->add_tx_queue(l_tx);


	l_tx.len = 150;
	for(i=0;i<l_tx.len;i++)
	{
	l_tx.buf[i] = 0xa1 + i;
	}

	gpri_serial_app->add_tx_queue(l_tx);
	#endif

}


/*
	顯示 發送區 
*/
void MainWindow::disp_tx_aera(serial_tx_s tx_data)
{
	QString l_str_temp;
	QString l_str_all = gpri_ui->textBrowser_tx->toPlainText();;

	gpri_arithmetic->char_array_to_string(&l_str_temp,tx_data.len,tx_data.buf);

	l_str_all += l_str_temp;

	gpri_ui->textBrowser_tx->setText(l_str_all);

	qDebug() <<" now disp tx aera and add data ";

}





/***********************語言*******************************/

/*
 * 語言選擇和切換
*/
void MainWindow::language_sw_to(language_type_e sel_lan)
{

    language l_language;
    l_language.switch_to_language(sel_lan);

    //this->changeEvent(LanguageChange);

    switch(sel_lan)
    {
         case language_english:
            gpri_ui->action_english->setChecked(true);
            gpri_ui->action_chinese_s->setChecked(false);
            gpri_ui->action_chinese_t->setChecked(false);
            break;
        case language_chs:
           gpri_ui->action_english->setChecked(false);
           gpri_ui->action_chinese_s->setChecked(true);
           gpri_ui->action_chinese_t->setChecked(false);
           break;
        case language_cht:
           gpri_ui->action_english->setChecked(false);
           gpri_ui->action_chinese_s->setChecked(false);
           gpri_ui->action_chinese_t->setChecked(true);
           break;
           /*默認是 繁中*/
        default:
            gpri_ui->action_english->setChecked(false);
            gpri_ui->action_chinese_s->setChecked(false);
            gpri_ui->action_chinese_t->setChecked(true);
            break;

    }

    this->refresh();

}


/*
 * 槽函數，切換到英文
*/
void MainWindow::slot_language_sw_to_english(void)
{

    language_sw_to(language_english);


}


/*
 * 槽函數，切換到簡體中文
*/
void MainWindow::slot_language_sw_to_chs(void)
{

    language_sw_to(language_chs);


}

/*
 * 槽函數，切換到繁體中文
*/
void MainWindow::slot_language_sw_to_cht(void)
{

    language_sw_to(language_cht);


}


















void MainWindow::test(void)
{
	#if 0
    QChar *b = new QChar[3];
    QString str("AB");
    b = str.data();

    QChar C[2];
    C[0] = *b;
    C[1] = *(b+1);
	#endif

    int i;
    serial_tx_s l_tx;
    serial_rx_s l_rx;

    l_tx.len =30;
    for(i=0;i<l_tx.len;i++)
    {
        l_tx.buf[i] = 0x30+i;
    }

    gpri_disp->disp_data_tx_aera_write(l_tx);

    l_tx.len =30;
    for(i=0;i<l_tx.len;i++)
    {
        l_tx.buf[i] = 0x40+i;
    }

    gpri_disp->disp_data_tx_aera_write(l_tx);

    l_tx.len =30;
    for(i=0;i<l_tx.len;i++)
    {
        l_tx.buf[i] = 0x50+i;
    }

    gpri_disp->disp_data_tx_aera_write(l_tx);


    l_rx.len =30;
    for(i=0;i<l_rx.len;i++)
    {
        l_rx.buf[i] = 0x30+i;
    }

    gpri_disp->disp_data_rx_aera_write(l_rx);

    l_rx.len =30;
    for(i=0;i<l_rx.len;i++)
    {
        l_rx.buf[i] = 0x40+i;
    }

    gpri_disp->disp_data_rx_aera_write(l_rx);

    l_rx.len =30;
    for(i=0;i<l_rx.len;i++)
    {
        l_rx.buf[i] = 0x50+i;
    }

    gpri_disp->disp_data_rx_aera_write(l_rx);




	

}



