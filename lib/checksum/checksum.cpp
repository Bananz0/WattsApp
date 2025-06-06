/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2023 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <Arduino.h>
#endif

#include "checksum.h"  // NOLINT

namespace bfs {
uint16_t Fletcher16::Compute(uint8_t const * const data, const size_t len) {
  if ((len == 0) || (!data)) {
    return 0;
  }
  sum0_ = 0;
  sum1_ = 0;
  for (size_t i = 0; i < len; i++) {
    sum0_ = (sum0_ + data[i]) % 0xFF;
    sum1_ = (sum1_ + sum0_) % 0xFF;
  }
  return sum1_ << 8 | sum0_;
}

void Fletcher16::Reset() {
  sum0_ = 0;
  sum1_ = 0;
}

uint16_t Fletcher16::Update(uint8_t const * const data, const size_t len) {
  if ((len == 0) || (!data)) {
    return sum1_ << 8 | sum0_;
  }
  for (size_t i = 0; i < len; i++) {
    sum0_ = (sum0_ + data[i]) % 0xFF;
    sum1_ = (sum1_ + sum0_) % 0xFF;
  }
  return sum1_ << 8 | sum0_;
}
}  // namespace bfs
