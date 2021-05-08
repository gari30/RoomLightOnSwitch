CC     = gcc
CFLAGS = -Wall -pthread -std=c++11
LIBS   = -lstdc++ -lpigpiod_if2 -lrt
SRC    = sw_light-on.cpp
OBJ    = $(SRC:%.cpp=%.o)
TARGET = sw_light-on

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

all: clean $(OBJ) $(TARGET)

clean:
	-rm -f $(OBJ) $(TARGET) *.d

