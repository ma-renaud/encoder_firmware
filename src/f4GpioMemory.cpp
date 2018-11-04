#include "f4GpioMemory.h"

GPIO_PinState_ F4GpioMemory::readPin(GPIO_Pin pin) {
  if ((IDR & static_cast<uint8_t>(pin)) != static_cast<uint8_t>(GPIO_PinState_::GPIO_PIN_RESET)) {
    return GPIO_PinState_::GPIO_PIN_SET;
  } else {
    return GPIO_PinState_::GPIO_PIN_RESET;
  }
}

void F4GpioMemory::write(GPIO_Pin pin, GPIO_PinState_ PinState) {
  if (PinState != GPIO_PinState_::GPIO_PIN_RESET)
    BSRR = static_cast<uint32_t>(pin);
  else
    BSRR = static_cast<uint32_t>(pin) << 16;
}

void F4GpioMemory::toggle(GPIO_Pin pin) {
  ODR ^= static_cast<uint16_t>(pin);
}