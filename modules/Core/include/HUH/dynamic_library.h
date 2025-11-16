#pragma once

#ifdef HUH_LINUX
#include <HUH/Linux/dynamic_library.h>
#elif defined(HUH_WIN)
#include <HUH/Windows/dynamic_library.h>
#endif