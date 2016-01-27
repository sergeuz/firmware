/**
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#ifndef TIMER_INFO_H
#define TIMER_INFO_H

#include "pinmap_hal.h"
#include "stm32.h"

#define STM32_TOTAL_TIMERS 14

// Basic timers
#define STM32_TIMER_6 5
#define STM32_TIMER_7 6

// General-purpose timers
#define STM32_TIMER_2 1
#define STM32_TIMER_3 2
#define STM32_TIMER_4 3
#define STM32_TIMER_5 4
#define STM32_TIMER_9 8
#define STM32_TIMER_10 9
#define STM32_TIMER_11 10
#define STM32_TIMER_12 11
#define STM32_TIMER_13 12
#define STM32_TIMER_14 13

// Advanced-control timers
#define STM32_TIMER_1 0
#define STM32_TIMER_8 7

#ifdef  __cplusplus
extern "C" {
#endif

// Timer info
typedef struct STM32_Timer_Info {
    TIM_TypeDef *tim;
} STM32_Timer_Info;

// Timer info (general-purpose and advanced-control timers)
typedef struct STM32_Advanced_Timer_Info {
    STM32_Timer_Info timer_info;
    uint8_t active_channel_flags;
} STM32_Advanced_Timer_Info;

STM32_Timer_Info* STM32_Timer_Info_For_ID(unsigned int id);
STM32_Timer_Info* STM32_Timer_Info_For_Pin(pin_t pin);

inline uint8_t STM32_Timer_Channel_Mask(uint16_t channel) {
    switch (channel) {
    case TIM_Channel_1:
        return 0x01;
    case TIM_Channel_2:
        return 0x02;
    case TIM_Channel_3:
        return 0x04;
    case TIM_Channel_4:
        return 0x08;
    default:
        return 0;
    }
}

#ifdef  __cplusplus
}
#endif

#endif /* TIMER_INFO_H */
