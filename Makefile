# File              : Makefile
# Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
# Date              : 11.10.2021
# Last Modified Date: 14.10.2021
# Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
#

LDFLAGS="-L/opt/homebrew/opt/openssl@3/lib"
CFLAGS="-I/opt/homebrew/opt/openssl@3/include"

LDFLAGSWIN=-L"./third-party/openssl-3.0.0-win32-mingw/lib"
CFLAGSWIN=-I"./third-party/openssl-3.0.0-win32-mingw/include"


all: run

build/egisz: *.c
	mkdir -p build
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS) -lssl -lcrypto

build/windows/egisz.exe: *.c
	mkdir -p build/windows
	i686-w64-mingw32-gcc -shared ./third-party/openssl-3.0.0-win32-mingw/lib/libcrypto.dll/*.o -o build/windows/libcrypto-3.dll
	i686-w64-mingw32-gcc -shared ./third-party/openssl-3.0.0-win32-mingw/lib/libssl.dll/*.o -o build/windows/libssl-3.dll
	#cd build/windows; ar -x libcrypto.dll.a; ar -x libssl.dll.a; cd ../../;
	i686-w64-mingw32-gcc $^ -o $@ $(CFLAGSWIN) $(LDFLAGSWIN) -lWs2_32 -lssl -lcrypto 
	#cp -f ./third-party/openssl-3.0.0-win32-mingw/lib/libssl.dll.a build/windows/libssl-3.dll.a
	#cp -f ./third-party/openssl-3.0.0-win32-mingw/lib/libcrypto.dll.a build/windows/libcrypto-3.dll.a


clean:
	rm -fr build

run: build/egisz
	open build/egisz

winrun: build/windows/egisz.exe
	echo "#!/bin/bash">run
	echo "/Users/kuzmich/Applications/Wineskin/Wine.app/Contents/MacOS/wineskinlauncher $(PWD)/build/windows/egisz.exe">>run
	chmod +x run
	open run
	

.Phony: run clean
