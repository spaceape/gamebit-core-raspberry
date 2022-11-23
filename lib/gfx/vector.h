#ifndef gfx_vector_h
#define gfx_vector_h
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
#include <limits>

namespace gfx {

/* vector format flag
 * [ V - - -  - - - - ]
*/

constexpr std::uint8_t fmt_vector = 0b10000000;

struct point_t
{
  float x;
  float y;

  public:
  inline  point_t() noexcept {
  }

  inline  point_t(float px, float py) noexcept:
          x(px),
          y(py) {
  }

  inline  point_t(const point_t& copy) noexcept:
          x(copy.x),
          y(copy.y) {
  }

  inline  point_t(point_t&& copy) noexcept:
          x(copy.x),
          y(copy.y) {
  }

  inline  ~point_t() {
  }
  
  static  float    get_dx(const point_t& lhs, const point_t& rhs) noexcept {
          return rhs.x - lhs.x;
  }

  static  float    get_dy(const point_t& lhs, const point_t& rhs) noexcept {
          return rhs.y - lhs.y;
  }
  
  static  float    get_slope(const point_t& lhs, const point_t& rhs) noexcept {
          float l_dx = get_dx(lhs, rhs);
          if(l_dx != 0.0f) {
              return get_dy(lhs, rhs) / l_dx;
          } else
              return std::numeric_limits<float>::infinity();
  }
  
  static  float    get_slope(float px0, float py0, float px1, float py1) noexcept {
          float l_dx = px1 - px0;
          if(l_dx != 0.0f) {
              return (py1 - py0) / l_dx;
          } else
          if(py1 > py0) {
              return std::numeric_limits<float>::infinity();
          } else
              return -1.0f * std::numeric_limits<float>::infinity();
  }

  inline  point_t& operator=(const point_t& rhs) noexcept {
          x = rhs.x;
          y = rhs.y;
          return *this;
  }

  inline  point_t& operator=(point_t&& rhs) noexcept {
          x = rhs.x;
          y = rhs.y;
          return *this;
  }
};

struct edge_t
{
  point_t p0;
  point_t p1;

  public:
  inline  edge_t() noexcept {
  }

  inline  edge_t(const point_t& cp0, const point_t& cp1) noexcept:
          p0(cp0),
          p1(cp1) {
  }
  
  inline  edge_t(const edge_t& copy) noexcept:
          p0(copy.p0),
          p1(copy.p1) {
  }
  
  inline  edge_t(edge_t&& copy) noexcept:
          p0(std::move(copy.p0)),
          p1(std::move(copy.p1)) {
  }
  
  inline  ~edge_t() {
  }

  inline  point_t& get_p0() noexcept {
          return p0;
  }

  inline  point_t& get_p1() noexcept {
          return p1;
  }

  inline  float    get_slope() noexcept {
          float l_dx = p1.x - p0.x;
          if(l_dx > 0.0f) {
              return (p1.y - p0.y) / l_dx;
          }
          return std::numeric_limits<float>::infinity();
  }
  
  inline  edge_t& operator=(const edge_t& rhs) noexcept {
          p0 = rhs.p0;
          p1 = rhs.p1;
          return *this;
  }

  inline  edge_t& operator=(edge_t&& rhs) noexcept {
          p0 = std::move(rhs.p0);
          p1 = std::move(rhs.p1);
          return *this;
  }
};

/*namespace gfx*/ }
#endif
