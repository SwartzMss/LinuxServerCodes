CXXFLAGS = -g3  -O0 -w -fPIC -std=c++11
CXX = g++
CC = gcc 

BUILDDIR = ./src
TARGET = $(BUILDDIR)/Server
TARGETDIR = ./Bin

SRC = $(wildcard src/*.c src/*.cpp src/*.cc)

OBJS = $(patsubst %.cpp,%.o,$(SRC))   

INCLUDES = -I./ -I./src -I./depend/xlogger/include

LIBDIR = -Wl,-rpath=./ -lpthread -ldl -L./depend/xlogger/lib -llog4cxx -lxlogger


all:$(TARGET)
	
$(TARGET): $(OBJS)
	$(CXX)   $(LIBDIR) -o $@ $^ 
	
%.o : %.cpp
	$(CXX) $(INCLUDES) -c $(CXXFLAGS) $< -o $@

%.o : %.c
	$(CXX) $(INCLUDES) -c $(CXXFLAGS) $< -o $@

%.o : %.cc
	$(CXX) $(INCLUDES) -c $(CXXFLAGS) $< -o $@

clean:
	@rm -rvf $(OBJS) $(TARGET)
	@rm -rf $(TARGETDIR)/*

install:$(TARGET)
	install -d $(TARGETDIR)
	install -m 0755 $(TARGET)  $(TARGETDIR)
	@install -m 0755 ./depend/xlogger/lib/*  $(TARGETDIR)
	@install -m 0755 src/log4cxx.properties  $(TARGETDIR)
