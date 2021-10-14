# File              : Makefile
# Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
# Date              : 11.10.2021
# Last Modified Date: 14.10.2021
# Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
#

LDFLAGS="-L/opt/homebrew/opt/openssl@3/lib"
CFLAGS="-I/opt/homebrew/opt/openssl@3/include"

LDFLAGSWIN=-L"./third-party/openssl-3.0.0-win32-mingw/lib"
CFLAGSWIN=-I"./third-party/openssl-3.0.0-win32-mingw/include/openssl"


all: run

build/egisz: *.c
	mkdir -p build
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS) -lssl -lcrypto

build/windows/egisz.exe: *.c
	mkdir -p build/windows
	i686-w64-mingw32-gcc $^ -o $@ $(CFLAGSWIN) $(LDFLAGSWIN) -lssl -lcrypto


clean:
	rm -fr build

run: build/egisz
	open build/egisz

winrun: build/windows/egisz.exe
	open build/windows/egisz.exe

.Phony: run clean
