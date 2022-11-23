/** 
    Copyright (c) 2020, wicked systems
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
#include "gateway.h"
#include <config.h>
#include <cstring>

namespace emc {

      constexpr int  s_mtu_min = 256;
      constexpr int  s_mtu_max = 4096;

      constexpr int  s_state_drop = std::numeric_limits<int>::min();  // discard all received input
      constexpr int  s_state_recover = -1;                            // buffer crashed, recover
      constexpr int  s_state_accept = 0;                              // normal state: read in and capture traffic
      constexpr int  s_state_capture_message = 2;                     // normal state, capture message until it completes
      constexpr int  s_state_capture_packet = 3;                      // normal state, capture packet until it completes

      constexpr int  s_copy_size_min = global::cache_small_max;
      constexpr int  s_copy_size_max = global::cache_large_max;

/* brief sanity checks for the global settings
*/
static_assert(EOF < 0, "this module requires `char` to be signed, please recompile with \"-fsigned-char\"");
static_assert(message_wait_time < message_drop_time, "message_wait_time should not exceed the message_drop_time");
static_assert(message_trip_time > message_wait_time, "message_trip_time should be greater than message_wait_time");
static_assert(message_ping_time > message_wait_time, "message_ping_time should be greater than message_wait_time");
static_assert(message_ping_time < 
                message_trip_time - message_wait_time - message_wait_time, 
                "message_ping_time should not be greater than the time it takes a ping message to arrive, be"
                "processed and circle back at the destination"
);

      gateway::gateway() noexcept:
      m_recv_data(nullptr),
      m_recv_io(nullptr),
      m_recv_iter(0),
      m_recv_last(0),
      m_recv_used(0),
      m_recv_size(0),
      m_recv_mtu(std::numeric_limits<short int>::max()),
      m_recv_state(s_state_accept),
      m_send_data(nullptr),
      m_send_io(nullptr),
      m_send_iter(0),
      m_send_last(0),
      m_send_used(0),
      m_send_size(0),
      m_send_mtu(mtu_size),
      m_gate_name{0},
      m_gate_info{0},
      m_user_role(false),
      m_host_role(false),
      m_gate_ping_time(message_ping_time),
      m_gate_info_time(message_wait_time + message_wait_time),
      m_gate_wait_time(message_wait_time),
      m_gate_drop_time(message_drop_time),
      m_gate_trip_time(message_trip_time),
      m_dispatch_request(false),
      m_dispatch_response(false),
      m_dispatch_comment(false),
      m_dispatch_packet(false),
      m_cache_enable(false),
      m_flush_auto(false),
      m_flush_sync(false),
      m_load_min(s_copy_size_min),
      m_load_max(s_copy_size_max),
      m_ping_ctr(false),
      m_info_ctr(false),
      m_drop_ctr(false),
      m_trip_ctr(false),
      m_msg_recv(0),
      m_msg_drop(0),
      m_msg_tmit(0),
      m_chr_recv(0),
      m_chr_tmit(0),
      m_mem_size(0),
      m_mem_used(0),
      m_active_bit(0),
      m_healty_bit(0)
{
      if constexpr (queue_size_min > 0) {
          m_recv_data = reinterpret_cast<char*>(::malloc(queue_size_min));
          m_recv_size = queue_size_min;
          if(m_cache_enable) {
              m_send_data = reinterpret_cast<char*>(::malloc(queue_size_min));
              m_send_size = queue_size_min;
          }
      }
}

      gateway::gateway(sys::ios* recv_io, sys::ios* send_io) noexcept:
      gateway()
{
      if(recv_io != nullptr) {
          m_recv_io = recv_io;
          if(send_io == nullptr) {
              m_send_io = recv_io;
          }
      }
      if(m_send_io != nullptr) {
          m_send_io = send_io;
      }
}
    
      gateway::~gateway()
{
      if(m_send_data) {
          free(m_send_data);
      }
      if(m_recv_data) {
          free(m_recv_data);
      }
}

