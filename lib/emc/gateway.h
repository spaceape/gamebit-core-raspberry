#ifndef emc_gateway_h
#define emc_gateway_h
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
#include <emc.h>
#include "protocol.h"
#include "command.h"
#include "timer.h"
#include <atomic>
#include <sys.h>
#include <sys/fmt.h>
#include <sys/ios.h>

namespace emc {

/* gateway
   base class for a duplex protocol stream
   - setup and maintain connection with the server
   - build and queue the protocol messages
   - recover the stream in case of incomplete or invalid messages
*/
class gateway
{
  char*     m_recv_data;
  sys::ios* m_recv_io;
  int       m_recv_iter;
  int       m_recv_last;
  int       m_recv_used;
  int       m_recv_size;
  int       m_recv_packet_left;
  int       m_recv_packet_size;
  int       m_recv_mtu;
  int       m_recv_state;

  char*     m_send_data;
  sys::ios* m_send_io;
  int       m_send_iter;
  int       m_send_last;
  int       m_send_used;
  int       m_send_size;
  int       m_send_mtu;

  protected:
  char      m_gate_name[emc_name_size];
  char      m_gate_info[emc_info_size];
  bool      m_user_role;                // client mode, send requests and wait for responses
  bool      m_host_role;                // server mode, accept and respond to requests
  int       m_gate_ping_time;           // time of silence before triggering a ping
  int       m_gate_info_time;           // time to wait for an info response, before sending an info request
  int       m_gate_wait_time;     
  int       m_gate_drop_time;           // time of silence before any incomplete message is discarded
  int       m_gate_trip_time;           // time of silence before the gateway is declared disconnected
  bool      m_dispatch_request;
  bool      m_dispatch_response;
  bool      m_dispatch_comment;
  bool      m_dispatch_packet;
  bool      m_cache_enable;             // enable write cache
  bool      m_flush_auto;               // send pending data as soon as EOL is written to the data buffer
  bool      m_flush_sync;               // send pending data upon sync() [[not implemented]]

  private:
  int       m_load_min;
  int       m_load_max;

  command   m_args;                     // request/response command line

  timer     m_ping_ctr;
  timer     m_info_ctr;                 // info timer    
  timer     m_drop_ctr;                 // drop timer
  timer     m_trip_ctr;                 // trip timer

  public:
  int       m_msg_recv;
  int       m_msg_drop;
  int       m_msg_tmit;
  int       m_chr_recv;
  int       m_chr_tmit;
  int       m_mem_size;
  int       m_mem_used;
  bool      m_active_bit;               // connection is active
  bool      m_healty_bit;               // connection is healthy

  private:
          void  emc_emit(char) noexcept;
          void  emc_emit(int, const char*) noexcept;
          char* emc_reserve(int) noexcept;
          void  emc_capture_request(char*, int) noexcept;
          void  emc_capture_response(char*, int) noexcept;
          void  emc_capture_comment(char*, int) noexcept;
          void  emc_capture_packet(char*, int, int) noexcept;
          void  emc_drop() noexcept;
          void  emc_trip() noexcept;

  protected:
          int   get_send_mtu() const noexcept;
          bool  set_send_mtu(int) noexcept;
          bool  set_recv_io(sys::ios*) noexcept;
          bool  set_send_io(sys::ios*) noexcept;

          void  emc_listen() noexcept;
          void  emc_connect() noexcept;

  inline  void  emc_put() noexcept {
  }

  template<typename... Args>
  inline  void  emc_put(char c, Args&&... next) noexcept {
          emc_emit(c);
          emc_put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  void  emc_put(const char* text, Args&&... next) noexcept {
          emc_emit(0, text);
          emc_put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  void  emc_put(const fmt::d& value, Args&&... next) noexcept {
          emc_emit(0, value);
          emc_put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  void  emc_put(const fmt::x& value, Args&&... next) noexcept {
          emc_emit(0, value);
          emc_put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  void  emc_put(const fmt::X& value, Args&&... next) noexcept {
          emc_emit(0, value);
          emc_put(std::forward<Args>(next)...);
  }

  // template<typename... Args>
  // inline  void  emc_put(const fmt::f& value, Args&&... next) noexcept {
  //         emc_save(0, value);
  //         emc_put(std::forward<Args>(next)...);
  // }

          void  emc_send_info_response() noexcept;
          void  emc_send_info_request() noexcept;
          void  emc_send_support_response() noexcept;
          void  emc_send_ping_request() noexcept;
          void  emc_send_pong_response() noexcept;
          void  emc_send_sync() noexcept;
          void  emc_send_help() noexcept;
          int   emc_send_error(int, const char* = nullptr, const char* = nullptr) noexcept;

  virtual void  emc_dispatch_request(const char*, int) noexcept;
  virtual int   emc_process_request(int, command&) noexcept;
  virtual void  emc_dispatch_response(const char*, int) noexcept;
  virtual int   emc_process_response(int, command&) noexcept;
  virtual void  emc_dispatch_comment(const char*, int) noexcept;
  virtual void  emc_dispatch_packet(const char*, int, int) noexcept;
  virtual void  emc_dispatch_disconnect() noexcept;

          void  emc_disconnect() noexcept;

  public:
          gateway() noexcept;
          gateway(sys::ios*, sys::ios* = nullptr) noexcept;
          gateway(const gateway&) noexcept = delete;
          gateway(gateway&&) noexcept = delete;
  virtual ~gateway();

          void     feed() noexcept;
          void     flush() noexcept;
          void     reset() noexcept;
          void     sync(const absolute_time_t&) noexcept;

          gateway& operator=(const gateway&) noexcept = delete;
          gateway& operator=(gateway&&) noexcept = delete;
};

/*namespace emc*/ }
#endif
