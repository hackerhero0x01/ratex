/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "lazy_tensors/literal.h"
#include "lazy_tensors/primitive_util.h"
#include "lazy_tensors/span.h"

namespace lazy_tensors {

class LiteralUtil {
 public:
  template <typename NativeT>
  static Literal CreateR0(NativeT value);

  template <typename NativeT>
  static Literal CreateR1(lazy_tensors::Span<const NativeT> values);
};

template <typename NativeT>
/* static */ Literal LiteralUtil::CreateR0(NativeT value) {
  Literal literal(ShapeUtil::MakeShape(primitive_util::NativeToPrimitiveType<NativeT>(), {}));
  literal.Set<NativeT>({}, value);
  return literal;
}

template <typename NativeT>
/* static */ Literal LiteralUtil::CreateR1(lazy_tensors::Span<const NativeT> values) {
  Literal literal(ShapeUtil::MakeShape(primitive_util::NativeToPrimitiveType<NativeT>(),
                                       {static_cast<int64_t>(values.size())}));
  literal.PopulateR1(values);
  return literal;
}

}  // namespace lazy_tensors
