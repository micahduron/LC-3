GXXFLAGS = -std=c++17 -Wall -Werror
GXX = g++ $(GXXFLAGS)

MODULES = \
  StringTokenizer \
  CharClass \
  StringView \
  LC3Writer

MODULE_SRCS = $(patsubst %, %.cpp, $(MODULES))
MODULE_DEPS = $(patsubst %, %.d, $(MODULES))
MODULE_OBJS = $(patsubst %, %.o, $(MODULES))
MODULE_HEADERS = $(patsubst %, %.h, $(MODULES))

.PHONY: all test debug clean

test:	$(MODULE_OBJS)
	make -C tests

clean:
	make -C tests clean
	rm -f $(MODULE_OBJS)

%.o:	%.cpp
	$(GXX) -MM -MF $(patsubst %.o,%.d, $@) -c $<

-include $(MODULE_DEPS)
