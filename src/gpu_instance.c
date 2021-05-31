/** @file gpu_instance.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "gpu_instance.h"

struct mdo_gpu_instance_s
{
  const mdo_allocator_t *alloc;
};

mdo_result_t
mdo_gpu_instance_create (mdo_gpu_instance_t **instance,
                         const mdo_allocator_t *alloc)
{
  mdo_gpu_instance_t *new_instance
      = mdo_allocator_malloc (alloc, sizeof (mdo_gpu_instance_t));
  *instance = new_instance;

  new_instance->alloc = alloc;

  return MDO_SUCCESS;
}

void
mdo_gpu_instance_delete (mdo_gpu_instance_t *instance)
{
  const mdo_allocator_t *alloc = instance->alloc;

  mdo_allocator_free (alloc, instance);
}

