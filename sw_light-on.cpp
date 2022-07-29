#include <cstdint>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "pigpiod_if2.h"

#include "sw_light-on.h"

#define TACTSW_PIN 7

void GpioEventNotify(int, unsigned int, unsigned int, unsigned int);
void TactSwPushEvent(void);

static uint8_t event_count_pin7 = 0; // TACTSW_PIN 7が押されたカウント

GpioPi::GpioPi() {
  pi_ = pigpio_start(NULL, NULL);
  TackSwitchPinInit();
}

GpioPi::~GpioPi() {
  pigpio_stop(pi_);
}

void GpioPi::TackSwitchPinInit() {
  set_mode(pi_, TACTSW_PIN, PI_INPUT); // GPIOをinputに設定
  set_pull_up_down(pi_, TACTSW_PIN, PI_PUD_UP); // GPIOをプルアップに設定
  set_noise_filter(pi_, TACTSW_PIN, kSamplingTime, 1); // チャタキャンセル
}

void GpioPi::SetCallback() {
  callback(pi_, TACTSW_PIN, FALLING_EDGE, &GpioEventNotify); // コールバックの登録
}

/* param
 *  pi    : pigpio no.(no use)
 *  gpio  : 0-31
 *  level : 0:change to low
 *          1:change to high
 *          2:no level change(a watchdog timeout)
 *  tick  : timestamp
 */
void GpioEventNotify(int pi, unsigned int gpio, unsigned int level, unsigned int tick) {
  switch(gpio) {
    case TACTSW_PIN:
      ++event_count_pin7;
//      printf("[%u]count increment: %d\n",level ,event_count_pin7);
      break;
    
    default:
      break;
  }
}

void TactSwPushEvent(void) {
  system("/home/garipi/homebot/bin/pi-send.sh 'pcroomlighton'");
}

int main(void) {
  GpioPi gpiopi;

  gpiopi.SetCallback();

  while(1) {
    if (event_count_pin7 != 0) {
      TactSwPushEvent();
      --event_count_pin7;
//      printf("decrement count: %d\n", event_count_pin7);
    }
    else {
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  };

  return 0;
}


