#ifndef core_soft_dpu_h
#define core_soft_dpu_h
/** 
    Copyright (c) 2018, wicked systems
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
#include <cstdlib>
#include <cstring>
#include <cmath>

#define _dptype int32_t
#define _dpts 4

namespace dpu {

inline  void  nop() noexcept {
}

inline  void  reg_clr(_dptype* r) noexcept {
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_mov(_dptype* r, _dptype imm) noexcept {
        r[0] = imm;
        r[1] = imm;
        r[2] = imm;
        r[3] = imm;
}

inline  void  reg_mov(_dptype* r, const _dptype* s) noexcept {
        r[0] = s[0];
        r[1] = s[1];
        r[2] = s[2];
        r[3] = s[3];
}

inline  void  reg_mov(_dptype* r, _dptype imm0, _dptype imm1, _dptype imm2, _dptype imm3) noexcept {
        r[0] = imm0;
        r[1] = imm1;
        r[2] = imm2;
        r[3] = imm3;
}

inline  void  reg_shl(_dptype* r) noexcept {
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
}

inline  void  reg_shr(_dptype* r) noexcept {
        r[3] = r[2];
        r[2] = r[1];
        r[1] = r[0];
}

inline  void  reg_neg(_dptype* r) noexcept {
        r[0] =-r[0];
        r[1] =-r[1];
        r[2] =-r[2];
        r[3] =-r[3];
}

inline  void  reg_inv(_dptype* r) noexcept {
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_add(_dptype* r, const _dptype imm) noexcept {
        r[0] += imm;
        r[1] += imm;
        r[2] += imm;
        r[3] += imm;
}

inline  void  reg_add(_dptype* r, const _dptype* s) noexcept {
        r[0] += s[0];
        r[1] += s[1];
        r[2] += s[2];
        r[3] += s[3];
}

inline  void  reg_sub(_dptype* r, const _dptype imm) noexcept {
        r[0] -= imm;
        r[1] -= imm;
        r[2] -= imm;
        r[3] -= imm;
}

inline  void  reg_sub(_dptype* r, const _dptype* s) noexcept {
        r[0] -= s[0];
        r[1] -= s[1];
        r[2] -= s[2];
        r[3] -= s[3];
}

inline  void  reg_mul(_dptype* r, const _dptype imm) noexcept {
        r[0] *= imm;
        r[1] *= imm;
        r[2] *= imm;
        r[3] *= imm;
}

inline  void  reg_mul(_dptype* r, const _dptype* s) noexcept {
        r[0] *= s[0];
        r[1] *= s[1];
        r[2] *= s[2];
        r[3] *= s[3];
}

inline  void  reg_div(_dptype* r, const _dptype imm) noexcept {
        r[0] /= imm;
        r[1] /= imm;
        r[2] /= imm;
        r[3] /= imm;
}

inline  void  reg_div(_dptype* r, const _dptype* s) noexcept {
        r[0] /= s[0];
        r[1] /= s[1];
        r[2] /= s[2];
        r[3] /= s[3];
}

inline  void  vec_mov(_dptype* p0, _dptype* p1, const _dptype imm) noexcept {
        while(p0 < p1) {
            reg_mov(p0, imm);
            p0 +=_dpts;
        }
}

inline  size_t vec_mov(_dptype* p0, _dptype* p1, _dptype* s0, _dptype* s1) noexcept {
        size_t dst_size = p1 - p0;
        size_t src_size = s1 - s0;
        if(dst_size > src_size) {
            dst_size = src_size;
        }
        if(dst_size) {
            std::memcpy(p0, s0, dst_size * sizeof(_dptype));
        }
        return dst_size;
}

inline  size_t vec_mov_prescaled(_dptype* p0, _dptype* p1, _dptype* s0, _dptype* s1, const _dptype* scale) noexcept {
        size_t dst_size;
        size_t src_size = s1 - s0;
        if(src_size) {
            dst_size = p1 - p0;
            if(src_size == dst_size) {
                _dptype* ic  = s0; /*copy pointer*/
                _dptype* i0  = p0;
                _dptype* i1;
                size_t  pts = src_size /_dpts;
                while(i0 < p1) {
                    i1 = i0 + pts;
                    while(i0 < i1) {
                      *i0 =(*ic) * (*scale);
                        ic++;
                        i0++;
                    }
                    scale++;
                }
            }
            return dst_size;
        }
        return 0;
}

inline  void vec_clr(_dptype* p0, _dptype* p1) noexcept {
        size_t dst_size = p1 - p0;
        if(dst_size) {
            std::memset(p0, 0, dst_size * sizeof(_dptype));
        }
}

inline  bool equ(_dptype lhs, _dptype rhs) noexcept {
        _dptype error = 0;
        if(lhs == rhs) {
            return true;
        }
        if(std::abs(rhs) > std::abs(lhs)) {
            error = std::abs((lhs - rhs) / rhs);
        } else
            error = std::abs((lhs - rhs) / lhs);
        return error <=_epsilon;
}

inline  bool equ(const _dptype* r, _dptype imm) noexcept {
        for(auto x = 0; x !=_dpts; x++) {
            if(equ(r[x], imm) == false) {
                return false;
            }
        }
        return true;
}

inline  bool equ(const _dptype* r, const _dptype* s) noexcept {
        for(auto x = 0; x !=_dpts; x++) {
            if(equ(r[x], s[x]) == false) {
                return false;
            }
        }
        return true;
}

inline  bool   equ_0(const _dptype* r) noexcept {
        return equ(r, 0);
}

inline  bool   equ_1(const _dptype* r) noexcept {
        return equ(r, 1);
}
/*namespace dpu*/ }
#endif
