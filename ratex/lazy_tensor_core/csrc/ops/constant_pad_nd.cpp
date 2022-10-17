/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensor_core/csrc/ops/constant_pad_nd.h"

#include "absl/strings/str_join.h"
#include "lazy_tensor_core/csrc/compiler/node_lowering.h"
#include "lazy_tensor_core/csrc/helpers.h"
#include "lazy_tensor_core/csrc/ops/scalar.h"
#include "lazy_tensors/computation_client/debug_macros.h"
#include "lazy_tensors/computation_client/util.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

ConstantPadNd::ConstantPadNd(const Value& input, std::vector<int64_t> pad, const at::Scalar& value)
    : Node(ir::OpKind(at::aten::constant_pad_nd), {input},
           /*num_outputs=*/1, lazy_tensors::util::MHash(pad, ScalarHash(value))),
      pad_(std::move(pad)),
      value_(value) {
  SetShapeDeferred([&]() { return compiler::NodeLowering::Get()->Infer(this); });
}

NodePtr ConstantPadNd::Clone(OpList operands) const {
  return MakeNode<ConstantPadNd>(operands.at(0), pad_, value_);
}

std::string ConstantPadNd::ToString() const {
  std::stringstream ss;
  ss << Node::ToString() << ", pad=(" << absl::StrJoin(pad_, ", ") << ")"
     << ", value=" << value_;
  return ss.str();
}

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
