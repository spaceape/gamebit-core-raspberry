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
#include "spi.h"
#include <hardware.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include "hardware/structs/spi.h"
#include "hardware/regs/spi.h"
#include <config.h>

namespace dev {

/* spi
*/
      spi::spi(spi_inst_t* dev, unsigned int rx_pin, unsigned int tx_pin, unsigned int clk_pin) noexcept:
      bus(),
      m_dev(dev),
      m_io_head(nullptr),
      m_io_tail(nullptr),
      m_dc_ptr(nullptr),
      m_dc_count(0),
      m_io_count(0),
      m_io_baud(0),
      m_ready_bit(false)
{
      if(dev != nullptr) {
          gpio_set_function(rx_pin, GPIO_FUNC_SPI);
          gpio_set_function(tx_pin, GPIO_FUNC_SPI);
          gpio_set_function(clk_pin, GPIO_FUNC_SPI);
          m_io_baud = spi_init(m_dev, hw_default_spi_baud);
          m_ready_bit = m_io_baud > 0;
      }
}

      spi::~spi()
{

      bus_dispose();
}

void  spi::monitor::bus_attach(spi*) noexcept
{
}

void  spi::monitor::bus_acquire(spi*) noexcept
{
}

void  spi::monitor::bus_release(spi*) noexcept
{
}

void  spi::monitor::bus_detach(spi*) noexcept
{
}

void  spi::bus_initialise() noexcept
{
}

bool  spi::bus_wait_release(spio* io, int timeout) const noexcept
{
      std::uint64_t  l_time_0;
      std::uint64_t  l_time_1  = time_us_64();
      std::uint64_t  l_dt      = 0;
      int            l_wait_us = timeout;
      if(timeout > 0) {
          // if timeout value is > 0 then wait for a finite amount of time
          while(m_dc_ptr != nullptr) {
              l_time_0 = l_time_1;
              bus_yield(io, l_wait_us);
              l_time_1 = time_us_64();
              l_dt     = l_time_1 - l_time_0;
              if(l_dt >= std::numeric_limits<unsigned int>::max() >> 1) {
                  return false;
              }
              l_wait_us -= l_dt;
              if(l_wait_us <= 0) {
                  break;
              }
          }
      } else
      if(timeout < 0) {
          // if timeout value is < 0 then block indefinitely
          while(m_dc_ptr != nullptr) {
              bus_yield(io, 0);
          }
      }
      return m_dc_ptr == nullptr;
}

void  spi::bus_yield(const spio* io, int wait) const noexcept
{
      io->yield(wait);
}

void  spi::bus_dispose() noexcept
{
      // detach all attached devices
      while(m_io_tail)
      {
          detach(m_io_tail);
      }
}

bool  spi::attach(spio* io) noexcept
{
      if(io) {
          if(io->m_bus == nullptr) {
              if(m_io_count < std::numeric_limits<std::int8_t>::max()) {
                  // link the device into the internal list
                  io->m_bus = this;
                  io->m_io_prev = m_io_tail;
                  if(io->m_io_prev) {
                      io->m_io_prev->m_io_next = io;
                  } else
                      m_io_head = io;
                  io->m_io_next = nullptr;
                  m_io_tail = io;
                  // notify the device that it's been attached
                  if(io->m_monitor) {
                      io->m_monitor->bus_attach(this);
                  }
                  // activate the bus if this is the first device being attached to it
                  if(m_io_count == 0) {
                      bus_initialise();
                  }
                  // set the device select line
                  if(true) {
                      gpio_init(io->get_cs_pin());
                      gpio_set_dir(io->get_cs_pin(), GPIO_OUT);
                      gpio_put(io->get_cs_pin(), 1);
                  }
                  ++m_io_count;
                  return true;
              }
          } else
          if(io->m_bus == this) {
              return true;
          }
      }
      return false;
}

int   spi::acquire(spio* io) noexcept
{
      if(m_dc_ptr != io) {
          if(m_dc_ptr != nullptr) {
              if(bool
                  l_acquire_success = bus_wait_release(io, io->get_timeout());
                  l_acquire_success == false) {
                  return -1;
              }
          }
          m_dc_ptr = io;
          if(m_dc_count == 0) {
              // bring down the select line for the device that just acquired the bus
              gpio_put(io->get_cs_pin(), 0);
              // reset the bus clock rate to match the prefference of the device that just acquired the bus
              if(long int
                  l_io_baud = io->get_baud();
                  l_io_baud != m_io_baud) {
                  m_io_baud = spi_set_baudrate(m_dev, l_io_baud);
                  // update the device baud rate to match what the bus can actually set, such that we don't need to
                  // enter this branch next time the device acquires the bus
                  if(m_io_baud != l_io_baud) {
                      io->set_baud(m_io_baud);
                  }
              }
              // let the device know that it has the bus
              if(io->m_monitor) {
                  io->m_monitor->bus_acquire(this);
              }
          }
      }
      return ++m_dc_count;
}

long int spi::recv(const spio* io, long int size) const noexcept
{
      if(m_dc_ptr == io) {
          auto          l_bus  = reinterpret_cast<spi_hw_t*>(m_dev);
          std::uint8_t  l_rxp  = io->get_rx_pattern();
          for(auto i_recv = 0; i_recv < size; i_recv++) {
              while((l_bus->sr & SPI_SSPSR_TNF_BITS) == 0) {
                  bus_yield(io, 0);
              }
              l_bus->dr = l_rxp;
              while((l_bus->sr & SPI_SSPSR_RNE_BITS) == 0) {
                  bus_yield(io, 0);
              }
              static_cast<void>(l_bus->dr);
          }
          l_bus->icr = SPI_SSPICR_RORIC_BITS;
          return size;
      }
      return 0;
}

long int spi::recv(const spio* io, std::uint8_t* data, long int size) const noexcept
{
      if(m_dc_ptr == io) {
          auto          l_bus  = reinterpret_cast<spi_hw_t*>(m_dev);
          std::uint8_t* i_data = data;
          std::uint8_t* i_last = i_data + size;
          std::uint8_t  l_rxp  = io->get_rx_pattern();
          while(i_data < i_last) {
              while((l_bus->sr & SPI_SSPSR_TNF_BITS) == 0) {
                  bus_yield(io, 0);
              }
              l_bus->dr = l_rxp;
              while((l_bus->sr & SPI_SSPSR_RNE_BITS) == 0) {
                  bus_yield(io, 0);
              }
              *i_data = l_bus->dr;
          }
          l_bus->icr = SPI_SSPICR_RORIC_BITS;
          return size;
      }
      return 0;
}

long int spi::send(const spio* io, std::uint8_t* data, long int size) const noexcept
{
      if(m_dc_ptr == io) {
          auto          l_bus  = reinterpret_cast<spi_hw_t*>(m_dev);
          std::uint8_t* i_data = data;
          std::uint8_t* i_last = i_data + size;
          while(i_data < i_last) {
              while((l_bus->sr & SPI_SSPSR_TNF_BITS) == 0) {
                  bus_yield(io, 0);
              }
              l_bus->dr = *i_data++;
              while((l_bus->sr & SPI_SSPSR_RNE_BITS) == 0) {
                  bus_yield(io, 0);
              }
              static_cast<void>(l_bus->dr);
          }
          l_bus->icr = SPI_SSPICR_RORIC_BITS;
          return size;
      }
      return 0;
}

long int spi::send(const spio* io, std::uint8_t data) const noexcept
{
      auto l_bus = reinterpret_cast<spi_hw_t*>(m_dev);
      if(m_dc_ptr == io) {
          while((l_bus->sr & SPI_SSPSR_TNF_BITS) == 0) {
              bus_yield(io, 0);
          }
          l_bus->dr = data;
          while((l_bus->sr & SPI_SSPSR_RNE_BITS) == 0) {
              bus_yield(io, 0);
          }
          static_cast<void>(l_bus->dr);
          return 1;
      }
      return 0;
}

int   spi::release(spio* io) noexcept
{
      if(m_dc_ptr == io) {
          if(m_dc_count > 0) {
              --m_dc_count;
              if(m_dc_count == 0) {
                  // let the device know that it has lost the bus
                  if(io->m_monitor) {
                      io->m_monitor->bus_release(this);
                  }
                  // bring up the select line for the device that just acquired the bus
                  gpio_put(io->get_cs_pin(), 1);
                  // clear the pointer to the current device to let others know that the bus is available
                  m_dc_ptr = nullptr;
              }
          }
          return m_dc_count;
      }
      return -1;
}

bool  spi::detach(spio* io) noexcept
{
      if(io) {
          if(io->m_bus == this) {
              // let the device know it's being detached
              if(io->m_monitor) {
                  io->m_monitor->bus_detach(this);
              }
              // we need to make sure that we release the bus if the device being detached is the currenltly selected one
              if(m_dc_ptr == io) {
                  while(m_dc_count > 0) {
                      release(io);
                      --m_dc_count;
                  }
              }
              // unlink the device from the internal list
              if(io->m_io_next) {
                  io->m_io_next->m_io_prev = io->m_io_prev;
              } else
                  m_io_tail = io->m_io_prev;
              if(io->m_io_prev) {
                  io->m_io_prev->m_io_next = io->m_io_next;
              } else
                  m_io_head = io->m_io_next;
              io->m_bus = nullptr;
              // update the bus state
              --m_io_count;
              if(m_io_count == 0) {
                  bus_dispose();
              }
              return true;
          }
      }
      return false;
}

bool  spi::is_ready(bool expect) const noexcept
{
      bool  l_enabled = m_ready_bit;
      bool  l_result = l_enabled == expect;
      return l_result;
}

bool  spi::is_suspended() const noexcept
{
      return is_ready(false);
}

/* spio
*/
      spio::spio(spi* bus, unsigned int cs_pin, unsigned int type) noexcept:
      device(type),
      m_bus(nullptr),
      m_io_prev(nullptr),
      m_io_next(nullptr),
      m_monitor(nullptr),
      m_cs_pin(cs_pin),
      m_rx_pattern(0),
      m_rx_dma(nullptr),
      m_baud(spi_default_baud),
      m_timeout(spi_default_timeout)
{
      if(bus) {
          bus->attach(this);
      }
}

