#include <memory>
#include "gmock/gmock.h"
#include "f4Gpio.h"

using namespace testing;

class GpioGroup : public Test {
 public:
  static constexpr unsigned int GPIOREGISTERSSIZE = 40;
  std::array<uint32_t, sizeof(F4GpioMemory)/4> gpioRegisters;
  std::unique_ptr<IGpio> gpio;

  void SetUp() override {
    gpioRegisters.fill(0);
    gpio = std::make_unique<F4Gpio>(reinterpret_cast<uintptr_t>(gpioRegisters.data()));
  }

  void TearDown() override {
    gpio.reset();
  }

  void updateRegisters(){
    gpioRegisters[5] = gpioRegisters[5] & ~(gpioRegisters[6] >> 16) | (gpioRegisters[6] & 0xFFFF);
    gpioRegisters[6] = 0;
  }

  void setPin(GPIO_Pin pin) {
    gpioRegisters[4] = gpioRegisters[4] | ((static_cast<uint32_t>(pin) & 0xFFFF));
  }
};

TEST_F(GpioGroup, TestInstanceSize)
{
  ASSERT_THAT(sizeof(F4GpioMemory), Eq(GPIOREGISTERSSIZE));
}

TEST_F(GpioGroup, TestToggle) {
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->togglePin(selectedPin);

  ASSERT_THAT(gpioRegisters[5], Eq(static_cast<uint32_t>(selectedPin)));
}

TEST_F(GpioGroup, TestWriteToOne)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->writePin(selectedPin, GPIO_PinState_::GPIO_PIN_SET);
  updateRegisters();

  ASSERT_THAT(gpioRegisters[5], Eq(static_cast<uint32_t>(selectedPin)));
}

TEST_F(GpioGroup, TestWriteToZero)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->writePin(selectedPin, GPIO_PinState_::GPIO_PIN_SET);
  updateRegisters();
  gpio->writePin(selectedPin, GPIO_PinState_::GPIO_PIN_RESET);
  updateRegisters();

  ASSERT_THAT(gpioRegisters[5], Eq(0));
}

TEST_F(GpioGroup, TestRead)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  setPin(selectedPin);

  ASSERT_THAT(gpio->readPin(selectedPin), Eq(GPIO_PinState_::GPIO_PIN_SET));
}


