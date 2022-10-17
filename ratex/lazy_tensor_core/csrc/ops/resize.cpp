/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/resize.h"

#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"
#include "lazy_tensors/shape_util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {
namespace {

lazy_tensors::Shape NodeOutputShape(const Value& input, lazy_tensors::Span<const int64_t> size) {
  return lazy_tensors::ShapeUtil::MakeShape(input.shape().element_type(), size);
}

}  // namespace

Resize::Resize(const Value& input, std::vector<int64_t> size)
    : Node(
          ir::OpKind(at::aten::resize), {input}, [&]() { return NodeOutputShape(input, size); },
          /*num_outputs=*/1, lazy_tensors::util::MHash(size)),
      size_(std::move(size)) {
}

NodePtr Resize::Clone(OpList operands) const {
  return MakeNode<Resize>(operands.at(0), size_);
}

std::string Resize::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", size=(" << absl::StrJoin(size_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
