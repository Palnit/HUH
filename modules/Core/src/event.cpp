#include "HUH/logging.h"

#include <atomic>
#include <HUH/types.h>
#include <HUH/definitions.h>

#include <HUH/event.h>

namespace HUH {

std::atomic<Uint64> HUH_CORE_API s_next_event_id = 0;

Uint64 EventHandler::GetNextEventId() {
    ++s_next_event_id;

    if (s_next_event_id == 0) {
        ++s_next_event_id;
    }

    return s_next_event_id;
}

}// namespace HUH