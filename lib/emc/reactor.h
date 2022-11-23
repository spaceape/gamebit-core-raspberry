#ifndef emc_reactor_h
#define emc_reactor_h
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
#include "session.h"

namespace emc {

/* reactor
*/
class reactor
{
  ios*          m_io;
  session*      m_session_ptr;
  char          m_session_data[sizeof(session)];
  public:
          reactor(ios*) noexcept;
          reactor(const reactor&) noexcept = delete;
          reactor(reactor&&) noexcept = delete;
  virtual ~reactor();
          bool     resume() noexcept;
  virtual const char* get_machine_name() const noexcept;
  virtual const char* get_machine_type() const noexcept;
          bool     suspend() noexcept;
          void     sync(const absolute_time_t&) noexcept;
          reactor& operator=(const reactor&) noexcept = delete;
          reactor& operator=(reactor&&) noexcept = delete;
};

/*namespace emc*/ }
#endif