      spio::~spio()
{
      if(m_bus) {
          m_bus->detach(this);
      }
}

bool  spio::acquire() noexcept
{
      return m_bus->acquire(this) >= 0;
}

long int spio::recv(long int size) const noexcept
{
      return m_bus->recv(this, size);
}

long int spio::recv(std::uint8_t* data, long int size) const noexcept
{
      return m_bus->recv(this, data, size);
}

long int spio::send(std::uint8_t* data, long int size) const noexcept
{
      return m_bus->send(this, data, size);
}

long int spio::send(std::uint8_t data) const noexcept
{
      return m_bus->send(this, data);
}

void  spio::release() noexcept
{
      m_bus->release(this);
}

std::uint8_t spio::get_cs_pin() const noexcept
{
      return m_cs_pin;
}

std::uint8_t spio::get_rx_pattern() const noexcept
{
      return m_rx_pattern;
}

void  spio::set_rx_pattern(std::uint8_t value) noexcept
{
      m_rx_pattern = value;
}

long int     spio::get_baud() const noexcept
{
      return m_baud;
}

void  spio::set_baud(long int value) noexcept
{
      m_baud = value;
}

long int     spio::get_timeout() const noexcept
{
      return m_timeout;
}

void  spio::set_timeout(long int value) noexcept
{
      m_timeout = value;
}

void  spio::set_monitor(spi::monitor* monitor) noexcept
{
      if(m_monitor != monitor) {
          if(m_monitor != nullptr) {
              if(m_bus != nullptr) {
                  m_monitor->bus_detach(m_bus);
              }
          }
          m_monitor = monitor;
          // notify the monitor that it's been attached to the bus, or otherwise (if it's attached late in the constructor)
          // it may miss it
          if(m_monitor != nullptr) {
              if(m_bus != nullptr) {
                  m_monitor->bus_attach(m_bus);
              }
          }
      }
}

bool  spio::is_ready(bool expect) const noexcept
{
      bool  l_enabled = m_bus && m_bus->is_ready();
      bool  l_result = l_enabled == expect;
      return l_result;
}

bool  spio::is_suspended() const noexcept
{
      return is_ready(false);
}

/*namespace dev*/ }
