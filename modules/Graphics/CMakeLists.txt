set(HEADER_FILES
        include/public/HUH/Graphics/basic_window.h
        include/public/HUH/Graphics/element_buffer_object.h
        include/public/HUH/Graphics/file_handling.h
        include/public/HUH/Graphics/generic_structs.h
        include/public/HUH/Graphics/SDL_GLDebugMessageCallback.h
        include/public/HUH/Graphics/shader_program.h
        include/public/HUH/Graphics/vertex_array_object.h
        include/public/HUH/Graphics/vertex_buffer_object.h
)
set(SOURCE_FILES
        src/basic_window.cpp
        src/element_buffer_object.cpp
        src/file_handling.cpp
        src/generic_structs.cpp
        src/SDL_GLDebugMessageCallback.cpp
        src/shader_program.cpp
        src/vertex_array_object.cpp
)

add_library(HUH-Graphics SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::Graphics ALIAS HUH-Graphics)
target_link_libraries(HUH-Graphics PUBLIC
        HUH
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        imgui::imgui
        GLEW::GLEW
        glm::glm
        OpenGL::GL
        implot::implot
)

target_include_directories(HUH-Graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public")
