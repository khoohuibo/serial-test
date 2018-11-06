CC = gcc
CFLAGS = -Og -Wno-format-truncation

SERIALPORTLIB_DIR = arduino-serial
SERIALPORTLIB_SRCS = $(SERIALPORTLIB_DIR)/arduino-serial-lib.c \

BIN = serial_test_client
SRCS = main.c \
	config.c \
	minIni.c \
  $(SERIALPORTLIB_SRCS)

LIBS  = -lpq
INCLUDES = -I/usr/include/postgresql
all :
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN) $(LIBS) $(INCLUDES)
clean:
	rm -fv *.o $(BIN)
