/** @file test-gpu_instance.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "gpu_instance.h"
#include "test_common.h"

static int
setup (void **state)
{
  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_gpu_instance_t *instance;
  mdo_result_t result = mdo_gpu_instance_create (&instance, alloc);
  if (!mdo_result_success (result))
    {
      return -1;
    }

  *state = instance;
  return 0;
}

static int
teardown (void **state)
{
  mdo_gpu_instance_t *instance = (mdo_gpu_instance_t *)*state;
  mdo_gpu_instance_delete (instance);
  return 0;
}

static void
test_create_and_destroy (void **state)
{
  assert_int_equal (0, setup (state));
  assert_int_equal (0, teardown (state));
}

static void
test_log_result_success_code (void **state)
{
  static const VkResult SUCCESS_CODE = VK_SUCCESS;

  mdo_gpu_instance_t *instance = (mdo_gpu_instance_t *)*state;
  mdo_result_t result
      = mdo_gpu_instance_log_result (instance, SUCCESS_CODE, "");
  assert_true (mdo_result_success (result));
}

static void
test_log_result_error_code (void **state)
{
  /* the exact error code doesn't matter;
   * VK_ERROR_UNKNOWN was picked arbitrarily
   * TODO(marceline-cramer): Test all Vulkan error codes? */
  static const VkResult ERROR_CODE = VK_ERROR_UNKNOWN;

  mdo_gpu_instance_t *instance = (mdo_gpu_instance_t *)*state;
  mdo_result_t result = mdo_gpu_instance_log_result (instance, ERROR_CODE, "");
  assert_false (mdo_result_success (result));
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown (test_create_and_destroy, NULL, NULL),
    cmocka_unit_test (test_log_result_success_code),
    cmocka_unit_test (test_log_result_error_code),
  };

  return cmocka_run_group_tests (tests, setup, teardown);
}
