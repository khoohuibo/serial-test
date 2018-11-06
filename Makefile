CC = gcc
CFLAGS = -g -Wall

TARGET = main

SERIALPORTLIB_DIR = arduino-serial
SERIALPORTLIB_SRCS = $(SERIALPORTLIB_DIR)/arduino-serial-lib.c \

BIN = gss
SRCS = main.c \
  $(SERIALPORTLIB_SRCS)

LIBS  = -lpq
INCLUDES = -I/usr/include/postgresql
all :
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN) $(LIBS) $(INCLUDES)
clean:
	$(RM) $(TARGET)
