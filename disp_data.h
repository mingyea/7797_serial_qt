#ifndef __DISP_DATA_H
#define __DISP_DATA_H

#include "cfg.h"
#include "serial_drive.h"



class disp_data
{

    public:

    disp_data();
    ~disp_data();

    void disp_data_init(void);
    bool disp_data_tx_aera_write(serial_tx_s data);
    bool disp_data_rx_aera_write(serial_rx_s data);
	bool disp_data_tx_aera_read(serial_tx_s *p_data);
	bool disp_data_rx_aera_read(serial_rx_s *p_data);

    private:

        void init(void);

        void test(void);

};





#endif // __DISP_DATA_H