void  gateway::emc_emit(char c) noexcept
{
      if(m_cache_enable) {
          char* l_save_ptr = emc_reserve(1);
          if(l_save_ptr) {
              *l_save_ptr = c;
              m_send_last =(l_save_ptr - m_send_data) + 1;
              m_send_used = m_send_last;
              // auto-flush enabled: call flush() if the char being stored into the output buffer is EOL
              if(m_flush_auto) {
                  if(c == EOL) {
                      flush();
                  }
              }
          }
      }
      else {
          m_send_io->put_char(c);
      }
}

void  gateway::emc_emit(int size, const char* text) noexcept
{
      char* l_save_ptr = nullptr;
      if(text) {
          if(size >= 0) {
              if(size == 0) {
                  size = std::strlen(text);
              }
              if(m_cache_enable) {
                  if((text < m_send_data + m_send_last) ||
                      (text > m_send_data + m_send_size)) {
                      l_save_ptr = emc_reserve(size);
                      if(l_save_ptr) {
                          std::strncpy(l_save_ptr, text, size);
                      }
                  }
                  if(l_save_ptr) {
                      m_send_last =(l_save_ptr - m_send_data) + size;
                      m_send_used = m_send_last;
                      // auto-flush enabled: call flush() if the last char being stored into the output buffer is EOL
                      if(m_flush_auto) {
                          if(text[size - 1] == EOL) {
                              flush();
                          }
                      }
                  }
              }
              else {
                  m_send_io->write(size, text);
              }
          }
      }
}

char* gateway::emc_reserve(int size) noexcept
{
      char* l_result = nullptr;
      if(size >= 0) {
          int l_reserve_offset = m_send_used;
          int l_reserve_last   = l_reserve_offset + size;
          if(l_reserve_last > m_send_size) {
              m_send_data = reinterpret_cast<char*>(::realloc(m_send_data, l_reserve_last));
              m_send_size = l_reserve_last;
          }
          if(m_send_data) {
              m_send_used = l_reserve_last + size;
              l_result    = m_send_data + l_reserve_offset;
          }
      }
      return l_result;
}

/* emc_capture_request()
   handle standard incoming request
*/
void  gateway::emc_capture_request(char* message, int length) noexcept
{
      int l_rc = 0;
      int l_send_marker = m_send_last;
    
      if(m_dispatch_request) {
          emc_dispatch_request(message, length);
      }
    
      // load the received message string onto up the command pre-parser
      if(int l_argc = m_args.load(message + 1); l_argc > 0) {
          if(m_args[0].has_size(1)) {
              int l_tag = m_args[0][0];
              switch(l_tag) {
                  case emc_request_info:
                      emc_send_info_response();
                      l_rc = 0;
                      break;
                  default:
                      l_rc = emc_process_request(l_argc, m_args);
                      break;
              }
          }
          else {
              l_rc = emc_process_request(l_argc, m_args);
          }
      }
      else {
          // command did not parse successfully - process as a commend and set an error
          emc_capture_comment(message, length);
          l_rc = 1;
      }
          
      // send error if the rc is not zero and not already sent by the process function
      if(l_rc) {
          if(l_send_marker == m_send_last) {
              emc_send_error(l_rc);
          }
      }
}

int   gateway::emc_process_request(int, command&) noexcept
{
      return 0;
}

