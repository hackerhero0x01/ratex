/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef COMPUTATION_CLIENT_UNIQUE_H_
#define COMPUTATION_CLIENT_UNIQUE_H_

#include <functional>
#include <set>

#include "absl/types/optional.h"
#include "lazy_tensors/computation_client/debug_macros.h"

namespace lazy_tensors {
namespace util {

// Helper class to allow tracking zero or more things, which should be forcibly
// be one only thing.
template <typename T, typename C = std::equal_to<T>>
class Unique {
 public:
  std::pair<bool, const T&> set(const T& value) {
    if (value_) {
      LTC_CHECK(C()(*value_, value)) << "'" << *value_ << "' vs '" << value << "'";
      return std::pair<bool, const T&>(false, *value_);
    }
    value_ = value;
    return std::pair<bool, const T&>(true, *value_);
  }

  operator bool() const {
    return value_.has_value();
  }
  operator const T&() const {
    return *value_;
  }
  const T& operator*() const {
    return *value_;
  }
  const T* operator->() const {
    return value_.operator->();
  }

  std::set<T> AsSet() const {
    std::set<T> vset;
    if (value_.has_value()) {
      vset.insert(*value_);
    }
    return vset;
  }

 private:
  absl::optional<T> value_;
};

}  // namespace util
}  // namespace lazy_tensors

#endif  // COMPUTATION_CLIENT_UNIQUE_H_
