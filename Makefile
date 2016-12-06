GXXFLAGS = -std=c++17 -Wall -Werror
GXX = g++ $(GXXFLAGS)

MODULES = Tokenizer CharClass StringView

MODULE_SRCS = $(patsubst %, %.cpp, $(MODULES))
MODULE_OBJS = $(patsubst %, %.o, $(MODULES))
MODULE_HEADERS = $(patsubst %, %.h, $(MODULES))

.PHONY: all test debug clean

test:	$(MODULE_OBJS)
	make -C tests

clean:
	make -C tests clean
	rm -f $(MODULE_OBJS)

Tokenizer.o:	Tokenizer.cpp Tokenizer.h CharClass.h StringView.h
	$(GXX) -c $<

%.o:	%.cpp %.h
	$(GXX) -c $<
