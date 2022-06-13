CXX     = g++
CXXFLAGS = -Wall -std=c++17
TARGET   = matrix_multiply_challenge
SRC     = matrix_multiply_challenge.cpp

$(TARGET): $(SRC)
        $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
        rm -f $(TARGET)