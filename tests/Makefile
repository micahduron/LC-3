GXXFLAGS = -std=c++17 -Wall -Wextra -Werror
GXX = g++ $(GXXFLAGS)

MODULES_DIR = ..

MODULE_OBJS = $(wildcard $(MODULES_DIR)/*.o)

TEST_SRCS = $(wildcard *_test.cpp)
TEST_EXECS = $(patsubst %.cpp, %, $(TEST_SRCS))
TEST_DEPS = $(patsubst %, %.d, $(TEST_EXECS))
TEST_OBJS = $(patsubst %, %.o, $(TEST_EXECS))

.PHONY: all test debug clean

test:	$(TEST_EXECS)

clean:
	rm -f $(TEST_OBJS) $(TEST_EXECS) $(TEST_DEPS)

$(TEST_EXECS): %:	%.o $(MODULE_OBJS)
	$(GXX) -o $@ $^

-include $(TEST_DEPS)

%.o:	%.cpp
	$(GXX) -MM -MF $(patsubst %.o,%.d,$@) $<
	$(GXX) -c $<
