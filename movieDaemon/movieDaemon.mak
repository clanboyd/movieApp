CC=arm-linux-gnueabihf-g++
INCLUDES=-Iinclude  -I/home/yoda/movieApp/common/include
CFLAGS=-c -Wall -DLINUX -Wno-deprecated -pthread -g
LDFLAGS=-lm -lpthread
TARGET=movieDaemon
SOURCES= src/MovieDaemon.cpp \
		 src/MovieDb.cpp \
		 src/Movies.cpp \
		 src/People.cpp \
         src/main.cpp

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
	scp src/*.cpp yoda@raspberrypi:/home/yoda/movieApp/movieDaemon/src
	scp include/*.h yoda@raspberrypi:/home/yoda/movieApp/movieDaemon/include
	scp data/*.csv yoda@raspberrypi:/home/yoda/movieApp/movieDaemon/data
	scp movieDaemon.mak yoda@raspberrypi:/home/yoda/movieApp/movieDaemon/movieDaemon.mak

