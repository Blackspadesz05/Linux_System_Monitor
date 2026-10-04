CXX = g++
TARGET = sysmonitor
SRC = src/main.cpp \
	  src/system/system.cpp \
	  src/system/cpu.cpp \
      src/util.cpp \
      src/system/alerts.cpp \
      src/process/process.cpp \
	  src/network/network.cpp

all:
	$(CXX) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)