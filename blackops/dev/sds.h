#ifndef bops_dev_sdspi_h
#define bops_dev_sdspi_h
/** 
    Copyright (c) 2021, wicked systems
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
#include <os.h>
#include "bd.h"
#include "spi.h"
#include "sd/sd.h"

namespace dev {

/* sds
   SPI SDCard block device
*/
class sds: public bd
{
  spio    m_io;
  bool    m_type:2;
  bool    m_sdhc:1;
  bool    m_sdxc:1;
  int     m_status:3;
  bool    m_ready_bit:1;

  protected:
          bool  bus_acquire() noexcept;
          void  bus_release() noexcept;

  inline  int   cmd_put(std::uint8_t*) const noexcept {
          return 0;
  }

  template<typename Xt, typename... Args>
  inline  int   cmd_put(std::uint8_t* cp, Xt value, Args&&... arguments) const noexcept {
          auto  l_src = reinterpret_cast<std::uint8_t*>(std::addressof(value));
          int   l_length = sizeof(Xt);
          if constexpr (os::is_lsb) {
              auto  l_dst = cp + l_length - 1;
              while(l_dst >= cp) {
                  *l_dst-- = *l_src++;
              }
          } else
          if constexpr (os::is_msb) {
                static_assert("`sdspi` does not support Big Endian");
          }
          return l_length + cmd_put(cp + l_length, std::forward<Args>(arguments)...);
  }

          void  bus_poll(long int) noexcept;

          auto  dev_send_command(std::uint8_t, std::uint32_t = 0) noexcept -> sd_result_t;
          bool  dev_load_cid() noexcept;
          bool  dev_load_csd() noexcept;
          bool  dev_load_sb(std::uint8_t*, std::uint32_t) noexcept;
          bool  dev_load_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept;
          bool  dev_save_sb(std::uint8_t*, std::uint32_t) noexcept;
          bool  dev_save_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept;
          bool  dev_reset() noexcept;

  public:
          sds(spi*, unsigned int) noexcept;
          sds(const sds&) noexcept = delete;
          sds(sds&&) noexcept = delete;
  virtual ~sds();

          bool  resume() noexcept;
  virtual bool  load(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept override;
  virtual bool  save(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept override;
  virtual int   get_status() const noexcept override;
          bool  suspend() noexcept;

          sds& operator=(const sds&) noexcept = delete;
          sds& operator=(sds&&) noexcept = delete;
};

/*namespace dev*/ }
#endif
