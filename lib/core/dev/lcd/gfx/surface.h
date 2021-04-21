#ifndef dev_gfx_surface_h
#define dev_gfx_surface_h
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
#include "gfx.h"
#include "dc.h"
#include "metrics.h"

namespace dev {
namespace gfx {

class surface
{
  std::uint8_t m_type;        // surface type: one of the type_* flags below
  std::uint8_t m_attributes;  // attributes: options that hardly ever change during the lifetime of the objects (sa_* flags)
  std::uint8_t m_flags;       // flags: dynamic, user specified options (sf_* flags)
  surface*     p_next;

  protected:
  friend class scene;

  public:
  static constexpr std::uint8_t type_undef  = 0u;
  static constexpr std::uint8_t type_colour = 1u;
  static constexpr std::uint8_t type_canvas = 2u;
  static constexpr std::uint8_t type_sprite = 3u;

  static constexpr std::uint8_t sa_none = 0u;
  static constexpr std::uint8_t sa_opaque = 1u;     // draw nothing under the object
  static constexpr std::uint8_t sa_default = sa_opaque;
  static constexpr std::uint8_t sa_auto = sa_default;

  static constexpr std::uint8_t sf_none = 0u;
  static constexpr std::uint8_t sf_visible = 128u;
  static constexpr std::uint8_t sf_default = sf_visible;
  static constexpr std::uint8_t sf_auto = sf_default;

  public:
          surface(std::uint8_t) noexcept;
          surface(const surface&) noexcept = delete;
          surface(surface&&) noexcept = delete;
  virtual ~surface();

  inline  std::uint8_t get_type() const noexcept {
          return m_type;
  }

  inline  bool has_attributes(unsigned int value) const noexcept {
          return m_attributes & value;
  }

  inline  std::uint8_t get_attributes() const noexcept {
          return m_attributes;
  }

  inline  bool has_flags(unsigned int value) const noexcept {
          return m_flags & value;
  }

  inline  std::uint8_t get_flags() const noexcept {
          return m_flags;
  }

  virtual gfx::box2d_t get_geometry() const noexcept = 0;
  virtual bool         sync(float) noexcept = 0;

          surface&  operator=(const surface&) noexcept = delete;
          surface&  operator=(surface&&) noexcept = delete;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
