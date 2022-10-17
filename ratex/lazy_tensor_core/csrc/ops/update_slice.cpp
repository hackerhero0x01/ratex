/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/update_slice.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/ops/ltc_ops.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

UpdateSlice::UpdateSlice(const Value& input, const Value& source,
                         lazy_tensors::Span<const int64_t> base_indices)
    : Node(ltc_update_slice, {input, source},
           /*num_outputs=*/1, lazy_tensors::util::MHash(base_indices)),
      base_indices_(base_indices.begin(), base_indices.end()) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr UpdateSlice::Clone(OpList operands) const {
  return MakeNode<UpdateSlice>(operands.at(0), operands.at(1), base_indices_);
}

std::string UpdateSlice::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", base_indices=(" << absl::StrJoin(base_indices_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
