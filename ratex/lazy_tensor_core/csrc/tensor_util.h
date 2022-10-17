/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "lazy_tensor_core/csrc/device.h"
#include "lazy_tensors/computation_client/computation_client.h"
#include "lazy_tensors/literal.h"
#include "lazy_tensors/shape.h"
#include "lazy_tensors/span.h"
#include "lazy_tensors/types.h"
#include "torch/csrc/autograd/variable.h"

namespace torch_lazy_tensors {

std::vector<int64_t> ComputeShapeStrides(const lazy_tensors::Shape& shape);

std::vector<int64_t> ComputeArrayStrides(lazy_tensors::Span<const int64_t> sizes);

// Converts a literal to an at::Tensor of the given element type.
at::Tensor MakeTensorFromLiteral(const lazy_tensors::Literal& literal,
                                 at::ScalarType dest_element_type);

std::vector<at::Tensor> DataHandlesToTensors(
    lazy_tensors::Span<const lazy_tensors::ComputationClient::DataPtr> data_handles,
    at::ScalarType dest_element_type);

bool TensorCompare(const at::Tensor& t1, const at::Tensor& t2);

// Uploads an ATEN tensor data to the device and fetches the corresponding
// device data handle.
lazy_tensors::ComputationClient::DataPtr TensorToDataHandle(const at::Tensor& tensor,
                                                            const Device& device);

void PopulateTensorBuffer(const at::Tensor& tensor, const lazy_tensors::Shape& dest_shape,
                          void* dest_buffer, size_t dest_buffer_size, const Device& device);

lazy_tensors::hash_t TensorHash(const at::Tensor& tensor);

// Retrieves the device data handles by parallel uploading data onto the
// corresponding devices.
std::vector<lazy_tensors::ComputationClient::DataPtr> CreateTensorsData(
    const std::vector<at::Tensor>& tensors, const std::vector<std::string>& devices);

// Creates a literal out of an ATEN tensor. If shape is specified, that
// shape+layout will be used, otherwise one will be generated out of the ATEN
// tensor shape. The device argument (can be nullptr for the default device)
// tells the API that the created Literal will be sent to such device.
lazy_tensors::Literal GetTensorLiteral(const at::Tensor& tensor, const lazy_tensors::Shape* shape,
                                       const Device* device);

// If "shape" is a tuple, return the element shapes, otherwise return a
// singleton list containing the original shape.
std::vector<lazy_tensors::Shape> GetComponentShapes(const lazy_tensors::Shape& shape);

// Create a shape with "device_type" compatible layout from the given "shape".
lazy_tensors::Shape MakeShapeWithDeviceLayout(const lazy_tensors::Shape& shape,
                                              DeviceType device_type);

// Create the shape to be used within a lowered computation, to represent a
// given tensor data.
lazy_tensors::Shape CreateComputationShapeFromTensor(const at::Tensor& tensor,
                                                     const Device* device);

at::ScalarType TensorTypeFromLtcType(lazy_tensors::PrimitiveType ltc_type);

lazy_tensors::PrimitiveType TensorTypeToLtcType(at::ScalarType scalar_type);

// Maps a type to the one which can be used on the given device (or the default
// device, id device is nullptr).
lazy_tensors::PrimitiveType GetDevicePrimitiveType(lazy_tensors::PrimitiveType type,
                                                   const Device* device);

// Converts the given scalar type to a primitive type.
lazy_tensors::PrimitiveType MakeLtcPrimitiveType(at::ScalarType scalar_type, const Device* device);

bool RequiresRawTypeCasting(at::ScalarType scalar_type, const Device* device);

lazy_tensors::PrimitiveType GetShapeDimensionType(const Device* device);

}  // namespace torch_lazy_tensors
