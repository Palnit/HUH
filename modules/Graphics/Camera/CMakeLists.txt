set(INC_PATH "include/HUH/Graphics")


set(HEADER_FILES
        ${INC_PATH}/camera.h
)
set(SOURCE_FILES
        src/camera.cpp
)

#if (EXISTS HUH::Window)
list(APPEND HEADER_FILES
        ${INC_PATH}/camera_window_connector.h
)
list(APPEND SOURCE_FILES
        src/camera_window_connector.cpp
)
#endif ()

add_library(HUH-Graphics-Camera SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Graphics::Camera ALIAS HUH-Graphics-Camera)
huh_target_create_definition(HUH-Graphics-Camera)
target_link_libraries(HUH-Graphics-Camera PUBLIC
        HUH::Graphics::Core
        HUH::Math
        #        TODO THIS being optional
        HUH::RHI
        HUH::Window
)
target_include_directories(HUH-Graphics-Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


target_link_libraries(HUH-Graphics INTERFACE HUH::Graphics::Camera)