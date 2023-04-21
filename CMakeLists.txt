cmake_minimum_required(VERSION 3.25)
project(vk_framework_impl)

set(CMAKE_CXX_STANDARD 17)

set(INC_DIR         ${CMAKE_SOURCE_DIR}/framework/inc)
set(SRC_DIR         ${CMAKE_SOURCE_DIR}/framework/src)
set(EXTERNAL_DIR    ${CMAKE_SOURCE_DIR}/external)
set(ASSET_FOLDER    ${CMAKE_SOURCE_DIR}/assets)

# render surface impl
configure_file( ${INC_DIR}/Window/vk_render_surface.h.in
                ${INC_DIR}/Window/vk_render_surface.h)

# platform windows
configure_file( ${INC_DIR}/Window/plat_window.h.in
                ${INC_DIR}/Window/plat_window.h)

# spir-v pseudo include
configure_file( ${INC_DIR}/Shader/utils/spirv_reflect.h.in
                ${INC_DIR}/Shader/utils/spirv_reflect.h)

# stb pseudo include
configure_file( ${INC_DIR}/Resources/stb_image.h.in
                ${INC_DIR}/Resources/stb_image.h)

# asset folder define
configure_file( ${INC_DIR}/Resources/asset_folder.h.in
                ${INC_DIR}/Resources/asset_folder.h)

# ===================================
# Add stb image lib
add_library(stb_image ${EXTERNAL_DIR}/stb/stb_loader.cpp)
# ===================================

# ===================================
# Add spv_reflect
add_library(spv_reflect ${EXTERNAL_DIR}/spirv-reflect/spirv_reflect.cpp)
# ===================================

# ===================================
# Build glm
add_subdirectory(${EXTERNAL_DIR}/glm)
# ===================================

# ===================================
# Build GLFW
add_subdirectory(${EXTERNAL_DIR}/glfw-3.3.8)
# ===================================

# ===================================
# Add shaderc lib
add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION
        ${EXTERNAL_DIR}/shaderc/lib/libshaderc_combined.a)
# ===================================

# ===================================
# Build log_util
add_library(log_util ${SRC_DIR}/Log/${CMAKE_SYSTEM_NAME}/Log.cpp)
target_include_directories(log_util PUBLIC ${INC_DIR})
# ===================================

# ===================================
# Build vk_loader
add_library(vk_loader   ${SRC_DIR}/vk_loader/vk_functions.cpp
                        ${SRC_DIR}/vk_loader/vk_loader.cpp)
target_include_directories(log_util PRIVATE ${INC_DIR})
# ===================================

# ===================================
# Link log_util to vk_loader
target_link_libraries(vk_loader PUBLIC log_util)
# ===================================

# ===================================
# Build lily-framework
add_library(lily    ${SRC_DIR}/Window/WindowBase.cpp
                    ${SRC_DIR}/Window/${CMAKE_SYSTEM_NAME}/Window.cpp
                    ${SRC_DIR}/Device/utils/vk_instance_and_device_utils.cpp
                    ${SRC_DIR}/common/vk_layer_and_extension_utils.cpp
                    ${SRC_DIR}/Device/Device.cpp
                    ${SRC_DIR}/Commands/utils/vk_command_X_utils.cpp
                    ${SRC_DIR}/Commands/Commands.cpp
                    ${SRC_DIR}/Resources/Buffer.cpp
                    ${SRC_DIR}/Resources/Image.cpp
                    ${SRC_DIR}/Resources/utils/vk_buffer_util.cpp
                    ${SRC_DIR}/Resources/Buffer/VertexBuffer.cpp
                    ${SRC_DIR}/Shader/utils/vk_descriptor_util.cpp
                    ${SRC_DIR}/Window/${CMAKE_SYSTEM_NAME}/vk_render_surface.cpp
                    ${SRC_DIR}/Shader/utils/shader_utils.cpp
                    ${SRC_DIR}/Renderer/Renderer.cpp
                    ${SRC_DIR}/Renderer/utils/vk_render_X_utils.cpp
                    framework/src/Shader/ShaderBuilder.cpp
                    framework/src/Shader/Shader.cpp
                    framework/src/Shader/utils/spv_reflect_util.cpp framework/src/Resources/Image/vk_image_util.cpp framework/src/Resources/Image/Texture2D.cpp framework/src/Resources/asset_utils.cpp)
# ===================================

# ===================================
# Link required libs to lily
target_link_libraries(lily PUBLIC vk_loader glfw glm shaderc spv_reflect stb_image)
# ===================================

# ===================================
# Create main
add_executable(vk_framework_impl main.cpp)
# ===================================

# ===================================
# Link required libs to main
target_link_libraries(vk_framework_impl PUBLIC lily)
target_include_directories(log_util PUBLIC ${INC_DIR})
# ===================================