CXX = g++
TARGET = sysmonitor
SRC = src/main.cpp \
	  src/system/system.cpp

all:
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)