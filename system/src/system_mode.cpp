/**
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

#include "system_mode.h"
#include "system_task.h"

static System_Mode_TypeDef current_mode = DEFAULT;

volatile bool SPARK_OPTION_SERIAL_CONSOLE_ENABLED = true;

namespace {

// Helper functions for primitive option types
template<typename T1, typename T2> // T1 - Exposed type, T2 - Internal type
inline int set_option(T2 &value, const void *data, int size)
{
    if (size != sizeof(T1)) {
        return -1; // Invalid buffer size
    }
    value = *static_cast<const T1*>(data);
    return 0;
}

template<typename T1, typename T2>
inline int get_option(T2 value, void *data, int size, int *actualSize)
{
    if (actualSize) {
        *actualSize = sizeof(T1);
    }
    if (!data) {
        return 0; // Only return size of the option data
    }
    if (size != sizeof(T1)) {
        return -1; // Invalid buffer size
    }
    *static_cast<T1*>(data) = (T1)value;
    return 0;
}

} // namespace

void set_system_mode(System_Mode_TypeDef mode)
{
    // the SystemClass is constructed twice,
    // once for the `System` instance used by the system (to provide the System.xxx api)
    // and again for the SYSTEM_MODE() macro. The order of module initialization is
    // undefined in C++ so they may be initialized in any arbitrary order.
    // Meaning, the instance from SYSTEM_MODE() macro might be constructed first,
    // followed by the `System` instance, which sets the mode back to `AUTOMATIC`.
    // The DEFAULT mode prevents this.
    if (mode==DEFAULT) {            // the default system instance
        if (current_mode==DEFAULT)         // no mode set yet
            mode = AUTOMATIC;       // set to automatic mode
        else
            return;                 // don't change the current mode when constructing the system instance and it's already set
    }

    current_mode = mode;
    switch (mode)
    {
    		case DEFAULT:   // DEFULT can't happen in practice since it's cleared above. just keeps gcc happy.
        case SAFE_MODE:
        case AUTOMATIC:
            SPARK_CLOUD_CONNECT = 1;
            SPARK_WLAN_SLEEP = 0;
            break;

        case SEMI_AUTOMATIC:
            SPARK_CLOUD_CONNECT = 0;
            SPARK_WLAN_SLEEP = 1;
            break;

        case MANUAL:
            SPARK_CLOUD_CONNECT = 0;
            SPARK_WLAN_SLEEP = 1;
            break;
    }
}

System_Mode_TypeDef system_mode()
{
    return current_mode;
}

int system_set_option(spark::SystemOption option, const void *data, int size)
{
    switch (option) {
    case spark::SystemOption::SERIAL_CONSOLE_ENABLED:
        return set_option<bool>(SPARK_OPTION_SERIAL_CONSOLE_ENABLED, data, size);
    default:
        return -1; // Invalid option
    }
}

int system_get_option(spark::SystemOption option, void *data, int size, int *actualSize)
{
    switch (option) {
    case spark::SystemOption::SERIAL_CONSOLE_ENABLED:
        return get_option<bool>(SPARK_OPTION_SERIAL_CONSOLE_ENABLED, data, size, actualSize);
    default:
        return -1; // Invalid option
    }
}

#if PLATFORM_THREADING

static volatile spark::feature::State system_thread_enable = spark::feature::DISABLED;

void system_thread_set_state(spark::feature::State state, void*)
{
    system_thread_enable = state;
}

spark::feature::State system_thread_get_state(void*)
{
    return system_thread_enable;
}

#else

void system_thread_set_state(spark::feature::State state, void*)
{
}

spark::feature::State system_thread_get_state(void*)
{
    return spark::feature::DISABLED;
}

#endif
