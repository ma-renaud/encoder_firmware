#include "gmock/gmock.h"
#include "f4GpioMemory.h"

using namespace testing;

class GPIOExposed : public F4GpioMemory{
   public:
      uint32_t odr() const {return ODR;}
      uint32_t idr() const {return IDR;}

      void updateRegisters(){
        ODR = ODR & ~(BSRR >> 16) | (BSRR & 0xFFFF);
        BSRR = 0;
      }

      void setPin(GPIO_Pin pin) {
        IDR = IDR | (static_cast<uint32_t>(pin) & 0xFFFF);
      }
};

class GpioMemoryGroup : public Test
{
   public:
      static constexpr unsigned int GPIOREGISTERSSIZE = 40;
      std::array<uint32_t, GPIOREGISTERSSIZE/4> gpioRegisters;
      GPIOExposed* gpio = reinterpret_cast<GPIOExposed*>(gpioRegisters.data());

      void SetUp() override
      {
         gpioRegisters.fill(0);
      }

      void TearDown() override {}

      void updateRegisters(){
        gpioRegisters[5] = (gpioRegisters[5] & ~(gpioRegisters[6] >> 16)) | (gpioRegisters[6] & 0xFFFF);
        gpioRegisters[6] = 0;
      }

      void setPin(GPIO_Pin pin) {
        gpioRegisters[4] = gpioRegisters[4] | ((static_cast<uint32_t>(pin) & 0xFFFF));
      }
};

TEST_F(GpioMemoryGroup, TestInstanceSize)
{
  ASSERT_THAT(sizeof(F4GpioMemory), Eq(GPIOREGISTERSSIZE));
}

TEST_F(GpioMemoryGroup, TestToggle) {
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->toggle(selectedPin);

  ASSERT_THAT(gpioRegisters[5], Eq(static_cast<uint32_t>(selectedPin)));
}

TEST_F(GpioMemoryGroup, TestWriteToOne)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->write(selectedPin, GPIO_PinState_::SET);
  updateRegisters();

  ASSERT_THAT(gpioRegisters[5], Eq(static_cast<uint32_t>(selectedPin)));
}

TEST_F(GpioMemoryGroup, TestWriteToZero)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  gpio->write(selectedPin, GPIO_PinState_::SET);
  updateRegisters();
  gpio->write(selectedPin, GPIO_PinState_::RESET);
  updateRegisters();

  ASSERT_THAT(gpioRegisters[5], Eq(0));
}

TEST_F(GpioMemoryGroup, TestRead)
{
  GPIO_Pin selectedPin = GPIO_Pin::PIN_5;
  setPin(selectedPin);

  ASSERT_THAT(gpio->readPin(selectedPin), Eq(GPIO_PinState_::SET));
}
