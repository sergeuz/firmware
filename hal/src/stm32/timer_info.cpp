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

#include <cstddef>
#include "timer_info.h"
#include "pinmap_impl.h"

namespace {

// Basic timers
STM32_Timer_Info timerInfo6 = { TIM6 },
    timerInfo7 = { TIM7 };

// General-purpose timers
STM32_Advanced_Timer_Info timerInfo2 = { { TIM2 }, 0 },
    timerInfo3 = { { TIM3 }, 0 },
    timerInfo4 = { { TIM4 }, 0 },
    timerInfo5 = { { TIM5 }, 0 },
    timerInfo9 = { { TIM9 }, 0 },
    timerInfo10 = { { TIM10 }, 0 },
    timerInfo11 = { { TIM11 }, 0 },
    timerInfo12 = { { TIM12 }, 0 },
    timerInfo13 = { { TIM13 }, 0 },
    timerInfo14 = { { TIM14 }, 0 };

// Advanced-control timers
STM32_Advanced_Timer_Info timerInfo1 = { { TIM1 }, 0 },
    timerInfo8 = { { TIM8 }, 0 };

STM32_Timer_Info* timerInfo[STM32_TOTAL_TIMERS] = {
    (STM32_Timer_Info*)&timerInfo1,
    (STM32_Timer_Info*)&timerInfo2,
    (STM32_Timer_Info*)&timerInfo3,
    (STM32_Timer_Info*)&timerInfo4,
    (STM32_Timer_Info*)&timerInfo5,
    (STM32_Timer_Info*)&timerInfo6,
    (STM32_Timer_Info*)&timerInfo7,
    (STM32_Timer_Info*)&timerInfo8,
    (STM32_Timer_Info*)&timerInfo9,
    (STM32_Timer_Info*)&timerInfo10,
    (STM32_Timer_Info*)&timerInfo11,
    (STM32_Timer_Info*)&timerInfo12,
    (STM32_Timer_Info*)&timerInfo13,
    (STM32_Timer_Info*)&timerInfo14
};

STM32_Timer_Info* timerInfoForHandle(TIM_TypeDef *tim) {
    for (size_t i = 0; i < STM32_TOTAL_TIMERS; ++i) {
        if (timerInfo[i]->tim == tim) {
            return timerInfo[i];
        }
    }
    return 0;
}

class PinTimerMap {
public:
    PinTimerMap() {
        STM32_Pin_Info *pinMap = HAL_Pin_Map();
        for (size_t i = 0; i < TOTAL_PINS; ++i) {
            pinTimers_[i] = timerInfoForHandle(pinMap[i].timer_peripheral);
        }
    }

    STM32_Timer_Info* timerInfo(pin_t pin) {
        if (pin < TOTAL_PINS) {
            return pinTimers_[pin];
        }
        return 0;
    }

private:
    STM32_Timer_Info* pinTimers_[TOTAL_PINS];
};

} // namespace

STM32_Timer_Info* STM32_Timer_Info_For_ID(unsigned int id) {
    if (id < STM32_TOTAL_TIMERS) {
        return timerInfo[id];
    }
    return 0;
}

STM32_Timer_Info* STM32_Timer_Info_For_Pin(pin_t pin) {
    static PinTimerMap pinTimers;
    return pinTimers.timerInfo(pin);
}
