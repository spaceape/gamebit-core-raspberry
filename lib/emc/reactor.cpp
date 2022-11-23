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
#include "reactor.h"

namespace emc {

      reactor::reactor(ios* io) noexcept:
      m_io(io),
      m_session_ptr(nullptr)
{
}

      reactor::~reactor()
{
      suspend();
}

bool  reactor::resume() noexcept
{
      if(m_session_ptr == nullptr) {
          m_session_ptr = new(m_session_data) session(this, m_io);
      }
      return m_session_ptr != nullptr;
}

const char* reactor::get_machine_name() const noexcept
{
      return emc_machine_name_none;
}

const char* reactor::get_machine_type() const noexcept
{
      return emc_machine_type_generic;
}

bool  reactor::suspend() noexcept
{
      if(m_session_ptr != nullptr) {
          m_session_ptr->~session();
          m_session_ptr = nullptr;
      }
      return true;
}

void  reactor::sync(const absolute_time_t& time) noexcept
{
      if(m_session_ptr != nullptr) {
          m_session_ptr->feed();
          m_session_ptr->sync(time);
      }
}

/*namespace emc*/ }
