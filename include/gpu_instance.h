/** @file gpu_instance.h
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include "mdo-utils/allocator.h"
#include "mdo-utils/result.h"

/** @typedef mdo_gpu_instance_t
  An object to create, destroy, and interface with all Vulkan instance state.
 */
typedef struct mdo_gpu_instance_s mdo_gpu_instance_t;

/** @function mdo_gpu_instance_create
  @brief Creates a GPU instance.
  @param instance The new #mdo_vk_instance_t.
  @param alloc The allocator to use.
  @return #mdo_result_t.
 */
mdo_result_t mdo_gpu_instance_create (mdo_gpu_instance_t **,
                                      const mdo_allocator_t *);

/** @function mdo_gpu_instance_delete
  @brief Deletes a GPU instance.
  @param instance The #mdo_vk_instance_t to delete.
 */
void mdo_gpu_instance_delete (mdo_gpu_instance_t *);

