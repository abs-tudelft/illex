// Copyright 2020 Teratide B.V.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include "illex/client_buffering.h"

namespace illex {

inline static auto Cast(const char* str) -> std::byte* {
  return const_cast<std::byte*>(reinterpret_cast<const std::byte*>(str));
}

static void GetResultFrom(const char* str, std::pair<size_t, size_t>* result) {
  auto* buf = Cast(str);
  JSONBuffer b;
  ASSERT_TRUE(JSONBuffer::Create(buf, strlen(str), &b).ok());
  ASSERT_TRUE(b.SetSize(strlen(str)).ok());
  *result = b.Scan(strlen(str), 0);
}

TEST(Client, Scan) {
  std::pair<size_t, size_t> result;
  GetResultFrom("{}\n", &result);
  ASSERT_EQ(result.first, 1);
  ASSERT_EQ(result.second, 0);
  GetResultFrom("{}\n{}", &result);
  ASSERT_EQ(result.first, 1);
  ASSERT_EQ(result.second, 2);
  GetResultFrom("{}\n\n", &result);
  ASSERT_EQ(result.first, 1);
  ASSERT_EQ(result.second, 0);
  GetResultFrom("\n\n\n", &result);
  ASSERT_EQ(result.first, 0);
  ASSERT_EQ(result.second, 0);
  GetResultFrom("{}", &result);
  ASSERT_EQ(result.first, 0);
  ASSERT_EQ(result.second, 2);
}

}  // namespace illex