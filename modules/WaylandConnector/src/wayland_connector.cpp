#include <HUH/logging.h>
#include <HUH/wayland_connector.h>

#include <wayland-client.h>
// #include <wayland-protocols/xdg-decoration-client-protocol.h>
// #include <wayland-protocols/xdg-shell-client-protocol.h>

namespace HUH {

WaylandConnector g_waylandConnector;

const wl_registry_listener WaylandConnector::s_registryListener = {
    .global = WaylandConnector::HandleWaylandGlobalRegister,
    .global_remove = WaylandConnector::HandleWaylandGlobalRegisterRemove,
};

WaylandConnector::WaylandConnector() {
    m_display = wl_display_connect(nullptr);
    if (!m_display) {
        HUH_ELOG(LogWaylandConnector, "Couldn't connect to Wayland display");
    }
    HUH_ILOG(LogWaylandConnector, "Wayland Connector Initialized");
    m_registry = wl_display_get_registry(m_display);
    wl_registry_add_listener(m_registry, &s_registryListener, this);
    wl_display_roundtrip(m_display);
}

WaylandConnector::~WaylandConnector() {
    if (m_display) {
        HUH_ILOG(LogWaylandConnector, "Destroying Wayland Connector")
        wl_display_disconnect(m_display);
    }
}

void WaylandConnector::HandleWaylandGlobalRegister(void* data,
                                                   wl_registry* registry,
                                                   Uint32 name,
                                                   const char* interface,
                                                   Uint32 version) {

#ifdef HUH_DEBUG
    HUH_LOG(LogWaylandConnector, Logging::DebugLog, "Wayland Global Interface: {} Version: {} Name: {} ", interface,
            version, name);
#endif
    auto* connector = static_cast<WaylandConnector*>(data);
    connector->m_globals.insert({interface, {name, interface, version}});
}

void WaylandConnector::HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, Uint32 name) {
    auto* connector = static_cast<WaylandConnector*>(data);
    // TODO my events but probably ignorable?
}

}// namespace HUH