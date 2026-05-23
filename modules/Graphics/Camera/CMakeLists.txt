set(INC_PATH "include/HUH/Graphics")


set(HEADER_FILES
        ${INC_PATH}/camera.h
)
set(SOURCE_FILES
        src/camera.cpp
)

add_library(HUH-Graphics-Camera SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Graphics::Camera ALIAS HUH-Graphics-Camera)
huh_target_create_definition(HUH-Graphics-Camera)
target_link_libraries(HUH-Graphics-Camera PUBLIC
        HUH::Graphics::Core
        HUH::Math
        HUH::RHI
        HUH::Window
)
target_include_directories(HUH-Graphics-Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


target_link_libraries(HUH-Graphics INTERFACE HUH::Graphics::Camera)