void  gateway::emc_capture_response(char* message, int length) noexcept
{
      if(m_dispatch_response) {
          emc_dispatch_response(message, length);
      }
      if(int l_argc = m_args.load(message + 1); l_argc > 0) {
          if(m_args[0].has_size(1)) {
              int  l_tag = m_args[0][0];
              switch(l_tag) {
                  case emc_response_info: {
                      if(m_args[1].has_text(emc_protocol_name)) {
                          if(m_args[2].has_text(emc_protocol_version, 0, 3)) {
                              char* l_name;
                              char* l_info;
                              int   l_mtu;
                              if(m_args[3].has_text()) {
                                  if(m_args[3].get_size() <= emc_name_size) {
                                      l_name = m_args[3].get_text();
                                  } else
                                      break;
                              } else
                                  break;
                              if(m_args[4].has_text()) {
                                  if(m_args[4].get_size() <= emc_info_size) {
                                      l_info = m_args[4].get_text();
                                  } else
                                      break;
                              } else
                                  break;
                              if(m_args[5].has_text()) {
                                  l_mtu = m_args[5].get_dec_int();
                                  if((l_mtu >= s_mtu_min) &&
                                      (l_mtu <= s_mtu_max)) {
                                        std::strncpy(m_gate_name, l_name, emc_name_size);
                                        std::strncpy(m_gate_info, l_info, emc_info_size);
                                        set_send_mtu(l_mtu);
                                        m_trip_ctr.suspend();
                                        m_trip_ctr.reset();
                                  } else
                                      break;
                              } else
                                  break;
                          }
                      }
                  }
                      break;
                  case emc_response_support:
                      break;
                  case emc_response_pong:
                      break;
                  case emc_response_error:
                      break;
                  case emc_response_bye:
                      break;
                  default:
                      emc_process_response(l_argc, m_args);
                      break;
              }
          } else
              emc_process_response(l_argc, m_args);
      }
}

int   gateway::emc_process_response(int, command&) noexcept
{
      return 0;
}

void  gateway::emc_capture_comment(char* message, int length) noexcept
{
      if(m_dispatch_comment) {
          emc_dispatch_comment(message, length);
      }
}

void  gateway::emc_capture_packet(char* message, int length, int channel) noexcept
{
      if(m_dispatch_packet) {
          emc_dispatch_packet(message, length, channel);
      }
}

void  gateway::emc_drop() noexcept
{
      // if(m_recv_used > m_recv_last) {
      //     m_recv_used = m_recv_last;
      //     m_msg_drop++;
      // }
}

void  gateway::emc_trip() noexcept
{
      emc_disconnect();
}

int   gateway::get_send_mtu() const noexcept
{
      return m_send_mtu;
}

bool  gateway::set_send_mtu(int mtu) noexcept
{
      if(mtu >= s_mtu_min) {
          if(mtu <= s_mtu_max) {
              m_send_mtu = mtu;
              return true;
          }
      }
      return false;
}

bool  gateway::set_recv_io(sys::ios* io) noexcept
{
      if(m_recv_io != io) {
          if(m_recv_io != nullptr) {
              if((m_recv_iter == 0) &&
                  (m_recv_last == 0)) {
                  m_recv_io = io;
                  m_active_bit = true;
                  return true;
              }
          } else
          if(m_recv_io == nullptr) {
              m_recv_io = io;
              return true;
          }
      } else
      if(m_recv_io != nullptr) {
          return true;
      }
      return false;
}

bool  gateway::set_send_io(sys::ios* io) noexcept
{
      if(m_send_io != io) {
          if(m_send_io != nullptr) {
              if(m_send_iter == m_send_last) {
                  m_send_io = io;
                  m_active_bit = true;
                  return true;
              }
          } else
          if(m_send_io == nullptr) {
              m_send_io = io;
          }
      } else
      if(m_send_io != nullptr) {
          return true;
      }
      return false;
}

/* connect
   use the supplied streams and wake up as a client
*/
void  gateway::emc_connect() noexcept
{
      if(m_user_role == false) {
          if(m_send_io != nullptr) {
              m_ping_ctr.resume();
              m_info_ctr.resume();
              m_trip_ctr.resume();
              m_recv_state = s_state_accept;
              m_user_role = true;
              m_active_bit = true;
              m_healty_bit = false;
          }
      }
}

/* connect
   use the supplied streams and wake up as a server
*/
void  gateway::emc_listen() noexcept
{
      if(m_host_role == false) {
          if(m_recv_io != nullptr) {
              m_drop_ctr.resume();
              m_host_role = true;
              m_active_bit = true;
          }
          if(m_host_role) {
              emc_send_sync();
          }
      }
}

