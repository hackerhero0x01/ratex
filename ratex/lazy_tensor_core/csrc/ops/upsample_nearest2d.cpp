/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/upsample_nearest2d.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

UpsampleNearest::UpsampleNearest(const Value& input, std::vector<int64_t> output_size)
    : Node(ir::OpKind(at::aten::upsample_nearest2d), {input},
           /*num_outputs=*/1, lazy_tensors::util::MHash(output_size)),
      output_size_(std::move(output_size)) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr UpsampleNearest::Clone(OpList operands) const {
  return MakeNode<UpsampleNearest>(operands.at(0), output_size_);
}

std::string UpsampleNearest::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", output_size=(" << absl::StrJoin(output_size_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
