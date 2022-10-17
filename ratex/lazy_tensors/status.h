/*
 * Copyright (c) 2018 Google Inc. All Rights Reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Modifications Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <c10/util/Logging.h>

#include <ostream>

namespace lazy_tensors {

class Status {
 public:
  Status(const char*) {
  }

  static Status OK() {
    static const Status ok = Status(nullptr);
    return ok;
  }

  bool ok() const {
    return true;
  }

  void IgnoreError() const {
    // no-op
  }
};

inline std::ostream& operator<<(std::ostream& os, const Status& primitive_type) {
  LOG(FATAL) << "Not implemented yet.";
}

}  // namespace lazy_tensors
