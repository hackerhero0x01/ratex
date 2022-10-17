/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/binary_cross_entropy_backward.h"

#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

BinaryCrossEntropyBackward::BinaryCrossEntropyBackward(const Value& grad_output,
                                                       const Value& logits, const Value& labels,
                                                       const absl::optional<Value>& weight,
                                                       ReductionMode reduction)
    : Node(ir::OpKind(at::aten::binary_cross_entropy_backward),
           lazy_tensors::util::GetValuesVector<Value>({grad_output, logits, labels}, {&weight}),
           /*num_outputs=*/1,
           lazy_tensors::util::MHash(lazy_tensors::util::GetEnumValue(reduction))),
      reduction_(reduction) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr BinaryCrossEntropyBackward::Clone(OpList operands) const {
  absl::optional<Value> weight;
  if (operands.size() > 3) {
    weight = operands.at(3);
  }
  return MakeNode<BinaryCrossEntropyBackward>(operands.at(0), operands.at(1), operands.at(2),
                                              weight, reduction_);
}

std::string BinaryCrossEntropyBackward::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", reduction=" << lazy_tensors::util::GetEnumValue(reduction_);
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
