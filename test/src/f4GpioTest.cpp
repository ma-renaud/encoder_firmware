#include <memory>
#include "gmock/gmock.h"
#include "f4GpioMemory.h"

using namespace testing;

class F4Gpio : public IGpio
{
   public:
      F4Gpio() = default;
      ~F4Gpio() = default;

      GPIO_PinState_ readPin(GPIO_Pin pin) override {return gpio_memory->readPin(pin);}
      void writePin(GPIO_Pin pin, GPIO_PinState_ PinState) override {}
      void togglePin(GPIO_Pin pin) override {gpio_memory->toggle(pin);}

   private:
      std::unique_ptr<F4GpioMemory> gpio_memory = std::make_unique<F4GpioMemory>();
};

class GpioGroup : public Test
{
   public:
      std::unique_ptr<F4Gpio> gpio;

      void SetUp() override
      {
         gpio = std::make_unique<F4Gpio>();
      }

      void TearDown() override
      {
         gpio.reset();
      }
};

TEST_F(GpioGroup, TestTest)
{
   FAIL();
}

