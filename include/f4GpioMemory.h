#ifndef F0GPIOMEMORY_H
#define F0GPIOMEMORY_H

#include <cstdint>
#include "IGpio.h"

typedef uint32_t volatile device_register;

class F4GpioMemory {
 public:
  F4GpioMemory() = default;
  ~F4GpioMemory() = default;
  GPIO_PinState_ readPin(GPIO_Pin pin);
  void write(GPIO_Pin pin, GPIO_PinState_ PinState);
  void toggle(GPIO_Pin pin);
  void init(GPIO_Pin pin, GPIO_Mode mode);

 protected:
  device_register MODER;
  device_register OTYPER;
  device_register OSPEEDR;
  device_register PUPDR;
  device_register IDR;
  device_register ODR;
  device_register BSRR;
  device_register LCKR;
  device_register AFR[2];

 private:
  static constexpr uint8_t NB_PINS = 16;
  void setOutputType(uint8_t position, GPIO_OUTPUT_TYPE type);
  void setOutputSpeed(uint8_t position, GPIO_OUTPUT_SPEED speed);
  void setPull(uint8_t position, GPIO_PUPD pupd);
};

#endif //F0GPIOMEMORY_H
