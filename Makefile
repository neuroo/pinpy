SRCS=src/main.cpp src/pinpy.cpp src/utils.cpp src/pinfunction.cpp src/pinfunctionprototype.cpp
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
EXEC=pinpy-bin
CC=clang++
CFLAGS=-g -Wall -O2 -fno-strict-aliasing -std=c++98 -fPIC
OFLAGS=-lxml2 -lpcrecpp -Wl,-all_load 
LFLAGS=-L/opt/local/lib/
INC=-I/opt/local/include/libxml2 -I/opt/local/include -I.

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

$(EXEC) : $(OBJS)
	$(CC) -o $(EXEC) $(OFLAGS) $(OBJS) $(LFLAGS)

.PHONY: clean
clean:
	@rm -f $(OBJS) $(EXEC) $(ARNAME)
