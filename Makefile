CXX = g++
TARGET = sysmonitor
SRC = src/main.cpp \
	  src/system/system.cpp \
	  src/system/cpu.cpp \
	  src/system/network.cpp

all:
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)