void  gateway::emc_send_info_response() noexcept
{
      const char* l_machine_name = m_gate_name;
      const char* l_machine_type = nullptr;
      const char* l_architecture_name = os::cpu_name;
      const char* l_architecture_type;

      if((l_machine_name == nullptr) ||
          (l_machine_name[0] == 0)) {
          l_machine_name = emc_machine_name_none;
      }

      if((l_machine_type == nullptr) ||
          (l_machine_type[0] == 0)) {
          l_machine_type = emc_machine_type_generic;
      }

      if(os::is_lsb) {
          l_architecture_type = emc_order_le;
      } else
          l_architecture_type = emc_order_be;

      emc_put(
          emc_tag_response, emc_response_info, SPC,
          emc_protocol_name, SPC,
          'v', emc_protocol_version, SPC,
          l_machine_name, SPC,
          l_machine_type, SPC,
          l_architecture_name, '_', l_architecture_type, SPC,
          fmt::x(m_recv_mtu),
          EOL
      );
}

void  gateway::emc_send_info_request() noexcept
{
}

void  gateway::emc_send_support_response() noexcept
{
}

void  gateway::emc_send_ping_request() noexcept
{
}

void  gateway::emc_send_pong_response() noexcept
{
}

void  gateway::emc_send_sync() noexcept
{
      emc_send_info_response();
      emc_send_support_response();
}

void  gateway::emc_send_help() noexcept
{
}

int   gateway::emc_send_error(int rc, const char*, const char*) noexcept
{
      return rc;
}

void  gateway::emc_dispatch_request(const char*, int) noexcept
{
}

void  gateway::emc_dispatch_response(const char*, int) noexcept
{
}

void  gateway::emc_dispatch_comment(const char*, int) noexcept
{
}

void  gateway::emc_dispatch_packet(const char*, int, int) noexcept
{
}

void  gateway::emc_dispatch_disconnect() noexcept
{
}

void  gateway::emc_disconnect() noexcept
{
      m_host_role = false;
      m_user_role = false;
      m_healty_bit = false;
      m_active_bit = false;
      emc_dispatch_disconnect();
      m_ping_ctr.suspend();
      m_info_ctr.suspend();
      m_drop_ctr.suspend();
      m_trip_ctr.suspend();
      m_recv_state = s_state_drop;
      m_recv_iter = 0;
      m_recv_last = 0;
      m_recv_used = 0;
      m_send_iter = 0;
      m_send_last = 0;
      m_send_used = 0;
      m_gate_name[0] = 0;
      m_gate_info[0] = 0;
}

