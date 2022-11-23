#ifndef bops_dev_ili9341_h
#define bops_dev_ili9341_h
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
#include <blackops.h>

namespace dev {

constexpr std::uint8_t ili9341_nop          = 0x00;
constexpr std::uint8_t ili9341_swrst        = 0x01;
constexpr std::uint8_t ili9341_rddid        = 0x04;
constexpr std::uint8_t ili9341_rddst        = 0x09;

constexpr std::uint8_t ili9341_rdmode       = 0x0a;
constexpr std::uint8_t ili9341_rdmadctl     = 0x0b;
constexpr std::uint8_t ili9341_rdpixfmt     = 0x0c;
constexpr std::uint8_t ili9341_rdimgfmt     = 0x0a;
constexpr std::uint8_t ili9341_rdselfdiag   = 0x0f;

constexpr std::uint8_t ili9341_slpin        = 0x10;
constexpr std::uint8_t ili9341_slpout       = 0x11;
constexpr std::uint8_t ili9341_ptlon        = 0x12;
constexpr std::uint8_t ili9341_noron        = 0x13;

constexpr std::uint8_t ili9341_invoff       = 0x20;
constexpr std::uint8_t ili9341_invon        = 0x21;
constexpr std::uint8_t ili9341_gammaset     = 0x26;
constexpr std::uint8_t ili9341_dispoff      = 0x28;
constexpr std::uint8_t ili9341_dispon       = 0x29;

constexpr std::uint8_t ili9341_caset        = 0x2a;
constexpr std::uint8_t ili9341_paset        = 0x2b;
constexpr std::uint8_t ili9341_ramwr        = 0x2c;
constexpr std::uint8_t ili9341_ramrd        = 0x2e;

constexpr std::uint8_t ili9341_ptlar        = 0x30;
constexpr std::uint8_t ili9341_vscrdef      = 0x33;
constexpr std::uint8_t ili9341_madctl       = 0x36;
constexpr std::uint8_t ili9341_vscrsadd     = 0x37;
constexpr std::uint8_t ili9341_pixfmt       = 0x3a;

constexpr std::uint8_t ili9341_wrdisbv      = 0x51;
constexpr std::uint8_t ili9341_rddisbv      = 0x52;
constexpr std::uint8_t ili9341_wrctrld      = 0x53;

constexpr std::uint8_t ili9341_frmctr1      = 0xb1;
constexpr std::uint8_t ili9341_frmctr2      = 0xb2;
constexpr std::uint8_t ili9341_frmctr3      = 0xb3;
constexpr std::uint8_t ili9341_invctr       = 0xb4;
constexpr std::uint8_t ili9341_dfunctr      = 0xb6;

constexpr std::uint8_t ili9341_pwctr1       = 0xc0;
constexpr std::uint8_t ili9341_pwctr2       = 0xc1;
constexpr std::uint8_t ili9341_pwctr3       = 0xc2;
constexpr std::uint8_t ili9341_pwctr5       = 0xc3;
constexpr std::uint8_t ili9341_pwctr6       = 0xc4;
constexpr std::uint8_t ili9341_vmctr1       = 0xc5;
constexpr std::uint8_t ili9341_vmctr2       = 0xc7;

constexpr std::uint8_t ili9341_rdid4        = 0xd3;
constexpr std::uint8_t ili9341_rdindex      = 0xd9;
constexpr std::uint8_t ili9341_rdid1        = 0xda;
constexpr std::uint8_t ili9341_rdid2        = 0xdb;
constexpr std::uint8_t ili9341_rdid3        = 0xdc;
constexpr std::uint8_t ili9341_rdidx        = 0xdd;

constexpr std::uint8_t ili9341_gmctrp1      = 0xe0;
constexpr std::uint8_t ili9341_gmctrn1      = 0xe1;

constexpr std::uint8_t ili9341_ifctl        = 0xf6;

constexpr std::uint8_t ili9341_madctl_my    = 0x80;
constexpr std::uint8_t ili9341_madctl_mx    = 0x40;
constexpr std::uint8_t ili9341_madctl_mv    = 0x20;
constexpr std::uint8_t ili9341_madctl_ml    = 0x10;
constexpr std::uint8_t ili9341_madctl_rgb   = 0x00;
constexpr std::uint8_t ili9341_madctl_bgr   = 0x08;
constexpr std::uint8_t ili9341_madctl_mh    = 0x04;

/*namespace dev*/ }
#endif
