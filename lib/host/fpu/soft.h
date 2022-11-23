#ifndef core_soft_fpu_h
#define core_soft_fpu_h
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

#define _fptype float
#define _fpts 4

namespace fpu {

inline  void  nop() noexcept {
}

inline  void  reg_clr(_fptype* r) noexcept {
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
}

inline  void  reg_mov(_fptype* r, _fptype imm) noexcept {
        r[0] = imm;
        r[1] = imm;
        r[2] = imm;
        r[3] = imm;
}

inline  void  reg_mov(_fptype* r, const _fptype* s) noexcept {
        r[0] = s[0];
        r[1] = s[1];
        r[2] = s[2];
        r[3] = s[3];
}

inline  void  reg_mov(_fptype* r, _fptype imm0, _fptype imm1, _fptype imm2, _fptype imm3) noexcept {
        r[0] = imm0;
        r[1] = imm1;
        r[2] = imm2;
        r[3] = imm3;
}

inline  void  reg_shl(_fptype* r) noexcept {
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
}

inline  void  reg_shr(_fptype* r) noexcept {
        r[3] = r[2];
        r[2] = r[1];
        r[1] = r[0];
}

inline  void  reg_neg(_fptype* r) noexcept {
        r[0] =-r[0];
        r[1] =-r[1];
        r[2] =-r[2];
        r[3] =-r[3];
}

inline  void  reg_inv(_fptype* r) noexcept {
        r[0] = 1.0 / r[0];
        r[1] = 1.0 / r[1];
        r[2] = 1.0 / r[2];
        r[3] = 1.0 / r[3];
}

inline  void  reg_add(_fptype* r, const _fptype imm) noexcept {
        r[0] += imm;
        r[1] += imm;
        r[2] += imm;
        r[3] += imm;
}

inline  void  reg_add(_fptype* r, const _fptype* s) noexcept {
        r[0] += s[0];
        r[1] += s[1];
        r[2] += s[2];
        r[3] += s[3];
}

inline  void  reg_sub(_fptype* r, const _fptype imm) noexcept {
        r[0] -= imm;
        r[1] -= imm;
        r[2] -= imm;
        r[3] -= imm;
}

inline  void  reg_sub(_fptype* r, const _fptype* s) noexcept {
        r[0] -= s[0];
        r[1] -= s[1];
        r[2] -= s[2];
        r[3] -= s[3];
}

inline  void  reg_mul(_fptype* r, const _fptype imm) noexcept {
        r[0] *= imm;
        r[1] *= imm;
        r[2] *= imm;
        r[3] *= imm;
}

inline  void  reg_mul(_fptype* r, const _fptype* s) noexcept {
        r[0] *= s[0];
        r[1] *= s[1];
        r[2] *= s[2];
        r[3] *= s[3];
}

inline  void  reg_div(_fptype* r, const _fptype imm) noexcept {
        r[0] /= imm;
        r[1] /= imm;
        r[2] /= imm;
        r[3] /= imm;
}

inline  void  reg_div(_fptype* r, const _fptype* s) noexcept {
        r[0] /= s[0];
        r[1] /= s[1];
        r[2] /= s[2];
        r[3] /= s[3];
}

inline  void  vec_mov(_fptype* p0, _fptype* p1, const _fptype imm) noexcept {
        while(p0 < p1) {
            reg_mov(p0, imm);
            p0 +=_fpts;
        }
}

inline  size_t vec_mov(_fptype* p0, _fptype* p1, _fptype* s0, _fptype* s1) noexcept {
        size_t dst_size = p1 - p0;
        size_t src_size = s1 - s0;
        if(dst_size > src_size) {
            dst_size = src_size;
        }
        if(dst_size) {
            std::memcpy(p0, s0, dst_size * sizeof(_fptype));
        }
        return dst_size;
}

inline  size_t vec_mov_prescaled(_fptype* p0, _fptype* p1, _fptype* s0, _fptype* s1, const _fptype* scale) noexcept {
        size_t dst_size;
        size_t src_size = s1 - s0;
        if(src_size) {
            dst_size = p1 - p0;
            if(src_size == dst_size) {
                _fptype* ic  = s0; /*copy pointer*/
                _fptype* i0  = p0;
                _fptype* i1;
                size_t  pts = src_size /_fpts;
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

inline  void vec_clr(_fptype* p0, _fptype* p1) noexcept {
        size_t dst_size = p1 - p0;
        if(dst_size) {
            std::memset(p0, 0, dst_size * sizeof(_fptype));
        }
}

inline  bool equ(_fptype lhs, _fptype rhs) noexcept {
        _fptype error = 0.0;
        if(lhs == rhs) {
            return true;
        }
        if(std::fabs(rhs) > std::fabs(lhs)) {
            error = std::fabs((lhs - rhs) / rhs);
        } else
            error = std::fabs((lhs - rhs) / lhs);
        return error <=_epsilon;
}

inline  bool equ(const _fptype* r, _fptype imm) noexcept {
        for(auto x = 0; x !=_fpts; x++) {
            if(equ(r[x], imm) == false) {
                return false;
            }
        }
        return true;
}

inline  bool equ(const _fptype* r, const _fptype* s) noexcept {
        for(auto x = 0; x !=_fpts; x++) {
            if(equ(r[x], s[x]) == false) {
                return false;
            }
        }
        return true;
}

inline  bool   equ_0(const _fptype* r) noexcept {
        return equ(r, 0.0);
}

inline  bool   equ_1(const _fptype* r) noexcept {
        return equ(r, 1.0);
}
/*namespace fpu*/ }
#endif
