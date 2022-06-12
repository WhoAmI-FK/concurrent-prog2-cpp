CXX     = g++
CXXFLAGS        = -Wall -std=c++17
TARGET  = measure_speedup_demo
SRC     = measure_speedup_demo.cpp

$(TARGET): $(SRC)
        $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
        rm -f $(TARGET)