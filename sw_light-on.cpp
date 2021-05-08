#include <cstdint>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "pigpiod_if2.h"

#include "sw_light-on.h"

#define TACTSW_PIN 7

void GpioEventNotify(int, unsigned int, unsigned int, unsigned int);
void TactSwPushEvent(uint8_t level);

static bool event_stop_flag = 0;

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
  set_noise_filter(pi_, TACTSW_PIN, kSamplingTime, kSamplingTime); // チャタキャンセル
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
      if (!event_stop_flag) {
        TactSwPushEvent(static_cast<uint8_t>(level));
        event_stop_flag = 1;
      }
      break;
    
    default:
      break;
  }
}

void TactSwPushEvent(uint8_t level) {
//  printf("TactSwitch push. level:%d\n", level);
  system("/home/garipi/homebot/bin/pi-send.sh 'room2lighton'");
}

int main(void) {
  GpioPi gpiopi;

  gpiopi.SetCallback();

  while(1) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (event_stop_flag) {
      std::this_thread::sleep_for(std::chrono::seconds(20));
      event_stop_flag = 0;
    }
  };

  return 0;
}


