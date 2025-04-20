set(GRAPHICS_INC_PATH "include/public/HUH/Graphics")
set(HEADER_FILES
        ${GRAPHICS_INC_PATH}/basic_window.h
        ${GRAPHICS_INC_PATH}/element_buffer_object.h
        ${GRAPHICS_INC_PATH}/file_handling.h
        ${GRAPHICS_INC_PATH}/generic_structs.h
        ${GRAPHICS_INC_PATH}/SDL_GLDebugMessageCallback.h
        ${GRAPHICS_INC_PATH}/shader_program.h
        ${GRAPHICS_INC_PATH}/vertex_array_object.h
        ${GRAPHICS_INC_PATH}/vertex_buffer_object.h
        ${GRAPHICS_INC_PATH}/texture.h
        ${GRAPHICS_INC_PATH}/camera.h
        ${GRAPHICS_INC_PATH}/generic_math.h
)
set(SOURCE_FILES
        src/basic_window.cpp
        src/element_buffer_object.cpp
        src/file_handling.cpp
        src/generic_structs.cpp
        src/SDL_GLDebugMessageCallback.cpp
        src/shader_program.cpp
        src/vertex_array_object.cpp
        src/texture.cpp
        src/camera.cpp
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(implot CONFIG REQUIRED)

add_library(HUH-Graphics SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Graphics ALIAS HUH-Graphics)

target_link_libraries(HUH-Graphics PUBLIC
        HUH
        SDL3::SDL3
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
        imgui::imgui
        GLEW::GLEW
        glm::glm
        OpenGL::GL
        implot::implot
)

target_include_directories(HUH-Graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public")
