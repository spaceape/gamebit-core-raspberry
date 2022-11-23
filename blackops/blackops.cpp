/** 
    Copyright (c) 2022, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "blackops.h"
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/stdio_uart.h>

#include "hardware.h"
#include <sys.h>
#include <sys/adt.h>
#include <sys/adt/node.h>
#include <sys/adt/device.h>
#include <sys/adt/directory.h>
#include <sys/adt/tree.h>
#include "dev/uart.h"
#include "dev/sds.h"
#include "dev/ili9341s.h"
#include "controller.h"

#include <config.h>
#include <version.h>
#include <hardware/spi.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <error.h>
#include <dbg.h>

/* hw_dev_array_max
   how many devices of a single kind to expect; used very short term to store pointers to availeble devices
*/
constexpr unsigned int hw_dev_array_max = 8;

/* g_*_dir
  base ADT nodes;
  ther rest of the tree is dynamically generated based upon configuration settings and runtime parameters
*/
sys::directory  s_adt;
sys::directory* g_adt = std::addressof(s_adt);

sys::static_mount_point<sys::directory>  g_env_dir(g_adt, bops_env_dirname);
sys::static_mount_point<sys::directory>  g_mnt_dir(g_adt, bops_mnt_dirname);
sys::static_mount_point<sys::directory>  g_dev_dir(g_adt, bops_dev_dirname);
sys::static_mount_point<sys::directory>  g_dyn_dir(g_adt, bops_dyn_dirname);
sys::static_mount_point<sys::directory>  g_proc_dir(g_adt, bops_proc_dirname);

bool post();

bool g_ready;

