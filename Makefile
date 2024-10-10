CXX = g++
CXXFLAGS = -std=c++11 -pthread

TARGET = tarea1

all: $(TARGET)

$(TARGET): tarea1.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) tarea1.o

tarea1.o: tarea1.cpp
	$(CXX) $(CXXFLAGS) -c tarea1.cpp

clean:
	rm -f *.o $(TARGET)