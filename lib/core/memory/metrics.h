#ifndef memory_metrics_h
#define memory_metrics_h
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
#include <global.h>
#include "manager/heap.h"
#include "manager/shared.h"
#include <traits.h>

namespace memory {

/* has_alloc_decl<Rt>
   determine if <Rt> has a numeric "alloc_bytes" member
*/
struct has_alloc_impl {
  template<typename Rt>
  static constexpr auto test(int) noexcept -> decltype(std::declval<Rt>().alloc_bytes);
  template<typename Rt>
  static constexpr auto test(...) noexcept -> std::false_type;
};

template<typename Rt>
struct has_alloc_decl: std::is_integral<decltype(has_alloc_impl::test<Rt>(0))>::type {
};

/* has_fixed_decl<Rt>
   determine if <Rt> has a numeric "fixed_bytes" member
*/
struct has_fixed_impl {
  template<typename Rt>
  static constexpr auto test(int) noexcept -> decltype(std::declval<Rt>().fixed_bytes);
  template<typename Rt>
  static constexpr auto test(...) noexcept -> std::false_type;
};

template<typename Rt>
struct has_fixed_decl: std::is_integral<decltype(has_fixed_impl::test<Rt>(0))>::type {
};

/* has_realloc_decl<Rt, Args...>
   determine if <Rt> has a member function called reallocate, invokable with <Args...>
*/
struct has_realloc_impl
{
  class none_type
  {
  };

  template<typename Rt, typename... Args>
  static constexpr auto test(int) -> 
      decltype(std::declval<Rt>().reallocate(
              std::declval<void*>(),
              std::declval<std::size_t>(),
              std::declval<std::size_t>(),
              std::declval<std::size_t>(),
              std::declval<Args>()...
          )
      );
  template<typename Rt, typename... Args>
  static constexpr auto test(...) -> none_type;
};

template<typename Rt, typename... Args>
struct has_realloc_decl: std::conditional<
        std::is_same<has_realloc_impl::none_type, decltype(has_realloc_impl::test<Rt, Args&&...>(0))>::value,
        std::false_type, 
        std::true_type
    >::type
{
};

/* get_alloc_size()
   returns the number of elements of type <Xt> that an allocator of type <RT> is comfortable
   growing with, as most allocators don't like reserving just a few bytes at a time;
*/
template<typename Rt, typename Xt>
constexpr auto get_alloc_size() noexcept -> std::size_t
{
    if constexpr (has_alloc_decl<Rt>::value) {
        if constexpr (Rt::alloc_bytes > sizeof(Xt)) {
            if constexpr (Rt::alloc_bytes % global::cache_large_max) {
                return  Rt::alloc_bytes - (Rt::alloc_bytes % global::cache_large_max) + global::cache_large_max;
            }
            return Rt::alloc_bytes / sizeof(Xt);
        }
    }
    return global::cache_small_max * sizeof(Xt);
}

/* get_alloc_size()
   round the given number of elements to a value that an allocator of type <Rt> can provide,
   based on its characteristics or allocation policy.
*/
template<typename Rt, typename Xt>
constexpr auto get_alloc_size(std::size_t size) noexcept -> std::size_t
{
    return get_round_value(size, get_alloc_size<Rt, Xt>());
}

/* get_fixed_size()
   if <Rt> is a fixed allocator, return the number of elements of type <Xt> it holds
*/
template<typename Rt, typename Xt>
constexpr auto get_fixed_size() noexcept -> std::size_t
{
    if constexpr (has_fixed_decl<Rt>::value) {
        return  Rt::fixed_bytes / sizeof(Xt);
    }
    return 0u;
}

/* get_fixed_size()
   determine if a memory resource of type <Rt> is resizable
*/
template<typename Rt, typename... Args>
constexpr bool get_resizable() noexcept
{
    return has_realloc_decl<Rt, Args...>::value;
}

/*namespace memory*/ }
#endif
