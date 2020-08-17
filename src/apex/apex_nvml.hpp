/*  Copyright (c) 2020 University of Oregon
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "apex.hpp"
#include "nvml.h"
#include <set>
#include <mutex>

namespace apex { namespace nvml {

class monitor {
public:
    monitor (void);
    ~monitor (void);
    void query();
    static void activateDeviceIndex(uint32_t index);
private:
    uint32_t deviceCount;
    std::vector<nvmlDevice_t> devices;
    static std::set<uint32_t> activeDeviceIndices;
    static std::mutex indexMutex;
}; // class monitor

} // namespace nvml
} // namespace apex
