/** @file gpu_instance.h
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h> /* for uint32_t */

#include <vulkan/vulkan.h>

#include "mdo-utils/allocator.h"
#include "mdo-utils/result.h"

/** @typedef mdo_gpu_instance_t
  An object to create, destroy, and interface with all "singleton"
  Vulkan state, including the Vulkan instance, physical device, and logical
  device.
 */
typedef struct mdo_gpu_instance_s mdo_gpu_instance_t;

/** @typedef mdo_gpu_requirements_t
 */
typedef struct mdo_gpu_requirements_s
{
  uint32_t min_api_version;
  uint32_t max_api_version;
  int request_validation;
} mdo_gpu_requirements_t;

/** @function mdo_gpu_instance_create
  @brief Creates a GPU instance.
  @param instance The new #mdo_gpu_instance_t.
  @param alloc The allocator to use.
  @param requirements See #mdo_gpu_requirements_t.
  @return #mdo_result_t.
 */
mdo_result_t mdo_gpu_instance_create (mdo_gpu_instance_t **,
                                      const mdo_allocator_t *,
                                      const mdo_gpu_requirements_t *);

/** @function mdo_gpu_instance_delete
  @brief Deletes a GPU instance.
  @param instance The #mdo_gpu_instance_t to delete.
 */
void mdo_gpu_instance_delete (mdo_gpu_instance_t *);

/** @function mdo_gpu_instance_log_result
  @brief Formats and logs a Vulkan result into an #mdo_result_t.
  @brief instance The #mdo_gpu_instance_t to format a result from.
  @brief result_code The Vulkan result code.
  @brief error_message A brief error message to log in the case of an error.
  @return #mdo_result_t.
 */
mdo_result_t mdo_gpu_instance_log_result (mdo_gpu_instance_t *, VkResult,
                                          const char *);
