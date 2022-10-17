/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <c10/core/Scalar.h>

#include "lazy_tensor_core/csrc/ir.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

class ShrinkBackward : public Node {
 public:
  ShrinkBackward(OpKind kind, const Value& grad_output, const Value& input,
                 const at::Scalar& lambda);

  std::string ToString() const override;

  NodePtr Clone(OpList operands) const override;

  at::Scalar lambda() const {
    return lambda_;
  }

 private:
  at::Scalar lambda_;
};

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
