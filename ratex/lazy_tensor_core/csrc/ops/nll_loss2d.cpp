/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/nll_loss2d.h"

#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/span.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

NllLoss2d::NllLoss2d(const Value& logits, const Value& labels, const absl::optional<Value>& weight,
                     ReductionMode reduction, int ignore_index)
    : Node(ir::OpKind(at::aten::nll_loss2d),
           lazy_tensors::util::GetValuesVector<Value>({logits, labels}, {&weight}),
           /*num_outputs=*/1,
           lazy_tensors::util::MHash(lazy_tensors::util::GetEnumValue(reduction), ignore_index)),
      reduction_(reduction),
      ignore_index_(ignore_index) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr NllLoss2d::Clone(OpList operands) const {
  absl::optional<Value> weight;
  if (operands.size() > 2) {
    weight = operands.at(2);
  }
  return MakeNode<NllLoss2d>(operands.at(0), operands.at(1), weight, reduction_, ignore_index_);
}

std::string NllLoss2d::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", reduction=" << lazy_tensors::util::GetEnumValue(reduction_)
     << ", ignore_index=" << ignore_index_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
