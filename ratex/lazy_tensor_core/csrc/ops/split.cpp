/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/split.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/data_ops.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

Split::Split(const Value& input, std::vector<int64_t> split_sizes, int64_t dim)
    : Node(ir::OpKind(at::aten::split), {input},
           ComputeSplitCount(input.shape().dimensions(dim), split_sizes),
           lazy_tensors::util::MHash(split_sizes, dim)),
      split_sizes_(std::move(split_sizes)),
      dim_(dim) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr Split::Clone(OpList operands) const {
  return MakeNode<Split>(operands.at(0), split_sizes_, dim_);
}

std::string Split::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", split_sizes=(" << absl::StrJoin(split_sizes_, ", ")
     << "), dim=" << dim_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
