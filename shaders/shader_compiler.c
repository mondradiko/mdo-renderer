/** @file shader_compiler.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shaderc/shaderc.h>

const char *
get_shaderc_compilation_status_string (shaderc_compilation_status status)
{
  switch (status)
    {
    case shaderc_compilation_status_success:
      return NULL;
    case shaderc_compilation_status_invalid_stage:
      return "invalid stage";
    case shaderc_compilation_status_compilation_error:
      return "compilation error";
    case shaderc_compilation_status_internal_error:
      return "internal error";
    case shaderc_compilation_status_null_result_object:
      return "null result object";
    case shaderc_compilation_status_invalid_assembly:
      return "invalid assembly";
    case shaderc_compilation_status_validation_error:
      return "validation error";
    default:
      return "unhandled error code";
    }
}

int
main (int argc, const char *argv[])
{
  const char *source_path = argv[1];
  const char *output_path = argv[2];

  char *shader_source;
  size_t source_size;

  {
    FILE *source_file = fopen (source_path, "rb");

    if (ferror (source_file))
      {
        fprintf (stderr, "failed to open source file %s\n", source_path);
        return 1;
      }

    fseek (source_file, 0, SEEK_END);
    source_size = ftell (source_file);
    fseek (source_file, 0, SEEK_SET);

    shader_source = (char *)malloc (source_size);
    fread (shader_source, 1, source_size, source_file);

    fclose (source_file);
  }

  char *shader_output;
  size_t output_size;

  {
    shaderc_compiler_t compiler = shaderc_compiler_initialize ();
    shaderc_compile_options_t options = shaderc_compile_options_initialize ();

    shaderc_optimization_level opt = shaderc_optimization_level_performance;
    shaderc_compile_options_set_optimization_level (options, opt);

    shaderc_compilation_result_t result = shaderc_compile_into_spv (
        compiler, shader_source, source_size,
        shaderc_glsl_default_compute_shader, source_path, "main", options);

    free ((void *)shader_source);

    const char *result_errors = shaderc_result_get_error_message (result);
    if (strlen (result_errors) > 0)
      {
        fprintf (stderr, "compilation errors:\n%s\n", result_errors);
      }

    shaderc_compilation_status status
        = shaderc_result_get_compilation_status (result);

    const char *compilation_error
        = get_shaderc_compilation_status_string (status);
    if (compilation_error)
      {
        fprintf (stderr, "compilation error: %s\n", compilation_error);
        shaderc_result_release (result);
        shaderc_compile_options_release (options);
        shaderc_compiler_release (compiler);
        return 1;
      }

    output_size = shaderc_result_get_length (result);
    shader_output = (char *)malloc (output_size);
    memcpy (shader_output, shaderc_result_get_bytes (result), output_size);

    shaderc_result_release (result);
    shaderc_compile_options_release (options);
    shaderc_compiler_release (compiler);
  }

  {
    FILE *output_file = fopen (output_path, "wb");

    if (ferror (output_file))
      {
        fprintf (stderr, "failed to open output file %s\n", source_path);
        return 1;
      }

    fwrite (shader_output, 1, output_size, output_file);
    fclose (output_file);
  }

  free ((void *)shader_output);

  return 0;
}
