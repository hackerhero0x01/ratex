/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lazy_tensors/shape_util.h"

#include "lazy_tensors/core/platform/errors.h"
#include "lazy_tensors/core/platform/hash.h"
#include "lazy_tensors/layout_util.h"

namespace lazy_tensors {

/* static */ int64_t ShapeUtil::TupleElementCount(const Shape& shape) {
  LTC_CHECK(shape.IsTuple()) << shape;
  return shape.tuple_shapes_size();
}

namespace {

// Helper for ForEachSubshape which visits the subshapes of the given shape in
// DFS pre-order starting with the index.
Status ForEachSubshapeHelper(const Shape& shape, const ShapeUtil::StatusVisitorFunction& func,
                             ShapeIndex* index) {
  TF_RETURN_IF_ERROR(func(shape, *index));
  if (shape.IsTuple()) {
    for (int64_t i = 0; i < ShapeUtil::TupleElementCount(shape); ++i) {
      // Track the sub-shape position, which can be used by the visitor
      // function.
      index->push_back(i);
      TF_RETURN_IF_ERROR(
          ForEachSubshapeHelper(ShapeUtil::GetTupleElementShape(shape, i), func, index));
      index->pop_back();
    }
  }
  return Status::OK();
}

}  // namespace

void ShapeUtil::ForEachSubshape(const Shape& shape, const VisitorFunction& func) {
  ShapeIndex index;
  // Can safely ignore error since the visitor closure always returns
  // Status::OK().
  ForEachSubshapeHelper(
      shape,
      [&func](const Shape& subshape, const ShapeIndex& index) {
        func(subshape, index);
        return Status::OK();
      },
      &index)
      .IgnoreError();
}

/*static*/ size_t ShapeUtil::Hash(const Shape& shape) {
  using lazy_tensors::hash;
  using lazy_tensors::Hash64Combine;

  size_t hash_value = hash<PrimitiveType>()(shape.element_type());

  if (shape.tuple_shapes().empty()) {
    for (int i = 0; i < shape.dimensions_size(); ++i) {
      hash_value = Hash64Combine(hash_value, hash<int64_t>()(shape.dimensions(i)));
      hash_value = Hash64Combine(hash_value, hash<bool>()(shape.is_dynamic_dimension(i)));
    }

    hash_value = Hash64Combine(hash_value, LayoutUtil::Hash(shape.layout()));
  } else {
    hash_value = 0;
    for (const Shape& subshape : shape.tuple_shapes()) {
      hash_value = Hash64Combine(hash_value, ShapeUtil::Hash(subshape));
    }
  }

  return hash_value;
}

}  // namespace lazy_tensors
