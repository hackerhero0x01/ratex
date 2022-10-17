/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/get_dimensions_size.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/ops/ltc_ops.h"
#include "lazy_tensor_core/csrc/tensor_util.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/shape_util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

GetDimensionsSize::GetDimensionsSize(const Value& input, std::vector<int64_t> dimensions)
    : Node(ltc_get_dimensions_size, {input},
           lazy_tensors::ShapeUtil::MakeShape(GetShapeDimensionType(/*device=*/nullptr), {}),
           /*num_outputs=*/1, lazy_tensors::util::MHash(dimensions)),
      dimensions_(std::move(dimensions)) {
}

NodePtr GetDimensionsSize::Clone(OpList operands) const {
  return MakeNode<GetDimensionsSize>(operands.at(0), dimensions_);
}

std::string GetDimensionsSize::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", dimensions=(" << absl::StrJoin(dimensions_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
