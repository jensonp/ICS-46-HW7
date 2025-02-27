CXX      = g++
CXXFLAGS := -ggdb -std=c++20 -Wall -Wextra -Werror -Wfatal-errors -pedantic -fsanitize=address,undefined 

SRCDIR  = src
SRCS    = $(SRCDIR)/main.cpp $(SRCDIR)/sorter.cpp
OBJS    = $(SRCS:.cpp=.o)
TARGET  = hw7

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
