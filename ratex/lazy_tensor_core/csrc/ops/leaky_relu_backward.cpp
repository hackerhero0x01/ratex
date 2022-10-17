/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/leaky_relu_backward.h"

#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

LeakyReluBackward::LeakyReluBackward(const Value& grad_output, const Value& input,
                                     double negative_slope)
    : Node(ir::OpKind(at::aten::leaky_relu_backward), {grad_output, input}, input.shape(),
           /*num_outputs=*/1, lazy_tensors::util::MHash(negative_slope)),
      negative_slope_(negative_slope) {
}

NodePtr LeakyReluBackward::Clone(OpList operands) const {
  return MakeNode<LeakyReluBackward>(operands.at(0), operands.at(1), negative_slope_);
}

std::string LeakyReluBackward::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", negative_slope=" << negative_slope_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
