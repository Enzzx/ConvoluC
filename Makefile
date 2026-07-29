CC = gcc
CFLAGS = -Wall -Wextra -O3 -fopenmp -fPIC -std=gnu99 -D_CRT_SECURE_NO_WARNINGS -DSTBIW_NOTUSED
LDFLAGS = -fopenmp

SRCS = src/transform.c src/utils.c
OBJS = $(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET = image_processor.dll
    RM = del /q
else
    TARGET = image_processor.so
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	$(RM) src\*.o $(TARGET)
else
	$(RM) src/*.o $(TARGET)
endif

.PHONY: all clean