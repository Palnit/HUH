set(INC_PATH "include/HUH/FileHandling/Image")


set(HEADER_FILES
        ${INC_PATH}/image.h
        ${INC_PATH}/image_reader.h
)
set(SOURCE_FILES
        src/image.cpp
        src/image_reader.cpp
)


add_library(HUH-FileHandling-Image SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(HUH::FileHandling::Image ALIAS HUH-FileHandling-Image)
huh_target_create_definition(HUH-FileHandling-Image)
target_link_libraries(HUH-FileHandling-Image PUBLIC
        HUH::Core
        HUH::Math
)
find_package(PNG)
if (PNG_FOUND)
    message(STATUS "Found libpng building png image readers")
    set(HEADER_FILES_PNG
            ${INC_PATH}/png/png_reader.h
    )
    set(SOURCE_FILES_PNG
            src/png/png_reader.cpp
    )
    target_compile_definitions(HUH-FileHandling-Image PUBLIC HUH_IMAGE_READER_AVAILABLE_PNG)
    target_sources(HUH-FileHandling-Image PUBLIC ${HEADER_FILES_PNG} ${SOURCE_FILES_PNG})
    target_link_libraries(HUH-FileHandling-Image PUBLIC PNG::PNG)
endif ()

target_include_directories(HUH-FileHandling-Image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
