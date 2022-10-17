/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/convolution_backward_overrideable.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

ConvolutionBackwardOverrideable::ConvolutionBackwardOverrideable(
    const Value& grad_output, const Value& input, const Value& weight, std::vector<int64_t> stride,
    std::vector<int64_t> padding, std::vector<int64_t> dilation, bool transposed,
    std::vector<int64_t> output_padding, int64_t groups)
    : Node(
          ir::OpKind(at::aten::convolution_backward_overrideable), {grad_output, input, weight},
          /*num_outputs=*/3,
          lazy_tensors::util::MHash(stride, padding, dilation, transposed, output_padding, groups)),
      stride_(std::move(stride)),
      padding_(std::move(padding)),
      dilation_(std::move(dilation)),
      output_padding_(std::move(output_padding)),
      transposed_(transposed),
      groups_(groups) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr ConvolutionBackwardOverrideable::Clone(OpList operands) const {
  return MakeNode<ConvolutionBackwardOverrideable>(operands.at(0), operands.at(1), operands.at(2),
                                                   stride_, padding_, dilation_, transposed_,
                                                   output_padding_, groups_);
}

std::string ConvolutionBackwardOverrideable::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", stride=(" << absl::StrJoin(stride_, ", ") << "), padding=("
     << absl::StrJoin(padding_, ", ") << "), dilation=(" << absl::StrJoin(dilation_, ", ")
     << "), transpose=" << transposed_ << ", output_padding=("
     << absl::StrJoin(output_padding_, ", ") << "), groups=" << groups_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
