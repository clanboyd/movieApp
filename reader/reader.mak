CC=arm-linux-gnueabihf-g++
INCLUDES=-Iinclude -I/home/yoda/movieApp/reader/include -I/home/yoda/movieApp/common/include
CFLAGS=-c -Wall -DLINUX -Wno-deprecated -pthread -g
LDFLAGS=-lm -lpthread
TARGET=reader
LIB=/home/yoda/movieApp/common/lib/libbcm2835.a
SOURCES= src/MySerial.cpp \
         src/Controls.cpp \
         src/Lcd.cpp \
         src/ClientSocket.cpp \
         src/reader.cpp

OBJS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIB)

.cpp.o: 
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(TARGET)
	rm -rf src/*.o

install:
	scp src/*cpp yoda@raspberrypi:/home/yoda/movieApp/reader/src
	scp include/*h yoda@raspberrypi:/home/yoda/movieApp/reader/include
	scp reader yoda@raspberrypi:/home/yoda/movieApp/reader/
	scp reader.mak yoda@raspberrypi:/home/yoda/movieApp/reader/
