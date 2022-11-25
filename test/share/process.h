#ifndef test_process_h
#define test_process_h
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
#include <uld.h>
#include <gfx.h>
#include <uld/target.h>
#include <uld/image.h>
#include <gfx/device.h>
#include <gfx/surface.h>

/* process
   base class of a guest process with access to graphics
*/
class process: public gfx::surface
{
  uld::target*  m_target;
  uld::image    m_image;
  gfx::device*  m_display_ptr;
  short int     m_display_sx;
  short int     m_display_sy;

  protected:
  virtual bool  gfx_prepare(gfx::device*) noexcept override;
  virtual void  gfx_draw() noexcept;
  virtual void  gfx_sync(int) noexcept;
  virtual bool  gfx_release(gfx::device*) noexcept override;

  public:
          process(uld::target*, gfx::device*, const char*) noexcept;
          process(const process&) noexcept = delete;
          process(process&&) noexcept = delete;
  virtual ~process();

          process& operator=(const process&) noexcept = delete;
          process& operator=(process&&) noexcept = delete;
};
#endif
