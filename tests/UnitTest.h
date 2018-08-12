#pragma once

#include <iostream>
#include <functional>
#include <vector>

enum class TestState {
    Success,
    Fail,
    Unknown
};

class TestContext {
public:
    TestState state() const {
        return m_state;
    }

    void success() {
        m_state = TestState::Success;
    }

    void fail() {
        m_state = TestState::Fail;
    }

    void succeedIf(bool status) {
        status ? success() : fail();
    }

    void failIf(bool status) {
        succeedIf(!status);
    }

private:
    TestState m_state = TestState::Unknown;
};

using TestFunction = std::function<void(TestContext&)>;

struct TestDescriptor {
    const char* testName = nullptr;
    TestFunction testFn;

    TestDescriptor(const char* name) :
      testName{ name }
    {}
};

static std::vector<TestDescriptor> g_UnitTests;

#define _UnitTestMake(TestName) \
    g_UnitTests.emplace_back(#TestName)

#define UnitTest(TestName, TestContextName) \
    _UnitTestMake(TestName).testFn = \
      [](TestContext& TestContextName) -> void

#define UnitTestFn(TestName, TestFunc) \
    _UnitTestMake(TestName).testFn = TestFunc

static int RunTests() {
    size_t successCount = 0;
    size_t failCount = 0;

    for (const auto& test : g_UnitTests) {
        TestContext ctx;
        test.testFn(ctx);

        std::cout << test.testName << ": ";

        switch (ctx.state()) {
            case TestState::Success:
                std::cout << "passed\n";

                ++successCount;
                break;
            case TestState::Fail:
                std::cout << "failed\n";

                ++failCount;
                break;
            default:
                throw std::logic_error("Test returned with an unknown state.");
        }
    }
    return failCount == 0 ? 0 : 1;
}
