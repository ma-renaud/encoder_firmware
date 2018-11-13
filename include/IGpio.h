#ifndef IGPIO_H
#define IGPIO_H

#include <cstdint>

enum class GPIO_Pin : uint16_t {
  PIN_0 = 0x0001,
  PIN_1 = 0x0002,
  PIN_2 = 0x0004,
  PIN_3 = 0x0008,
  PIN_4 = 0x0010,
  PIN_5 = 0x0020,
  PIN_6 = 0x0040,
  PIN_7 = 0x0080,
  PIN_8 = 0x0100,
  PIN_9 = 0x0200,
  PIN_10 = 0x0400,
  PIN_11 = 0x0800,
  PIN_12 = 0x1000,
  PIN_13 = 0x2000,
  PIN_14 = 0x4000,
  PIN_15 = 0x8000,
  PIN_All = 0xFFFF
};

enum class GPIO_PinState_ : uint8_t {
  RESET = 0,
  SET
};

enum class GPIO_Mode : uint8_t {
  DIGITAL_OUT = 0,
  DIGITAL_IN
};

enum class GPIO_Direction_Mode : uint8_t {
  INPUT = 0,
  OUTPUT,
  ALTERNATE_FUNCTION,
  ANALOG
};

enum class GPIO_Output_Type : uint8_t {
  PUSH_PULL = 0,
  OPEN_DRAIN
};

enum class GPIO_Output_Speed : uint8_t {
  LOW_SPEED = 0,
  MEDIUM_SPEED,
  FAST_SPEED,
  HIGH_SPEED
};

enum class GPIO_Pupd : uint8_t {
  NOPULL = 0,
  PULLUP,
  PULLDOWN,
  RESERVED
};

class IGpio {
 public:
  virtual GPIO_PinState_ readPin(GPIO_Pin pin) = 0;
  virtual void writePin(GPIO_Pin pin, GPIO_PinState_ PinState) = 0;
  virtual void togglePin(GPIO_Pin pin) = 0;
  virtual void init(GPIO_Pin pin, GPIO_Mode mode) = 0;
};

#endif //IGPIO_H
