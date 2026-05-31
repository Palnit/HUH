set(INC_PATH "include/HUH/Graphics")


set(HEADER_FILES
        ${INC_PATH}/transform.h
        ${INC_PATH}/time.h
)
set(SOURCE_FILES
        src/transform.cpp
        src/time.cpp
)

add_library(HUH-Graphics-Core SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Graphics::Core ALIAS HUH-Graphics-Core)
huh_target_create_definition(HUH-Graphics-Core)
target_link_libraries(HUH-Graphics-Core PUBLIC
        HUH::Core
        HUH::Math
)
target_include_directories(HUH-Graphics-Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


add_library(HUH-Graphics INTERFACE)
add_library(HUH::Graphics ALIAS HUH-Graphics)
target_link_libraries(HUH-Graphics INTERFACE HUH::Graphics::Core)
