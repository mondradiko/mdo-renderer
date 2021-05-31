/** @file gpu_instance.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "gpu_instance.h"

struct mdo_gpu_instance_s
{
  const mdo_allocator_t *alloc;

  mdo_result_t vk_error;
};

mdo_result_t
mdo_gpu_instance_create (mdo_gpu_instance_t **instance,
                         const mdo_allocator_t *alloc)
{
  mdo_gpu_instance_t *new_instance
      = mdo_allocator_malloc (alloc, sizeof (mdo_gpu_instance_t));
  *instance = new_instance;

  new_instance->alloc = alloc;
  new_instance->vk_error
      = mdo_result_create (MDO_LOG_ERROR, "Vulkan error (%s):\n%s", 2, false);

  return MDO_SUCCESS;
}

void
mdo_gpu_instance_delete (mdo_gpu_instance_t *instance)
{
  const mdo_allocator_t *alloc = instance->alloc;

  mdo_allocator_free (alloc, instance);
}

static const char *
get_vulkan_result_message (VkResult vk_result)
{
  switch (vk_result)
    {
    case VK_SUCCESS:
      return NULL;
    case VK_ERROR_UNKNOWN:
      return "VK_ERROR_UNKNOWN";
    default:
      return "unhandled error code";
    }
}

mdo_result_t
mdo_gpu_instance_log_result (mdo_gpu_instance_t *instance, VkResult vk_result,
                             const char *error_message)
{
  const char *code_message = get_vulkan_result_message (vk_result);

  if (code_message)
    return LOG_RESULT (instance->vk_error, code_message, error_message);
  else
    return MDO_SUCCESS;
}