/* initialise()
   setup peripherals and runtime resources
*/
bool  initialise(unsigned int) noexcept
{
      if(g_ready == false) {
          sys::device*  l_stdio = nullptr;
          auto          l_dev   = g_dev_dir.get_ptr();
          bool          l_stdio_success = true;
          bool          l_sdc_success = true;
          bool          l_lcd_success = true;
          bool          l_post_success;

          // forgot what this does: turn the power FET on harder?
          if constexpr (hw_power_hard) {
              gpio_set_dir(23, GPIO_OUT);
              gpio_put(23, 1);
          }

          if constexpr (is_debug) {
              stdio_uart_init();
          }

          // enable UART devices
          if constexpr (hw_enable_uart0 || hw_enable_uart1) {
              sys::node* l_uart[2];
              if constexpr (hw_enable_uart0) {
                  l_uart[0] = l_dev->make_node<dev::uart>("uart0", uart0, pin_uart0_rx, pin_uart0_tx);
                  if(l_uart[0] != nullptr) {
                      l_stdio = l_uart[0]->get_device();
                  }
                  l_stdio_success &= (l_uart[0] != nullptr);
              }
              if constexpr (hw_enable_uart1) {
                  l_uart[1] = l_dev->make_node<dev::uart>("uart1", uart1, pin_uart1_rx, pin_uart1_tx);
                  if(l_uart[1] != nullptr) {
                      if(l_uart[0] == nullptr) {
                          l_stdio = l_uart[1]->get_device();
                      }
                  }
                  l_stdio_success &= (l_uart[1] != nullptr);
              }
              stdio_set_driver_enabled(&stdio_uart, true);
              uart_puts(uart0, bops_name);
              uart_puts(uart0, ", v");
              uart_puts(uart0, bops_version);
              uart_puts(uart0, "\n");
          }

          // enable SPI devices;
          // SPI devices will be created with a nodename of 'spi0', 'spi1',... - unless there is a single SPI device available on
          // the system, in which case its name will simply be 'spi';
          // if a `hw_default_spi` is set, then an additional entry called `spi` will be created to point to the specified device
          if constexpr (hw_enable_spi0 || hw_enable_spi1) {
              int        l_spi_count = 0;
              sys::node* l_spi[2];
              if constexpr (hw_enable_spi0) {
                  l_spi_count++;
              }
              if constexpr (hw_enable_spi1) {
                  l_spi_count++;
              }
              if constexpr (hw_enable_spi0) {
                  if(l_spi_count > 1) {
                      l_spi[0] = l_dev->make_node<dev::spi>("spi0", spi0, pin_spi0_rx, pin_spi0_tx, pin_spi0_sck);
                  } else
                  if(l_spi_count == 1) {
                      l_spi[0] = l_dev->make_node<dev::spi>("spi", spi0, pin_spi0_rx, pin_spi0_tx, pin_spi0_sck);
                  }
              }
              if constexpr (hw_enable_spi1) {
                  if(l_spi_count > 1) {
                      l_spi[1] = l_dev->make_node<dev::spi>("spi1", spi1, pin_spi1_rx, pin_spi1_tx, pin_spi1_sck);
                  } else
                  if(l_spi_count == 1) {
                      l_spi[1] = l_dev->make_node<dev::spi>("spi", spi1, pin_spi1_rx, pin_spi1_tx, pin_spi1_sck);
                  }
              }
              if(l_spi_count > 1) {
                  if((hw_default_spi_index >= 0) &&
                      (hw_default_spi_index < l_spi_count)) {
                      l_dev->make_link("spi", l_spi[hw_default_spi_index]);
                  }
              }
          }

          // set up SDC
          if constexpr (hw_enable_sdc) {
              auto  l_sdc_bus = dev::spi::get_from_adt(hw_sdc_bus);
              if(l_sdc_bus) {
                  auto  l_sdc_dev = sys::device::make<dev::sds>(l_dev, "mmc", l_sdc_bus, pin_sdc_cs);
                  if(l_sdc_dev != nullptr) {
                      l_sdc_success = l_sdc_dev->resume();
                  } else
                      l_sdc_success = show_error(
                          err_fail,
                          "Unable to spawn `sdcard` device.",
                          __FILE__,
                          __LINE__,
                          hw_sdc_bus
                      );
              } else
                  l_sdc_success = show_error(
                      err_fail,
                      "Unable to address device at location `%s`.",
                      __FILE__,
                      __LINE__,
                      hw_sdc_bus
                  );
          }

          // set up LCD
          if constexpr (hw_enable_lcd) {
              auto  l_lcd_bus = dev::spi::get_from_adt(hw_lcd_bus);
              if(l_lcd_bus) {
                  auto  l_lcd_dev = sys::device::make<dev::ili9341s>(l_dev, "display", l_lcd_bus, pin_lcd_cs, pin_lcd_ds, pin_lcd_reset);
                  if(l_lcd_dev != nullptr) {
                      l_lcd_success = l_lcd_dev->resume();
                  } else
                      l_lcd_success = show_error(
                          err_fail,
                          "Unable to spawn `sdcard` device.",
                          __FILE__,
                          __LINE__,
                          hw_lcd_bus
                      );
              } else
                  l_lcd_success = show_error(
                      err_fail,
                      "Unable to address device at location `%s`.",
                      __FILE__,
                      __LINE__,
                      hw_lcd_bus
                  );
          }

          if(l_stdio_success &&
              l_sdc_success &&
              l_lcd_success) {
              l_post_success = post();
              if(l_post_success) {
                  g_ready = true;
              }
          }

          g_adt->list();
      }
      return g_ready;
}

/* dispose
   release hardware resources
*/
bool  dispose() noexcept
{
      return true;
}

/* panic()
*/
void  panic() noexcept
{
      // go, blinky!
      // hard reset to go out of this routine
      gpio_init(PICO_DEFAULT_LED_PIN);
      gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
      while(true) {
          gpio_put(PICO_DEFAULT_LED_PIN, 1);
          sleep_ms(250);
          gpio_put(PICO_DEFAULT_LED_PIN, 0);
          sleep_ms(250);
      }
}