void  gateway::feed() noexcept
{
      bool l_rewind = false;

      do {
          // shift consumed data to make sure the read buffer does not overflow unnecessarily
          if(m_recv_last > 0) {
              int l_drop_size = m_recv_last;
              int l_copy_size = m_recv_iter - m_recv_last;
              if(l_copy_size > 0) {
                  if(l_copy_size < l_drop_size) {
                      std::memcpy(m_recv_data, m_recv_data + m_recv_last, l_copy_size);
                  } else
                      std::memmove(m_recv_data, m_recv_data + m_recv_last, l_copy_size);
              }
              m_recv_used -= m_recv_last;
              m_recv_iter -= m_recv_last;
              m_recv_last -= m_recv_last;
          }
      
          // compute load size
          // the load size is limited by the maximum load size and the maximum size of the memory buffer
          int l_load_size = m_recv_size - m_recv_used;
          if(l_load_size < m_load_min) {
              l_load_size += m_load_max;
              if(m_recv_used + l_load_size > m_load_max) {
                  l_load_size = m_load_max - m_recv_used;
                  if(l_load_size == 0) {
                      m_recv_state = s_state_recover;
                  }
              }
          }

          if(m_recv_state >= s_state_accept) {
              // reserve buffer space if needed
              int l_recv_size = m_recv_used + l_load_size;
              if(l_recv_size > m_recv_size) {
                  auto l_recv_ptr = reinterpret_cast<char*>(::realloc(m_recv_data, l_recv_size));
                  if(l_recv_ptr == nullptr) {
                      m_recv_state = s_state_recover;
                      l_rewind = true;
                      continue;
                  }
                  m_recv_data = l_recv_ptr;
                  m_recv_size = l_recv_size;
                  m_mem_size  = m_recv_size + m_send_size;
              }
              // read in
              char* l_read_ptr  = m_recv_data + m_recv_used;
              int   l_read_size = m_recv_io->read(l_load_size, l_read_ptr);
              if(l_read_size > 0) {
                  m_recv_used = m_recv_used + l_read_size;
                  // received data, reset relevant timers
                  m_ping_ctr.reset();
                  m_trip_ctr.reset();
                  // seek at the last recv_iter position, record the newly read symbols and look for a message terminator
                  // - messages are unconditionally cached into the recv buffer, pre-parsed and dispatched as soon as a CR/LF
                  //   is encountered;
                  // - packets can either be dispatched as soon as they arrive or cached, depending on how the channel is set
                  //   by its governing stream;
                  // - packet data arriving on an unbound channel is immediately discarded (and hence forever lost)
                  if(m_recv_state >= s_state_accept) {
                      char*  l_feed_prev;
                      char*  l_feed_iter = m_recv_data + m_recv_iter;
                      char*  l_feed_tail = l_feed_iter + l_read_size;
                      while(l_feed_iter < l_feed_tail) {
                          char* l_feed_base = m_recv_data + m_recv_last;
                          if(m_recv_state == s_state_accept) {
                              m_recv_packet_left = 0;
                              m_recv_packet_size = 0;
                              if(*l_feed_iter > 0) {
                                  // regular message, set state to '_capture_message'
                                  m_recv_state = s_state_capture_message;
                              } else
                              if(*l_feed_iter < EOF) {
                                  // packet, set state to '_capture_packet'
                                  m_recv_state = s_state_capture_packet;
                              } else
                              if(*l_feed_iter == EOF) {
                                  // caugth EOF, ignore in this context
                                  m_recv_last = m_recv_iter + 1;
                              }
                          }
                          if(m_recv_state == s_state_capture_message) {
                              // try to cope with `\r`, `\n' and `\r\n` altogheter
                              if((*l_feed_iter == EOL) || (*l_feed_iter == RET)) {
                                  *l_feed_iter = 0;
                                  if(l_feed_iter > m_recv_data) {
                                      l_feed_prev = l_feed_iter - 1;
                                      if((*l_feed_iter == EOL) && (*l_feed_prev == RET)) {
                                          *l_feed_prev = 0;
                                      }
                                  }
                                  if(*l_feed_base == emc_tag_request) {
                                      // capture requests, if gateway is running as server
                                      if(m_host_role) {
                                          if(*l_feed_base == emc_tag_sync) {
                                              emc_send_sync();
                                          } else
                                          if(*l_feed_base == emc_tag_help) {
                                              emc_send_help();
                                          } else
                                              emc_capture_request(l_feed_base, l_feed_iter - l_feed_base);
                                      } else
                                          emc_capture_comment(l_feed_base, l_feed_iter - l_feed_base);
                                  } else
                                  if(*l_feed_base == emc_tag_response) {
                                      // capture responses, if waiting for any
                                      if(m_user_role) {
                                          emc_capture_response(l_feed_base, l_feed_iter - l_feed_base);
                                      } else
                                          emc_capture_comment(l_feed_base, l_feed_iter - l_feed_base);
                                  } else
                                      emc_capture_comment(l_feed_base, l_feed_iter - l_feed_base);
                                  m_drop_ctr.reset();
                                  m_recv_last = m_recv_iter + 1;
                                  m_msg_recv++;
                              } else
                              if((*l_feed_iter >= 0) &&
                                  (*l_feed_iter <= ASCII_MAX)) {
                              } else
                                  // non-ASCII in the middle of a message - decide what TODO later
                                  ;
                          } else
                          if(m_recv_state == s_state_capture_packet) {
                          }
                          l_feed_iter++;
                          m_recv_iter++;
                      }
                  }
                  // update statistics
                  m_chr_recv += l_read_size;
                  m_mem_used  = m_recv_used + m_send_used;
                  l_rewind    = l_read_size == l_load_size;
              }
          } else
          if(m_recv_state == s_state_recover) {
              // recover from error or unknown state:
              // - max buffer size reached
              // - buffer (re-)allocation failed
              if(m_recv_packet_size > 0) {
                  // recover from a failed packet: skip until its end
                  int l_load_size = m_recv_packet_left;
                  int l_read_size = m_recv_io->read(l_load_size);
                  if(l_read_size > 0) {
                      m_ping_ctr.reset();
                      m_trip_ctr.reset();
                      if(l_read_size == m_recv_packet_left) {
                          m_recv_last = 0;
                          m_recv_iter = 0;
                          m_recv_used = 0;
                          m_recv_packet_size = 0;
                          m_recv_state = s_state_accept;
                          m_msg_drop++;
                      }
                      m_recv_packet_left -= l_read_size;
                      m_chr_recv         += l_read_size;
                  }
              } else
              if(true) {
                  // recover from a failed message: skip until an EOL is found
                  char l_read_char = m_recv_io->get_char();
                  while(l_read_char != EOF) {
                      m_chr_recv++;
                      if((l_read_char == EOL) ||
                          (l_read_char == RET)) {
                          m_recv_last = 0;
                          m_recv_iter = 0;
                          m_recv_used = 0;
                          m_recv_state = s_state_accept;
                          m_msg_drop++;
                          break;
                      }
                      m_ping_ctr.reset();
                      m_trip_ctr.reset();
                      l_read_char = m_recv_io->get_char();
                  }
              }
              // we are already dropping a message
              m_drop_ctr.reset();
          } else
          if(m_recv_state == s_state_drop) {
              // read in data as to not overrun the FIFOs, but drop it all
              int  l_load_size = m_load_max;
              int  l_read_size = m_recv_io->read(l_load_size);
              if(l_read_size > 0) {
                  m_ping_ctr.reset();
                  m_trip_ctr.reset();
              }
              m_drop_ctr.reset();
              m_chr_recv += l_read_size;
              l_rewind    = l_read_size == l_load_size;
          }
      }
      while(l_rewind);
}

