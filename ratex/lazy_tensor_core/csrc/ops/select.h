/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "lazy_tensor_core/csrc/ir.h"

namespace torch_lazy_tensors {
namespace ir {
namespace ops {

class Select : public Node {
 public:
  Select(const Value& input, int64_t dim, int64_t start, int64_t end, int64_t stride);

  NodePtr Clone(OpList operands) const override;

  std::string ToString() const override;

  int64_t dim() const {
    return dim_;
  }

  int64_t start() const {
    return start_;
  }

  int64_t end() const {
    return end_;
  }

  int64_t stride() const {
    return stride_;
  }

  static lazy_tensors::Shape MakeSelectShape(const lazy_tensors::Shape& shape, int64_t dim,
                                             int64_t start, int64_t end, int64_t stride);

  static int64_t GetStride(int64_t start, int64_t end, int64_t stride);

 private:
  int64_t dim_;
  int64_t start_;
  int64_t end_;
  int64_t stride_;
};

}  // namespace ops
}  // namespace ir
}  // namespace torch_lazy_tensors
