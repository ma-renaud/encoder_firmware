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
  for (position = 0; position < NB_PINS; position++) {
    uint32_t posTemp = static_cast<uint32_t>(0x01) << position;
    if (posTemp == (static_cast<uint32_t>(pin) & posTemp))
      break;
  }

  if (mode == GPIO_Mode::DIGITAL_OUT) {
    setMode(position, GPIO_Direction_Mode::OUTPUT);
    setOutputType(position, GPIO_Output_Type::PUSH_PULL);
    setOutputSpeed(position, GPIO_Output_Speed::LOW_SPEED);
    setPull(position, GPIO_Pupd::NOPULL);
  } else if (mode == GPIO_Mode::DIGITAL_IN) {
    setMode(position, GPIO_Direction_Mode::INPUT);
    setPull(position, GPIO_Pupd::PULLDOWN);
  }
}

void F4GpioMemory::setMode(uint8_t position, GPIO_Direction_Mode mode) {
  uint32_t temp = MODER;
  temp &= ~(static_cast<uint32_t>(0x03) << (position * 2));
  temp |= (static_cast<uint8_t>(mode) << (position * 2));
  MODER = temp;
}

void F4GpioMemory::setOutputType(uint8_t position, GPIO_Output_Type type) {
  uint32_t temp = OTYPER;
  temp &= ~(static_cast<uint32_t>(0x01) << position);
  temp |= static_cast<uint8_t>(type) << position;
  OTYPER = temp;
}

void F4GpioMemory::setOutputSpeed(uint8_t position, GPIO_Output_Speed speed) {
  uint32_t temp = OSPEEDR;
  temp &= ~(static_cast<uint32_t>(0x03) << (position * 2));
  temp |= (static_cast<uint8_t>(speed) << (position * 2));
  OSPEEDR = temp;
}

void F4GpioMemory::setPull(uint8_t position, GPIO_Pupd pupd) {
  uint32_t temp = PUPDR;
  temp &= ~(static_cast<uint32_t>(0x03) << (position * 2));
  temp |= (static_cast<uint8_t>(pupd) << (position * 2));
  PUPDR = temp;
}
