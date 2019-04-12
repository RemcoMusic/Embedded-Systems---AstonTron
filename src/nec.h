#ifndef NEC_H
#define NEC_H

#include "esp_err.h"

#include "driver/periph_ctrl.h"
#include "driver/rmt.h"
#include "driver/timer.h"
#include "freertos/queue.h"

class NEC{

  public:
    NEC();
    static NEC* getSingleton();
    void loop(int task_idx);
    void setCommandReceived(int);
    int getCommand();


  private:

    long rmtDelay = 0;

    esp_err_t initialize_ir();

    static NEC* singleton;

    RingbufHandle_t rb;
};

#endif