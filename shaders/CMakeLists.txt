# Copyright (c) 2021 Marceline Cramer
# SPDX-License-Identifier: MIT

# pkgconfig is REQUIRED for shaderc
include (FindPkgConfig)
pkg_check_modules (SHADERC shaderc REQUIRED)

add_executable (shader_compiler shader_compiler.c)

target_link_libraries (shader_compiler ${SHADERC_LIBRARIES})
target_include_directories (shader_compiler PUBLIC ${SHADERC_INCLUDE_DIRS})

function (compile_shader ret SHADER_NAME)
  set (SHADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_NAME}.glsl")
  set (SHADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.h")

  add_custom_command (
    OUTPUT ${SHADER_OUTPUT}
    COMMAND shader_compiler ${SHADER_SOURCE} ${SHADER_OUTPUT}
    DEPENDS ${SHADER_SOURCE}
  )

  set (${ret} "${SHADER_OUTPUT}" PARENT_SCOPE)
endfunction ()

compile_shader (TEST_SHADER test)

add_library (mdo-renderer-shaders INTERFACE ${TEST_SHADER})
