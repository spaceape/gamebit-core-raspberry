#ifndef memory_allocator_h
#define memory_allocator_h
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
#include <memory.h>
#include <limits>

template<class Tp = std::uint8_t>
class polymorphic_allocator
{
  resource* m_resource;

  public:
  using value_type = Tp;

  public:
  inline  polymorphic_allocator() noexcept:
          m_resource(get_default_resource()) {
  }

  inline  polymorphic_allocator(resource* r) noexcept:
          m_resource(r) {
  }

  inline  polymorphic_allocator(const polymorphic_allocator& copy) noexcept = delete;
 
  template<class U>
  inline  polymorphic_allocator(const polymorphic_allocator<U>& copy) noexcept:
          m_resource(copy.m_resource) {
  }
 
  inline  polymorphic_allocator& operator=(const polymorphic_allocator&) = delete;
 
  inline  Tp*   allocate(std::size_t n) noexcept {
          if(n <= (std::numeric_limits<std::size_t>::max() / sizeof(Tp))) {
              return static_cast<Tp*>(m_resource->allocate(n * sizeof(Tp), alignof(Tp)));
          }
          return nullptr;
  }

  inline  void  deallocate(Tp* p, std::size_t n) noexcept {
          m_resource->deallocate(p, n * sizeof(Tp), alignof(Tp));
  }
 
  inline  void* allocate_bytes(std::size_t nbytes, std::size_t alignment = alignof(max_align_t)) noexcept {
          return m_resource->allocate(nbytes, alignment);
  }

  inline  void  deallocate_bytes(void* p, std::size_t nbytes, std::size_t alignment = alignof(max_align_t)) {
          m_resource->deallocate(p, nbytes, alignment);
  }

  template<class T>
  inline  T*    allocate_object(std::size_t n = 1) noexcept {
          if((std::numeric_limits<size_t>::max() / sizeof(T)) >= n) {
              return static_cast<T*>(allocate_bytes(n * sizeof(T), alignof(T)));
          }
          return nullptr;
  }

  template<class T>
  inline  void  deallocate_object(T* p, size_t n = 1) noexcept {
          deallocate_bytes(p, n * sizeof(T), alignof(T));
  }

  template<class T, class... Args>
  inline  T*    new_object(Args&&... args) noexcept {
          T* p = allocate_object<T>();
          if(p) {
              construct(p, std::forward<Args>(args)...);
          }
          return p;
  }

  template<class T>
  inline  void  delete_object(T* p) noexcept {
          destroy(p);
          deallocate_object(p);
  }

  template<class T, class... Args>
  inline  void  construct(T* p, Args&&... args) noexcept {
          new(p) T(std::forward<Args>(args)...);
  }

  template<class T>
  inline  void  destroy(T* p) noexcept {
          p->~T();
  }

  polymorphic_allocator select_on_container_copy_construction() const {
          return polymorphic_allocator();
  }
 
  inline  resource* get_resource() const noexcept {
          return m_resource;
  }
};

// #if __cplusplus == 201703L
//   template<typename _Tp>
//     class polymorphic_allocator;
// #else // C++20
// # define __cpp_lib_polymorphic_allocator 201902L
//   template<typename _Tp = std::byte>
//     class polymorphic_allocator;
// #endif

//   // Global memory resources
//   memory_resource* new_delete_resource() noexcept;
//   memory_resource* null_memory_resource() noexcept;
//   memory_resource* set_default_resource(memory_resource* __r) noexcept;
//   memory_resource* get_default_resource() noexcept
//     __attribute__((__returns_nonnull__));

//   // Pool resource classes
//   struct pool_options;
// #ifdef _GLIBCXX_HAS_GTHREADS
//   class synchronized_pool_resource;
// #endif
//   class unsynchronized_pool_resource;
//   class monotonic_buffer_resource;


//   // C++17 23.12.3 Class template polymorphic_allocator
//   template<typename _Tp>
//     class polymorphic_allocator
//     {
//       // _GLIBCXX_RESOLVE_LIB_DEFECTS
//       // 2975. Missing case for pair construction in polymorphic allocators
//       template<typename _Up>
// 	struct __not_pair { using type = void; };

