.SUFFIXES : .c .o

OBJECTS = main.o sdes.o
SRCS = main.c sdes.c

CC = gcc
CFLAGS =-std=c99
CFLAGS+=-W -Wall

TARGET = sdes_test_app 

all : $(TARGET)

$(TARGET) : $(OBJECTS)
			  $(CC) -o $@ $(OBJECTS)
					   rm -rf $(OBJECTS)
clean :
		rm -rf $(OBJECTS) $(TARGET)
