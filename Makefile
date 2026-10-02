CXX = g++
TARGET = sysmonitor
SRC = src/main.cpp \
	  src/system/system.cpp \
	  src/system/cpu.cpp

all:
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)