// Copyright 2020 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_TOOLS_UTILS_VM_UTIL_H_
#define IREE_TOOLS_UTILS_VM_UTIL_H_

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "iree/base/status.h"
#include "iree/hal/api.h"
#include "iree/vm/api.h"
#include "iree/vm/ref_cc.h"

namespace iree {

// TODO(benvanik) Update these when we can use RAII with the C API.

// Synchronously reads a file's contents into a string.
Status GetFileContents(const char* path, std::string* out_contents);

// Parses |input_strings| into a variant list of VM scalars and buffers.
// Scalars should be in the format:
//   type=value
// Buffers should be in the IREE standard shaped buffer format:
//   [shape]xtype=[value]
// described in iree/hal/api.h
// Uses |allocator| to allocate the buffers.
// Uses descriptors in |descs| for type information and validation.
// The returned variant list must be freed by the caller.
Status ParseToVariantList(iree_hal_allocator_t* allocator,
                          absl::Span<const absl::string_view> input_strings,
                          iree_vm_list_t** out_list);
Status ParseToVariantList(iree_hal_allocator_t* allocator,
                          absl::Span<const std::string> input_strings,
                          iree_vm_list_t** out_list);

// Prints a variant list of VM scalars and buffers to |os|.
// Prints scalars in the format:
//   value
// Prints buffers in the IREE standard shaped buffer format:
//   [shape]xtype=[value]
// described in
// https://github.com/google/iree/tree/main/iree/hal/api.h
// Uses descriptors in |descs| for type information and validation.
Status PrintVariantList(iree_vm_list_t* variant_list,
                        std::ostream* os = &std::cout);

// Creates the default device for |driver| in |out_device|.
// The returned |out_device| must be released by the caller.
Status CreateDevice(const char* driver_name, iree_hal_device_t** out_device);

// Creates a hal module |driver| in |out_hal_module|.
// The returned |out_module| must be released by the caller.
Status CreateHalModule(iree_hal_device_t* device,
                       iree_vm_module_t** out_module);

// Loads a VM bytecode from an opaque string.
// The returned |out_module| must be released by the caller.
Status LoadBytecodeModule(absl::string_view module_data,
                          iree_vm_module_t** out_module);

}  // namespace iree

#endif  // IREE_TOOLS_UTILS_VM_UTIL_H_
