//    Copyright The Blocxxi Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

namespace asap {
namespace debug {
namespace ui {

class AbstractApplication {
 public:
  virtual ~AbstractApplication() = default;

  virtual void Init() = 0;
  virtual bool Draw() = 0;
  virtual void ShutDown() = 0;
};

}  // namespace ui
}  // namespace debug
}  // namespace asap
