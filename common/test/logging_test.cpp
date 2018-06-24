//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <catch2/catch.hpp>

#include <common/logging.h>


namespace asap {
namespace logging {

TEST_CASE("TestLoggable", "[common][logging]") {
  class Foo : Loggable<Id::TESTING> {
   public:
    Foo() { ASLOG(trace, "Foo construcor"); }
  };

  ASLOG_MISC(info, "Hello World!");
  Foo foo;
}

TEST_CASE("TestMultipleThreads", "[common][logging]") {
  std::thread th1([]() {
    for (auto ii = 0; ii < 5; ++ii)
      ASLOG_MISC(debug, "Logging from thread 1: {}", ii);
  });
  std::thread th2([]() {
    auto &test_logger = Registry::GetLogger(Id::TESTING);
    for (auto ii = 0; ii < 5; ++ii)
      ASLOG_TO_LOGGER(test_logger, trace, "Logging from thread 2: {}", ii);
  });
  th1.join();
  th2.join();
}

TEST_CASE("TestLogWithPrefix", "[common][logging]") {
  auto &test_logger = Registry::GetLogger(Id::TESTING);

  AS_DO_LOG(test_logger, debug, "message");
  AS_DO_LOG(test_logger, debug, "message {}", 1);
  AS_DO_LOG(test_logger, debug, "message {} {}", 1, 2);
  AS_DO_LOG(test_logger, debug, "message {} {} {}", 1, 2, 3);
  AS_DO_LOG(test_logger, debug, "message {} {} {} {}", 1, 3, 3, 4);
}

class MockSink : public spdlog::sinks::sink {
public:
  void log(const spdlog::details::log_msg &) {
    ++called_;
  }
  void flush() {}
  void Reset() { called_ = 0; }

  int called_{0};
};

TEST_CASE("TestLogPushSink", "[common][logging]") {
  spdlog::sinks::sink *first_mock = new MockSink();
  spdlog::sinks::sink *second_mock = new MockSink();
	auto first_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(first_mock);
	auto second_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(second_mock);

	auto &test_logger = Registry::GetLogger(Id::TESTING);
	Registry::PushSink(first_sink_ptr);
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(dynamic_cast<MockSink*>(first_sink_ptr.get())->called_ == 1);

	Registry::PushSink(second_sink_ptr);
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(dynamic_cast<MockSink*>(first_sink_ptr.get())->called_ == 0);
  REQUIRE(dynamic_cast<MockSink*>(second_sink_ptr.get())->called_ == 1);

  Registry::PopSink();

	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(dynamic_cast<MockSink*>(first_sink_ptr.get())->called_ == 1);
  REQUIRE(dynamic_cast<MockSink*>(second_sink_ptr.get())->called_ == 0);

	Registry::PopSink();

	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(dynamic_cast<MockSink*>(first_sink_ptr.get())->called_ == 0);
  REQUIRE(dynamic_cast<MockSink*>(second_sink_ptr.get())->called_ == 0);
}

}  // namespace logging
}  // namespace asap
