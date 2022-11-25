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
#include "surface.h"
#include "tile/cbo.h"
#include "tile/cmo.h"
#include "tile/cso.h"

namespace gfx {

      surface::surface(bool o_allow_attach) noexcept:
      dc(),
      m_manager(nullptr),
      m_mid(nullptr),
      m_attach_prev(nullptr),
      m_attach_next(nullptr),
      m_allow_attach(o_allow_attach),
      m_attach_head(nullptr),
      m_attach_tail(nullptr)
{
}

      surface::surface(surface* manager, bool o_allow_attach) noexcept:
      surface(o_allow_attach)
{
      manager->attach(this);
}

      surface::~surface()
{
      if(m_manager) {
          m_manager->detach(this);
      }
}

bool  surface::gfx_prepare(device*) noexcept
{
      return true;
}

bool  surface::gfx_attach_surface(surface*) noexcept
{
      return true;
}

bool  surface::gfx_detach_surface(surface*) noexcept
{
      return true;
}

void  surface::gfx_draw() noexcept
{
}


void  surface::gfx_sync(int) noexcept
{
}

bool  surface::gfx_release(device*) noexcept
{
      return true;
}

bool  surface::attach(surface* surface_ptr) noexcept
{
      bool l_allow_attach = false;
      if(m_allow_attach) {
          if((surface_ptr != this) &&
              (surface_ptr != nullptr)) {
              if(surface_ptr->m_manager == nullptr) {
                  l_allow_attach = gfx_attach_surface(surface_ptr);
                  if(l_allow_attach) {
                      surface_ptr->m_manager = this;
                      if(m_attach_tail) {
                          m_attach_tail->m_attach_next = surface_ptr;
                      } else
                          m_attach_head = surface_ptr;
                      m_attach_tail = surface_ptr;
                  }
              }
          }
      }
      return l_allow_attach;
}

bool  surface::detach(surface* surface_ptr) noexcept
{
      bool l_allow_detach = false;
      if(surface_ptr != nullptr) {
          if(surface_ptr->m_manager == this) {
              l_allow_detach = gfx_detach_surface(surface_ptr);
              if(l_allow_detach) {
                  if(surface_ptr->m_attach_prev) {
                      surface_ptr->m_attach_prev->m_attach_next = surface_ptr->m_attach_next;
                  } else
                      m_attach_head = surface_ptr->m_attach_prev;
                  if(surface_ptr->m_attach_next) {
                      surface_ptr->m_attach_next->m_attach_prev = surface_ptr->m_attach_prev;
                  } else
                      m_attach_tail = surface_ptr->m_attach_prev;
                  surface_ptr->m_manager      = nullptr;
                  surface_ptr->m_attach_prev  = nullptr;
                  surface_ptr->m_attach_next  = nullptr;
              }
          }
      }
      return l_allow_detach;
}

/*namespace gfx*/ }
