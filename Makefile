# File              : Makefile
# Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
# Date              : 11.10.2021
# Last Modified Date: 14.10.2021
# Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
#

LDFLAGS="-L/opt/homebrew/opt/openssl@3/lib"
CFLAGS="-I/opt/homebrew/opt/openssl@3/include"


all: run

build/egisz: *.c
	mkdir -p build
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS) -lssl -lcrypto

clean:
	rm -fr build

run: build/egisz
	open build/egisz

.Phony: run clean
