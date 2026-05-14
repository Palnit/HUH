#pragma once
#include <HUH/types.h>

#include <wayland-client.h>

#include <map>
#include <string>

inline HUH::LogCategory LogWaylandConnector("Wayland Connector");

namespace HUH {
// TODO Custom scanner created for HUH and my own events specifically
class WaylandConnector {
public:
    WaylandConnector();
    ~WaylandConnector();

    struct GlobalData {
        Uint32 name;
        std::string interface;
        Uint32 version;
    };

    operator wl_display*() const { return m_display; }

    template<typename T>
    T* RegisterGlobal(const wl_interface* interface) {
        if (const auto it = m_createdGlobals.find(interface->name); it != m_createdGlobals.end()) {
            return static_cast<T*>(it->second);
        }
        auto it = m_globals.find(interface->name);
        if (it == m_globals.end()) {
            HUH_ELOG(LogWaylandConnector, "Global Interface not found")
            return nullptr;
        }
        auto& [key, data] = *it;
        auto global = wl_registry_bind(m_registry, data.name, interface, data.version);
        m_createdGlobals.emplace(key, global);
        return static_cast<T*>(global);
    }

    static void HandleWaylandGlobalRegister(void* data,
                                            wl_registry* registry,
                                            Uint32 name,
                                            const char* interface,
                                            Uint32 version);

    static void HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, Uint32 name);

private:
    std::map<std::string, GlobalData> m_globals;
    // TODO Destroy on globals ???
    std::map<std::string, void*> m_createdGlobals;
    wl_display* m_display = nullptr;
    wl_registry* m_registry = nullptr;
    static const wl_registry_listener s_registryListener;
};

extern WaylandConnector g_waylandConnector;

}// namespace HUH
