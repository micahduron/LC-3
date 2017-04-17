CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror
CXX ?= g++

COMPILE := $(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA)

MODULES = \
  StringTokenizer \
  CharClass \
  StringView \
  LC3Writer \
  LC3Reader

MODULE_SRCS = $(patsubst %, %.cpp, $(MODULES))
MODULE_DEPS = $(patsubst %, %.d, $(MODULES))
MODULE_OBJS = $(patsubst %, %.o, $(MODULES))

.PHONY: all test debug clean

test:	$(MODULE_OBJS)
	make -C tests

clean:
	make -C tests clean
	rm -f $(MODULE_OBJS) $(MODULE_DEPS)

%.o:	%.cpp
	$(COMPILE) -MM -MF $(patsubst %.o,%.d,$@) $<
	$(COMPILE) -c $<

-include $(MODULE_DEPS)
