/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/avg_pool_nd.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {
namespace {

c10::Symbol AvgPoolNdSymbol(int64_t spatial_dim_count) {
  switch (spatial_dim_count) {
    case 1:
      return at::aten::avg_pool1d;
    case 2:
      return at::aten::avg_pool2d;
    case 3:
      return at::aten::avg_pool3d;
    default:
      LTC_ERROR() << "Invalid number of spatial dimensions: " << spatial_dim_count;
  }
}

}  // namespace

AvgPoolNd::AvgPoolNd(const Value& input, int64_t spatial_dim_count,
                     std::vector<int64_t> kernel_size, std::vector<int64_t> stride,
                     std::vector<int64_t> padding, bool ceil_mode, bool count_include_pad)
    : Node(ir::OpKind(AvgPoolNdSymbol(spatial_dim_count)), {input},
           /*num_outputs=*/1,
           lazy_tensors::util::MHash(spatial_dim_count, kernel_size, stride, padding, ceil_mode,
                                     count_include_pad)),
      spatial_dim_count_(spatial_dim_count),
      kernel_size_(std::move(kernel_size)),
      stride_(std::move(stride)),
      padding_(std::move(padding)),
      ceil_mode_(ceil_mode),
      count_include_pad_(count_include_pad) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr AvgPoolNd::Clone(OpList operands) const {
  return MakeNode<AvgPoolNd>(operands.at(0), spatial_dim_count_, kernel_size_, stride_, padding_,
                             ceil_mode_, count_include_pad_);
}

std::string AvgPoolNd::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", spatial_dim_count=" << spatial_dim_count_ << ", kernel_size=("
     << absl::StrJoin(kernel_size_, ", ") << "), stride=(" << absl::StrJoin(stride_, ", ")
     << "), padding=(" << absl::StrJoin(padding_, ", ")
     << "), count_include_pad=" << count_include_pad_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
