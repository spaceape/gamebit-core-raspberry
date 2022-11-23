#ifndef bops_dev_uart_h
#define bops_dev_uart_h
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
#include <sys.h>
#include <sys/adt/device.h>

namespace dev {

/* uart_default_baud [Hz]
   default rate at which UART bus shall operate
*/
constexpr long int uart_default_baud = 115200;

/* uart
   stream for uart devices
*/
class uart: public sys::device, public sys::ios
{
  uart_inst_t*  m_uart;
  long int      m_mode;

  public:
  friend class spio;

  public:
          uart(uart_inst_t*, unsigned int, unsigned int) noexcept;
          uart(const uart&) noexcept = delete;
          uart(uart&&) noexcept = delete;
  virtual ~uart();

  virtual int  get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

  virtual int  seek(int, int) noexcept override;

  virtual int  read(int) noexcept override;
  virtual int  read(int, char*) noexcept override;

  virtual int  put_char(char) noexcept override;
  virtual int  put_byte(unsigned char) noexcept override;
  virtual int  write(int, const char*) noexcept override;

  virtual int  get_size() noexcept override;

          void set_baud(long int) noexcept;
          void set_blocking(bool = true) noexcept;
          void set_nonblocking() noexcept;
  
  virtual bool is_seekable() const noexcept override;
  virtual bool is_readable() const noexcept override;
  virtual bool is_writable() const noexcept override;

  static  uart*  get_from_adt(const char* path) noexcept {
          return cast_from_adt<uart>(path);
  }

          uart& operator=(const uart&) noexcept = delete;
          uart& operator=(uart&&) noexcept = delete;
};

/*namespace dev*/ }
#endif