void  gateway::flush() noexcept
{
      if(m_send_last) {
          int l_save_size = m_send_last - m_send_iter;
          if(l_save_size > 0) {
              // optimisation to flush evertything in the output buffer without reparsing if its size is lower than the MTU
              // and ends with an EOL
              if(l_save_size < m_send_mtu) {
                  if(m_send_data[m_send_last - 1] == EOL) {
                      m_send_iter += m_send_io->write(l_save_size, m_send_data);
                  }
              }
              // split the output buffer in multiple MTU-sized chunks, but avoid splitting a message in the middle if at
              // all possible
              // NOTE: not yet properly implemented - not particularly necessary either
              if(m_send_iter < m_send_last) {
                  m_send_iter += m_send_io->write(l_save_size, m_send_data);
              }
          }
          if(m_send_iter == m_send_used) {
              m_send_iter = 0;
              m_send_last = 0;
              m_send_used = 0;
          }
      }
}

void  gateway::reset() noexcept
{
      emc_trip();
}

void  gateway::sync(const absolute_time_t& dt) noexcept
{
      m_ping_ctr.update(dt);
      if(m_ping_ctr.test(m_gate_ping_time)) {
          emc_send_ping_request();
          m_ping_ctr.reset();
      }
      m_info_ctr.update(dt);
      if(m_info_ctr.test(m_gate_info_time)) {
          emc_send_info_request();
      }
      // update the drop timer and check if it is time to drop the incomplete message in the
      // queue, if present
      m_drop_ctr.update(dt);
      if(m_drop_ctr.test(m_gate_drop_time)) {
          emc_drop();
          m_drop_ctr.reset();
      }
      // update the trip timer
      m_trip_ctr.update(dt);
      if(m_trip_ctr.test(m_gate_trip_time)) {
          m_active_bit = false;
          m_healty_bit = false;
          emc_trip();
          m_trip_ctr.reset();
      }
}

/*namespace emc*/ }