//       template<typename _Up1, typename _Up2>
// 	struct __not_pair<pair<_Up1, _Up2>> { };

//     public:
//       using value_type = _Tp;

//       polymorphic_allocator() noexcept
//       : _M_resource(get_default_resource())
//       { }

//       polymorphic_allocator(memory_resource* __r) noexcept
//       __attribute__((__nonnull__))
//       : _M_resource(__r)
//       { _GLIBCXX_DEBUG_ASSERT(__r); }

//       polymorphic_allocator(const polymorphic_allocator& __other) = default;

//       template<typename _Up>
// 	polymorphic_allocator(const polymorphic_allocator<_Up>& __x) noexcept
// 	: _M_resource(__x.resource())
// 	{ }

//       polymorphic_allocator&
//       operator=(const polymorphic_allocator&) = delete;

//       [[nodiscard]]
//       _Tp*
//       allocate(size_t __n)
//       __attribute__((__returns_nonnull__))
//       {
// 	if (__n > (numeric_limits<size_t>::max() / sizeof(_Tp)))
// 	  std::__throw_bad_alloc();
// 	return static_cast<_Tp*>(_M_resource->allocate(__n * sizeof(_Tp),
// 						       alignof(_Tp)));
//       }

//       void
//       deallocate(_Tp* __p, size_t __n) noexcept
//       __attribute__((__nonnull__))
//       { _M_resource->deallocate(__p, __n * sizeof(_Tp), alignof(_Tp)); }

// #if __cplusplus > 201703L
//       void*
//       allocate_bytes(size_t __nbytes,
// 		     size_t __alignment = alignof(max_align_t))
//       { return _M_resource->allocate(__nbytes, __alignment); }

//       void
//       deallocate_bytes(void* __p, size_t __nbytes,
// 		       size_t __alignment = alignof(max_align_t))
//       { _M_resource->deallocate(__p, __nbytes, __alignment); }

//       template<typename _Up>
// 	_Up*
// 	allocate_object(size_t __n = 1)
// 	{
// 	  if ((std::numeric_limits<size_t>::max() / sizeof(_Up)) < __n)
// 	    __throw_length_error("polymorphic_allocator::allocate_object");
// 	  return static_cast<_Up*>(allocate_bytes(__n * sizeof(_Up),
// 						  alignof(_Up)));
// 	}

//       template<typename _Up>
// 	void
// 	deallocate_object(_Up* __p, size_t __n = 1)
// 	{ deallocate_bytes(__p, __n * sizeof(_Up), alignof(_Up)); }

//       template<typename _Up, typename... _CtorArgs>
// 	_Up*
// 	new_object(_CtorArgs&&... __ctor_args)
// 	{
// 	  _Up* __p = allocate_object<_Up>();
// 	  __try
// 	    {
// 	      construct(__p, std::forward<_CtorArgs>(__ctor_args)...);
// 	    }
// 	  __catch (...)
// 	    {
// 	      deallocate_object(__p);
// 	      __throw_exception_again;
// 	    }
// 	  return __p;
// 	}

//       template<typename _Up>
// 	void
// 	delete_object(_Up* __p)
// 	{
// 	  destroy(__p);
// 	  deallocate_object(__p);
// 	}
// #endif // C++2a

// #if __cplusplus == 201703L
//       template<typename _Tp1, typename... _Args>
// 	__attribute__((__nonnull__))
// 	typename __not_pair<_Tp1>::type
// 	construct(_Tp1* __p, _Args&&... __args)
// 	{
// 	  // _GLIBCXX_RESOLVE_LIB_DEFECTS
// 	  // 2969. polymorphic_allocator::construct() shouldn't pass resource()
// 	  using __use_tag
// 	    = std::__uses_alloc_t<_Tp1, polymorphic_allocator, _Args...>;
// 	  if constexpr (is_base_of_v<__uses_alloc0, __use_tag>)
// 	    ::new(__p) _Tp1(std::forward<_Args>(__args)...);
// 	  else if constexpr (is_base_of_v<__uses_alloc1_, __use_tag>)
// 	    ::new(__p) _Tp1(allocator_arg, *this,
// 			    std::forward<_Args>(__args)...);
// 	  else
// 	    ::new(__p) _Tp1(std::forward<_Args>(__args)..., *this);
// 	}

