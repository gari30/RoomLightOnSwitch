#include <stdio.h>

#include "sw_light-on.h"
#include "pigpiod_if2.h"

GpioPi::GpioPi() {
  pi = pigpio_start(NULL, NULL);
}

GpioPi::~GpioPi() {
  pigpio_stop(pi);
}

int main(void) {

  printf("test\n");

  return 0;
}


