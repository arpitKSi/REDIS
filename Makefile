CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wno-unused-variable -Iinclude

SRCS = src/14_server.cpp \
       src/avl.cpp \
       src/hashtable.cpp \
       src/zset.cpp \
       src/heap.cpp \
       src/thread_pool.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = redis_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
