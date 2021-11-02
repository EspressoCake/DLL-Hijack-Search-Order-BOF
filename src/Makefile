BOFNAME := dll_hijack_hunter
CC_x64 := x86_64-w64-mingw32-gcc

all:
	$(CC_x64) -Wno-unused-variable -Wno-multichar -Wno-comment -o ../dist/$(BOFNAME).x64.o -c main.c -masm=intel

clean:
	rm -f ../dist/$(BOFNAME).x64.o