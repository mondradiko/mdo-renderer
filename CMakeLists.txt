# Copyright (c) 2021 Marceline Cramer
# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.18)
project (mdo-renderer VERSION 0.0.0)

# options
option (ENABLE_TESTS "Enable testing suite.")

# C standard
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED TRUE)

if (UNIX)
  add_compile_options (-Wpedantic)
endif ()

# dependencies
find_package (mdo-utils CONFIG REQUIRED)

include (FindVulkan)
if (NOT Vulkan_FOUND)
  message (FATAL_ERROR "Vulkan not found.")
endif ()

# setup library
include (mondradiko_setup_library)
mondradiko_setup_library (mdo-renderer MDO_RENDERER_OBJ
  src/gpu_instance.c
)

target_link_libraries (${MDO_RENDERER_OBJ} PUBLIC
  mondradiko::mdo-utils
  Vulkan::Vulkan
)

# tests
if (ENABLE_TESTS)
  enable_testing ()
  add_subdirectory (tests)
endif ()
