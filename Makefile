CXX = g++
TARGET = sysmonitor
SRC = src/main.cpp

all:
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)