GXXFLAGS = -std=c++17 -Wall -Werror
GXX = g++ $(GXXFLAGS)

MODULES = \
  StringTokenizer \
  CharClass \
  StringView \
  LC3Writer

MODULE_SRCS = $(patsubst %, %.cpp, $(MODULES))
MODULE_OBJS = $(patsubst %, %.o, $(MODULES))
MODULE_HEADERS = $(patsubst %, %.h, $(MODULES))

.PHONY: all test debug clean

test:	$(MODULE_OBJS)
	make -C tests

clean:
	make -C tests clean
	rm -f $(MODULE_OBJS)

StringTokenizer.o:	StringTokenizer.cpp StringTokenizer.h CharClass.h StringView.h
	$(GXX) -c $<

LC3Writer.o:	LC3Writer.cpp LC3Writer.h util/BinaryFileIO.h
	$(GXX) -c $<

%.o:	%.cpp %.h
	$(GXX) -c $<

%.h:	%.tpp
	touch $@
