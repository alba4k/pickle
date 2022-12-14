CC := clang
CFLAGS := -Wall -Wextra
TARGET := pickle

OBJ := main.o pickle.o

OS := $(shell uname -s 2> /dev/null)

ifeq ($(OS),Linux)
	INSTALLPATH := /usr/bin
endif

ifeq ($(OS),Darwin)
	INSTALLPATH := /usr/local/bin
endif

build/$(TARGET): $(OBJ)
	mkdir -p build/
	$(CC) -o build/$(TARGET) $(OBJ) $(CFLAGS)

main.o: src/main.c src/pickle.h
	$(CC) -c src/main.c $(CFLAGS)

pickle.o: src/pickle.c src/pickle.h
	$(CC) -c src/pickle.c $(CFALGS)

run: build/$(TARGET)
	build/$(TARGET)

install: build/$(TARGET)

install: build/$(TARGET)
	cp -f build/$(TARGET) $(INSTALLPATH)/$(TARGET)	

uninstall:
	rm $(INSTALLPATH)/$(TARGET) 

clean:
	-rm -rf build *.o