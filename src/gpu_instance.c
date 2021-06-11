/** @file gpu_instance.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "gpu_instance.h"

struct mdo_gpu_instance_s
{
  const mdo_allocator_t *alloc;

  mdo_result_t vk_error;

  VkInstance instance;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL
vulkan_debug_callback (
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *userdata)
{
  mdo_log_level_t severity;

  switch (message_severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      severity = MDO_LOG_INFO;
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      severity = MDO_LOG_WARNING;
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    default:
      severity = MDO_LOG_ERROR;
      break;
    }

  LOG_AT (severity, callback_data->pMessage);
  return VK_FALSE;
}

mdo_result_t
mdo_gpu_instance_create (mdo_gpu_instance_t **instance,
                         const mdo_allocator_t *alloc,
                         const mdo_gpu_requirements_t *requirements)
{
  mdo_gpu_instance_t *new_instance
      = mdo_allocator_malloc (alloc, sizeof (mdo_gpu_instance_t));
  *instance = new_instance;

  new_instance->alloc = alloc;
  new_instance->vk_error
      = mdo_result_create (MDO_LOG_ERROR, "Vulkan error (%s):\n%s", 2, false);
  new_instance->instance = VK_NULL_HANDLE;

  VkApplicationInfo application_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,

    /* TODO(marceline-cramer) read these parameters from requirements */
    .pApplicationName = "mdo-renderer",
    .applicationVersion = VK_MAKE_VERSION (0, 0, 0),
    .pEngineName = "mdo-renderer",
    .engineVersion = VK_MAKE_VERSION (0, 0, 0),

    .apiVersion = requirements->min_api_version,
  };

  const char *layers[] = { "VK_LAYER_KHRONOS_validation" };

  VkInstanceCreateInfo ci = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &application_info,
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = layers,
    .enabledExtensionCount = 0,
  };

  VkDebugUtilsMessengerCreateInfoEXT messenger_ci = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                   | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                   | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = vulkan_debug_callback,
  };

  ci.pNext = &messenger_ci;

  VkResult vk_result;
  vk_result = vkCreateInstance (&ci, NULL, &new_instance->instance);

  mdo_result_t result;
  result = mdo_gpu_instance_log_result (new_instance, vk_result,
                                        "failed to create VkInstance");

  if (!mdo_result_success (result))
    return result;

  return MDO_SUCCESS;
}

void
mdo_gpu_instance_delete (mdo_gpu_instance_t *instance)
{
  const mdo_allocator_t *alloc = instance->alloc;

  if (instance->instance)
    vkDestroyInstance (instance->instance, NULL);

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
