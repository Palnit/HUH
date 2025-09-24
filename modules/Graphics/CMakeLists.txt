set(GRAPHICS_INC_PATH "Window/include/HUH/Graphics")
set(GRAPHICS_INC_PATH_OPENGL ${GRAPHICS_INC_PATH}/OpenGl)
set(GRAPHICS_INC_PATH_VULKAN ${GRAPHICS_INC_PATH}/Vulkan)

set(HEADER_FILES_OPENGL
        ${GRAPHICS_INC_PATH_OPENGL}/basic_window.h
        ${GRAPHICS_INC_PATH_OPENGL}/element_buffer_object.h
        ${GRAPHICS_INC_PATH_OPENGL}/file_handling.h
        ${GRAPHICS_INC_PATH_OPENGL}/SDL_GLDebugMessageCallback.h
        ${GRAPHICS_INC_PATH_OPENGL}/shader_program.h
        ${GRAPHICS_INC_PATH_OPENGL}/vertex_array_object.h
        ${GRAPHICS_INC_PATH_OPENGL}/vertex_buffer_object.h
        ${GRAPHICS_INC_PATH_OPENGL}/texture.h
        ${GRAPHICS_INC_PATH_OPENGL}/camera.h
        ${GRAPHICS_INC_PATH_OPENGL}/generic_math.h
)
set(SOURCE_FILES_OPENGL
        Window/src/OpenGl/basic_window.cpp
        Window/src/OpenGl/element_buffer_object.cpp
        Window/src/OpenGl/file_handling.cpp
        Window/src/OpenGl/SDL_GLDebugMessageCallback.cpp
        Window/src/OpenGl/shader_program.cpp
        Window/src/OpenGl/vertex_array_object.cpp
        Window/src/OpenGl/texture.cpp
        Window/src/OpenGl/camera.cpp
)

set(HEADER_FILES_VULKAN
        ${GRAPHICS_INC_PATH_VULKAN}/basic_window.h
)
set(SOURCE_FILES_VULKAN
        Window/src/Vulkan/basic_window.cpp
)

set(HEADER_FILES
        ${GRAPHICS_INC_PATH}/generic_structs.h
)

set(SOURCE_FILES
        Window/src/generic_structs.cpp
)


find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_library(HUH-Graphics SHARED ${HEADER_FILES} ${SOURCE_FILES} ${HEADER_FILES_OPENGL} ${SOURCE_FILES_OPENGL} ${HEADER_FILES_VULKAN} ${SOURCE_FILES_VULKAN})
add_library(HUH::Graphics ALIAS HUH-Graphics)
target_compile_definitions(HUH-Graphics PRIVATE HUH_COMP)

target_link_libraries(HUH-Graphics PUBLIC
        HUH
        HUH::Core
        SDL3::SDL3
        $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
        imgui::imgui
        GLEW::GLEW
        glm::glm
        OpenGL::GL
        implot::implot
)


target_include_directories(HUH-Graphics PUBLIC "Window/include")
