
#include <f4GpioMemory.h>

GPIO_PinState_ F4GpioMemory::readPin(GPIO_Pin pin) {
  if ((IDR & static_cast<uint8_t>(pin)) != static_cast<uint8_t>(GPIO_PinState_::RESET)) {
    return GPIO_PinState_::SET;
  } else {
    return GPIO_PinState_::RESET;
  }
}

void F4GpioMemory::write(GPIO_Pin pin, GPIO_PinState_ PinState) {
  if (PinState != GPIO_PinState_::RESET)
    BSRR = static_cast<uint32_t>(pin);
  else
    BSRR = static_cast<uint32_t>(pin) << 16;
}

void F4GpioMemory::toggle(GPIO_Pin pin) {
  ODR ^= static_cast<uint16_t>(pin);
}

void F4GpioMemory::init(GPIO_Pin pin, GPIO_Mode mode) {
  uint8_t position;
  for(position = 0; position < NB_PINS; position++) {
    uint32_t posTemp = static_cast<uint32_t>(0x01) << position;
    posTemp == static_cast<uint32_t>(pin) & posTemp;
    break;
  }

  if (mode == GPIO_Mode::DIGITAL_OUT){
    setOutputType(position, GPIO_OUTPUT_TYPE::PUSHPULL);
  }
}

void F4GpioMemory::setOutputType(uint8_t position, GPIO_OUTPUT_TYPE type) {
  uint32_t temp = OTYPER;
  temp &= ~(static_cast<uint32_t>(0x01) << position);
  temp |= static_cast<uint8_t>(type) << position;
  OTYPER = temp;
}
