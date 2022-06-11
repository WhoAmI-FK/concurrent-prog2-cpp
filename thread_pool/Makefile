CXX     = g++
CXXFLAGS = -Wall
TARGET  = thread_pool
SRC     = thread_pool.cpp
LDLIB   = -lboost_thread -lboost_system

$(TARGET) : $(SRC)
        $(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET)

.PHONY: clean
clean:
        rm -f $(TARGET)