CC := clang
CFLAGS := -Wall -Wextra
TARGET := pickle

OBJ := main.o pickle.o

build/$(TARGET): $(OBJ)
	mkdir -p build/
	$(CC) -o build/$(TARGET) $(OBJ) $(CFLAGS)

main.o: src/main.c src/pickle.h
	$(CC) -c src/main.c $(CFLAGS)

pickle.o: src/pickle.c src/pickle.h
	$(CC) -c src/pickle.c $(CFALGS)

run: build/$(TARGET)
	build/$(TARGET)

clean:
	-rm -rf build *.o