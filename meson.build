# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

foreach device_name, device_lib : sw_lib_arch_core_devices

  aes_masking_isw_elf = executable(
    'aes_masking_isw_' + device_name,
    sources: ['aes_masking_isw.c',
    	'gf28mul.S',
    ],
    name_suffix: 'elf',
    dependencies: [
      sw_examples_demos,
      sw_lib_runtime_hart,
      sw_lib_runtime_print,
      sw_lib_runtime_log,
      sw_lib_pinmux,
      sw_lib_dif_gpio,
      sw_lib_irq,
      sw_lib_dif_spi_device,
      sw_lib_dif_uart,
      riscv_crt,
      sw_lib_irq_handlers,
      device_lib,
      sw_lib_testing_test_status,
    ],
  )

  aes_masking_isw_embedded = custom_target(
    'aes_masking_isw_' + device_name,
    command: make_embedded_target,
    input: aes_masking_isw_elf,
    output: make_embedded_target_outputs,
    build_by_default: true,
  )

  custom_target(
    'aes_masking_isw_export_' + device_name,
    command: export_embedded_target,
    input: [aes_masking_isw_elf, aes_masking_isw_embedded],
    output: 'aes_masking_isw_export_' + device_name,
    build_always_stale: true,
    build_by_default: true,
  )
endforeach
