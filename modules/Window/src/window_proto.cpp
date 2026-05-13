#include <HUH/Window/prototypes/window_proto.h>

namespace HUH {

std::string ToString(KeyBindings key){
    switch (key) {
        case KeyBindings::Mouse_1: return "Mouse_1";
        case KeyBindings::Mouse_2: return "Mouse_2";
        case KeyBindings::Mouse_3: return "Mouse_3";
        case KeyBindings::Mouse_4: return "Mouse_4";
        case KeyBindings::Mouse_5: return "Mouse_5";
        default: return "Unknown";
    }
}}