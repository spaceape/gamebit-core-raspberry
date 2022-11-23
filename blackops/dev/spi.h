#ifndef dev_spi_h
#define dev_spi_h
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
#include <blackops.h>
#include <hardware.h>
#include "bus.h"
#include <hardware/gpio.h>
#include <hardware/spi.h>

namespace dev {

class spi;
class spio;

/* spi_default_timeout [us]
   default timeout before acquire() and read() operations return
*/
constexpr long int spi_default_timeout = -1;

/* spi_default_baud [Hz]
   default rate at which SPI bus shall operate
*/
constexpr long int spi_default_baud = 16777216;   //16MHz

/* spi
   hardware SPI bus; abstracts away pin and port assignments, as well as the control logic for multiple devices sharing it
*/
class spi: public bus
{
  spi_inst_t*   m_dev;
  spio*         m_io_head;
  spio*         m_io_tail;
  spio*         m_dc_ptr;       // device which acquired and currently controlls the bus
  std::int8_t   m_dc_count;     // how many locks current device has onto the bus
  std::int8_t   m_io_count;     // how many devices are registered to the bus
  long int      m_io_baud;
  bool          m_ready_bit:1;

  private:
          void  bus_initialise() noexcept;
          bool  bus_wait_release(spio*, int) const noexcept;
          void  bus_yield(const spio*, int) const noexcept;
          void  bus_dispose() noexcept;


  public:
  static constexpr unsigned int type = type_undef;

  struct monitor
  {
    virtual void  bus_attach(spi*) noexcept;
    virtual void  bus_acquire(spi*) noexcept;
    virtual void  bus_release(spi*) noexcept;
    virtual void  bus_detach(spi*) noexcept;
  };

  friend class spio;

  public:
          spi(spi_inst_t*, unsigned int, unsigned int, unsigned int) noexcept;
          spi(const spi&) noexcept = delete;
          spi(spi&&) noexcept = delete;
          ~spi();

          bool  attach(spio*) noexcept;
          int   acquire(spio*) noexcept;
          long int recv(const spio*, long int) const noexcept;
          long int recv(const spio*, std::uint8_t*, long int) const noexcept;
          long int send(const spio*, std::uint8_t*, long int) const noexcept;
          long int send(const spio*, std::uint8_t) const noexcept;
          int   release(spio*) noexcept;
          bool  detach(spio*) noexcept;

  static  spi*  get_from_adt(const char* path) noexcept {
          return cast_from_adt<spi>(path);
  }

          bool  is_ready(bool = true) const noexcept;
          bool  is_suspended() const noexcept;

          spi& operator=(const spi&) noexcept = delete;
          spi& operator=(spi&&) noexcept = delete;
};

/* spio
   generic device on the SPI bus; abstracts away pin assignments and select logic
*/
class spio: public sys::device
{
  spi*          m_bus;
  spio*         m_io_prev;
  spio*         m_io_next;
  spi::monitor* m_monitor;

  std::uint8_t  m_cs_pin;
  std::uint8_t  m_rx_pattern;   // what pattern to emit on the data line while clocking the bus for a read
  void*         m_rx_dma;       // DMA channel for data transfer
  long int      m_baud;
  long int      m_timeout:24;

  public:
  friend class spi;

  public:
          spio(spi*, unsigned int, unsigned int = sys::device::type_undef) noexcept;
          spio(const spio&) noexcept = delete;
          spio(spio&&) noexcept = delete;
  virtual ~spio();

          bool acquire() noexcept;
          long int  recv(long int) const noexcept;
          long int  recv(std::uint8_t*, long int) const noexcept;
          long int  send(std::uint8_t*, long int) const noexcept;
          long int  send(std::uint8_t) const noexcept;
          void release() noexcept;

          std::uint8_t get_cs_pin() const noexcept;
          std::uint8_t get_rx_pattern() const noexcept;
          void         set_rx_pattern(std::uint8_t) noexcept;
          long int     get_baud() const noexcept;
          void         set_baud(long int) noexcept;
          long int     get_timeout() const noexcept;
          void         set_timeout(long int) noexcept;

          void         set_monitor(spi::monitor*) noexcept;

          bool         is_ready(bool = true) const noexcept;
          bool         is_suspended() const noexcept;

          spio& operator=(const spio&) noexcept = delete;
          spio& operator=(spio&&) noexcept = delete;
};

/*namespace dev*/ }
#endif
