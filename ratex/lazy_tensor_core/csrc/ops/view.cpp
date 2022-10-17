/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/view.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/data_ops.h"
#include "lazy_tensor_core/csrc/helpers.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/shape_util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {
namespace {

lazy_tensors::Shape NodeOutputShape(const Value& input,
                                    lazy_tensors::Span<const int64_t> output_sizes) {
  const lazy_tensors::Shape& input_shape = input.shape();
  auto info = Helpers::GetDynamicReshapeInfo(input_shape, output_sizes);
  if (info) {
    return std::move(info->output_shape);
  }
  const auto complete_output_sizes = GetCompleteShape(output_sizes, input_shape.dimensions());
  return lazy_tensors::ShapeUtil::MakeShape(input_shape.element_type(), complete_output_sizes);
}

}  // namespace

View::View(const Value& input, std::vector<int64_t> output_size)
    : Node(ir::OpKind(at::aten::view), {input}, NodeOutputShape(input, output_size),
           /*num_outputs=*/1, lazy_tensors::util::MHash(output_size)),
      output_size_(std::move(output_size)) {
}

std::string View::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", output_size=(" << absl::StrJoin(output_size_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
