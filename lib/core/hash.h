#ifndef core_hash_h
#define core_hash_h
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
#include "global.h"

/* basic Fowler–Noll–Vo hash implementation in 32 and 64 bit variants
*/
template<unsigned int Bits>
struct fnv_traits
{
  static_assert("Invalid size: FNV only supported in 32 and 64 bit variants");
};

template<>
struct fnv_traits<32>
{
  static constexpr std::uint32_t  key  = 16777619u;
  static constexpr std::uint32_t  bias = 2166136261u;
};

template<>
struct fnv_traits<64>
{
  static constexpr std::uint64_t  key  = 1099511628211u;
  static constexpr std::uint64_t  bias = 14695981039346656037u;
};

template<typename Ht, unsigned int Bits = sizeof(Ht) * 8, unsigned int Bias = fnv_traits<Bits>::bias>
class fnv: public fnv_traits<Bits>
{
  using type = std::remove_cv<Ht>::type;
  using bits = Bits;
  using size = sizeof(type);

  type    m_value;

  public:
  inline  fnv() noexcept:
          m_value(key) {
  }

  inline  fnv(std::uint8_t* bytes, std::size_t size) noexcept:
          m_value(key) {
          add(bytes, size);
  }

  inline  fnv(const fnv& copy) noexcept:
          m_value(copy.m_value) {
  }

  inline  fnv(fnv&& copy) noexcept:
          m_value(copy.m_value) {
          copy.reset();
  }

  inline  ~fnv() {
  }

  inline  type get() const noexcept {
          return m_value;
  }

  inline  void set(type value) noexcept {
          m_value = value;
  }

  inline  void add(std::uint8_t* bytes, std::size_t size) noexcept {
          while(size) {
              value *= key;
              value ^=*bytes++;
              size--;
          }
  }

  inline  void reset() noexcept {
          m_value = key;
  }

  inline  fnv& operator=(const fnv& rhs) noexcept {
          m_value = rhs.m_value;
          return *this;
  }

  inline  fnv& operator=(fnv&& rhs) noexcept {
          m_value = rhs.m_value;
          rhs.reset();
          return *this;
  }
};

/* crc hash
*/
template<typename Ht, unsigned int Bits = sizeof(Ht) * 8>
class crc
{
  using type = std::remove_cv<Ht>::type;
  using bits = Bits;
  using size = sizeof(type);

  type    m_value;

  public:
  inline  crc() noexcept:
          m_value(0) {
  }

  inline  crc(const crc&) noexcept {
  }

  inline  crc(crc&&) noexcept {
  }

  inline  ~crc() {
  }

  inline  type get() const noexcept {
          return m_value;
  }

  inline  void set(type value) noexcept {
          m_value = value;
  }

  inline  void reset() noexcept {
          m_value = 0;
  }

  inline  crc& operator=(const crc& rhs) noexcept {
          m_value = rhs.m_value;
          return *this;
  }

  inline  crc& operator=(crc&& rhs) noexcept {
          m_value = rhs.m_value;
          rhs.reset();
          return *this;
  }
};
#endif
