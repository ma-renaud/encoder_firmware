#ifndef F4GPIO_H
#define F4GPIO_H

#include "IGpio.h"
#include "f4GpioMemory.h"

class F4Gpio : public IGpio {
 public:
  explicit F4Gpio(uint32_t address) {
    gpio_memory = reinterpret_cast<F4GpioMemory *>(address);
  }
  ~F4Gpio() = default;

  GPIO_PinState_ readPin(GPIO_Pin pin) override { return gpio_memory->readPin(pin); }
  void writePin(GPIO_Pin pin, GPIO_PinState_ PinState) override { gpio_memory->write(pin, PinState); }
  void togglePin(GPIO_Pin pin) override { gpio_memory->toggle(pin); }

 private:
  F4GpioMemory *gpio_memory;
};

#endif //F4GPIO_H
