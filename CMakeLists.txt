cmake_minimum_required(VERSION 3.25)
project(vk_framework_impl)

set(CMAKE_CXX_STANDARD 17)

set(INC_DIR ${CMAKE_SOURCE_DIR}/framework/inc)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/framework/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

configure_file(${INC_DIR}/Window/vk_render_surface.h.in
               ${INC_DIR}/Window/vk_render_surface.h)

configure_file(${INC_DIR}/Window/plat_window.h.in
        ${INC_DIR}/Window/plat_window.h)

# ===================================
# Build glm
add_subdirectory(${EXTERNAL_DIR}/glm)
# ===================================

# ===================================
# Build GLFW
add_subdirectory(${EXTERNAL_DIR}/glfw-3.3.8)
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
                    ${SRC_DIR}/Shader/Shader.cpp)
# ===================================

# ===================================
# Link required libs to lily
target_link_libraries(lily PUBLIC vk_loader glfw glm)
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