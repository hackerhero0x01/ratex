/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/all_to_all.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/ops/ltc_ops.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/shape_util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

AllToAll::AllToAll(const Value& input, const Value& token, int64_t split_dimension,
                   int64_t concat_dimension, int64_t split_count,
                   std::vector<std::vector<int64_t>> groups)
    : Node(ltc_all_to_all, {input, token},
           /*num_outputs=*/2,
           lazy_tensors::util::MHash(split_dimension, concat_dimension, split_count, groups)),
      split_dimension_(split_dimension),
      concat_dimension_(concat_dimension),
      split_count_(split_count),
      groups_(std::move(groups)) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr AllToAll::Clone(OpList operands) const {
  return MakeNode<AllToAll>(operands.at(0), operands.at(1), split_dimension_, concat_dimension_,
                            split_count_, groups_);
}

std::string AllToAll::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", split_dimension=" << split_dimension_
     << ", concat_dimension=" << concat_dimension_ << ", split_count=" << split_count_
     << ", groups=(";
  for (size_t i = 0; i < groups_.size(); ++i) {
    ss << (i == 0 ? "(" : ",(");
    ss << absl::StrJoin(groups_[i], ", ") << ")";
  }
  ss << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
