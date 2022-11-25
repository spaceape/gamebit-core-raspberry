#ifndef emc_config_h
#define emc_config_h
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
#include <emc.h>

namespace emc {

/* mtu_size
 * maximum size of a message to set implicitely on this platform, sometimes depending on the
 * communication backend
*/
constexpr int   mtu_size = 255;

/* queue_size_min
 * minimum size of the message queue - used to reserve memory for the receive and transmit queues;
 * should be at least equal to mtu_size, to at least avoid reallocating very soon
*/
constexpr int   queue_size_min = global::cache_large_max;

/* queue_size_max
 * maximum size of the message queue
*/
constexpr int   queue_size_max = 4096;


/* message_drop_time
 * drop a message if it is not completed within this time interval (in microseconds)
*/
constexpr int   message_drop_time = 4 * global::usps;

/* message_wait_time
 * generic response timeout
 * should not be greater than message_drop_time, otherwise, message may be dropped while waiting and never be completed.
*/
constexpr int   message_wait_time = 2 * global::usps;

/* message_trip_time
 * if silent for this interval, peer should be declared unreachable 
 * should be (much) greater than message_wait_time
*/
constexpr int   message_trip_time = 60 * global::usps;

/* message_ping_time
 * if silent for this interval, peer should be queried with a ping message
 * should be greater than message_wait_time to avoid flooding a slow connection with ping messages, but lower than 
 * message_trip_time minus message_wait_time, to make sure a pong message has time to be processed and travel back 
*/
constexpr int   message_ping_time = message_trip_time / 2;

/* host_connect_time
*/
constexpr int   socket_connect_time = 10 * global::usps;

/*namespace emc*/ }
#endif