GXXFLAGS = -std=c++17 -Wall -Werror
GXX = g++ $(GXXFLAGS)

MODULES = StringTokenizer CharClass StringView
MODULES_DIR = ..

MODULE_SRCS = $(patsubst %, $(MODULES_DIR)/%.cpp, $(MODULES))
MODULE_OBJS = $(patsubst %.cpp, %.o, $(MODULE_SRCS))
MODULE_HEADERS = $(patsubst %.cpp, %.h, $(MODULE_SRCS))

TEST_EXECS = $(patsubst %, %_test, $(MODULES))
TEST_OBJS = $(patsubst %, %.o, $(TEST_EXECS))

.PHONY: all test debug clean

test:	$(TEST_EXECS)

clean:
	rm -f $(TEST_OBJS) $(TEST_EXECS)

$(TEST_EXECS): %:	%.o $(MODULE_OBJS)
	$(GXX) -o $@ $^

$(TEST_OBJS): %.o:	%.cpp $(MODULE_HEADERS)
	$(GXX) -c $<
