#include <stdio.h>
#include <gamebit.h>
#include <dev/pwm.h>
#include <dev/pwm/osc.h>
#include <pico/stdlib.h>

bool  test_sound_01() noexcept
{
      dev::pwm  l_pwm;
      dev::osc* l_osc = l_pwm.get_osc(dev::osc::fct_sin, 250.0f, 1.0f);
      if(l_osc) {
          l_osc->set_bus(dev::osc::bus_com);
          l_osc->trigger(true);
      }
      while(true) {
      }
      return false;
}

int   main(int, char**)
{
      gamebit::initialise();
      test_sound_01();
      gamebit::dispose();
      return 0;
}
