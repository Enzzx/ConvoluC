CC = gcc
CFLAGS = -Wall -Wextra -O3 -fopenmp -std=gnu99 -D_CRT_SECURE_NO_WARNINGS -DSTBIW_NOTUSED
LDFLAGS = -fopenmp -mthreads

SRCS = src/main.c src/transform.c src/utils.c
OBJS = $(SRCS:.c=.o)

ifdef COMSPEC
    RM = del /q
    EXE = .exe
    RUN_CMD = image_processor.exe
else
    RM = rm -f
    EXE =
    RUN_CMD = ./image_processor
endif

all: image_processor$(EXE)

lib: $(OBJS)
	$(CC) $(CFLAGS) -shared -o filtro.dll $(OBJS) $(LDFLAGS)

image_processor.exe: $(OBJS)
	$(CC) $(CFLAGS) -o image_processor.exe $(OBJS) $(LDFLAGS)

image_processor: $(OBJS)
	$(CC) $(CFLAGS) -o image_processor $(OBJS) $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifdef COMSPEC
	$(RM) src\*.o image_processor.exe
else
	$(RM) src/*.o image_processor
endif

run:
	$(RUN_CMD)

.PHONY: all clean run