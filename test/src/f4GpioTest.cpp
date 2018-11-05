#include <memory>
#include "gmock/gmock.h"
#include "f4GpioMemory.h"

using namespace testing;

class F0Gpio : public IGpio
{
   public:
      F0Gpio() = default;
      ~F0Gpio() = default;

      GPIO_PinState_ readPin(GPIO_Pin pin) override {return gpio_memory->readPin(pin);}
      void writePin(GPIO_Pin pin, GPIO_PinState_ PinState) override {}
      void togglePin(GPIO_Pin pin) override {gpio_memory->toggle(pin);}

   private:
      std::unique_ptr<F4GpioMemory> gpio_memory = std::make_unique<F4GpioMemory>();
};

class GpioGroup : public Test
{
   public:
      std::unique_ptr<F0Gpio> gpio;

      void SetUp() override
      {
         gpio = std::make_unique<F0Gpio>();
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

