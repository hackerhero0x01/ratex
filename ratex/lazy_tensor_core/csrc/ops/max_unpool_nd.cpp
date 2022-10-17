/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/max_unpool_nd.h"

#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {
namespace {

c10::Symbol MaxUnpoolNdSymbol(int64_t spatial_dim_count) {
  switch (spatial_dim_count) {
    case 2:
      return at::aten::max_unpool2d;
    case 3:
      return at::aten::max_unpool3d;
    default:
      LTC_ERROR() << "Invalid number of spatial dimensions: " << spatial_dim_count;
  }
}

}  // namespace

MaxUnpoolNd::MaxUnpoolNd(const Value& input, const Value& indices, std::vector<int64_t> output_size)
    : Node(ir::OpKind(MaxUnpoolNdSymbol(output_size.size())), {input, indices},
           /*num_outputs=*/1, lazy_tensors::util::MHash(output_size)),
      output_size_(std::move(output_size)) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr MaxUnpoolNd::Clone(OpList operands) const {
  return MakeNode<MaxUnpoolNd>(operands.at(0), operands.at(1), output_size_);
}

std::string MaxUnpoolNd::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", output_size=(" << absl::StrJoin(output_size_, ", ") << ")";
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
