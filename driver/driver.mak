CC=arm-linux-gnueabihf-g++
INCLUDES=-Iinclude -I/home/yoda/movieApp/reader/include -I/home/yoda/movieApp/common/include
CFLAGS=-c -Wall -DLINUX -Wno-deprecated -pthread -g
LDFLAGS=-lm -lpthread
TARGET=lcdDriver
LIB=/home/yoda/movieApp/common/lib/libbcm2835.a
SOURCES= LcdDriver.cpp \
         /home/yoda/movieApp/reader/src/Lcd.cpp

OBJS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIB)

.cpp.o: 
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(TARGET)
	rm -rf *.o