//       template<typename _Tp1, typename _Tp2,
// 	       typename... _Args1, typename... _Args2>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(pair<_Tp1, _Tp2>* __p, piecewise_construct_t,
// 		  tuple<_Args1...> __x, tuple<_Args2...> __y)
// 	{
// 	  auto __x_tag =
// 	    __use_alloc<_Tp1, polymorphic_allocator, _Args1...>(*this);
// 	  auto __y_tag =
// 	    __use_alloc<_Tp2, polymorphic_allocator, _Args2...>(*this);
// 	  index_sequence_for<_Args1...> __x_i;
// 	  index_sequence_for<_Args2...> __y_i;

// 	  ::new(__p) pair<_Tp1, _Tp2>(piecewise_construct,
// 				      _S_construct_p(__x_tag, __x_i, __x),
// 				      _S_construct_p(__y_tag, __y_i, __y));
// 	}

//       template<typename _Tp1, typename _Tp2>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(pair<_Tp1, _Tp2>* __p)
// 	{ this->construct(__p, piecewise_construct, tuple<>(), tuple<>()); }

//       template<typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(pair<_Tp1, _Tp2>* __p, _Up&& __x, _Vp&& __y)
// 	{
// 	  this->construct(__p, piecewise_construct,
// 			  forward_as_tuple(std::forward<_Up>(__x)),
// 			  forward_as_tuple(std::forward<_Vp>(__y)));
// 	}

//       template <typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(pair<_Tp1, _Tp2>* __p, const std::pair<_Up, _Vp>& __pr)
// 	{
// 	  this->construct(__p, piecewise_construct,
// 			  forward_as_tuple(__pr.first),
// 			  forward_as_tuple(__pr.second));
// 	}

//       template<typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(pair<_Tp1, _Tp2>* __p, pair<_Up, _Vp>&& __pr)
// 	{
// 	  this->construct(__p, piecewise_construct,
// 			  forward_as_tuple(std::forward<_Up>(__pr.first)),
// 			  forward_as_tuple(std::forward<_Vp>(__pr.second)));
// 	}
// #else
//       template<typename _Tp1, typename... _Args>
// 	__attribute__((__nonnull__))
// 	void
// 	construct(_Tp1* __p, _Args&&... __args)
// 	{
// 	  std::uninitialized_construct_using_allocator(__p, *this,
// 	      std::forward<_Args>(__args)...);
// 	}
// #endif

//       template<typename _Up>
// 	__attribute__((__nonnull__))
// 	void
// 	destroy(_Up* __p)
// 	{ __p->~_Up(); }

//       polymorphic_allocator
//       select_on_container_copy_construction() const noexcept
//       { return polymorphic_allocator(); }

//       memory_resource*
//       resource() const noexcept
//       __attribute__((__returns_nonnull__))
//       { return _M_resource; }

//     private:
//       using __uses_alloc1_ = __uses_alloc1<polymorphic_allocator>;
//       using __uses_alloc2_ = __uses_alloc2<polymorphic_allocator>;

//       template<typename _Ind, typename... _Args>
// 	static tuple<_Args&&...>
// 	_S_construct_p(__uses_alloc0, _Ind, tuple<_Args...>& __t)
// 	{ return std::move(__t); }

//       template<size_t... _Ind, typename... _Args>
// 	static tuple<allocator_arg_t, polymorphic_allocator, _Args&&...>
// 	_S_construct_p(__uses_alloc1_ __ua, index_sequence<_Ind...>,
// 		       tuple<_Args...>& __t)
// 	{
// 	  return {
// 	      allocator_arg, *__ua._M_a, std::get<_Ind>(std::move(__t))...
// 	  };
// 	}

//       template<size_t... _Ind, typename... _Args>
// 	static tuple<_Args&&..., polymorphic_allocator>
// 	_S_construct_p(__uses_alloc2_ __ua, index_sequence<_Ind...>,
// 		       tuple<_Args...>& __t)
// 	{ return { std::get<_Ind>(std::move(__t))..., *__ua._M_a }; }

//       memory_resource* _M_resource;
//     };


#endif