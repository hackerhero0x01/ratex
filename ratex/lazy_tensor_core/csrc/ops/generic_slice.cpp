/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/generic_slice.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/ops/ltc_ops.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

GenericSlice::GenericSlice(const Value& input, lazy_tensors::Span<const int64_t> base_indices,
                           lazy_tensors::Span<const int64_t> sizes)
    : Node(ltc_generic_slice, {input},
           /*num_outputs=*/1, lazy_tensors::util::MHash(base_indices, sizes)),
      base_indices_(base_indices.begin(), base_indices.end()),
      sizes_(sizes.begin(), sizes.end()) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr GenericSlice::Clone(OpList operands) const {
  return MakeNode<GenericSlice>(operands.at(0), base_indices_, sizes_);
}

std::string GenericSlice::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", base_indices=(" << absl::StrJoin(base_indices_, ", ") << "), sizes=("
     << absl::StrJoin(sizes_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
