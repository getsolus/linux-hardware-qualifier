MESON_OPTS := --buildtype debug -Db_sanitize=none -Db_lundef=false
CC := clang

all: build

build: BUILDDIR
	ninja -C BUILDDIR

BUILDDIR:
	CC=$(CC) meson $(MESON_OPTS) BUILDDIR

reconfigure:
	meson $(MESON_OPTS) --reconfigure BUILDDIR

install:
	ninja -C BUILDDIR install

clean:
	rm -rf BUILDDIR || exit 0

test:
	valgrind --leak-check=full --show-leak-kinds=all ./BUILDDIR/src/linux-hardware-qualifier
	/usr/bin/time ./BUILDDIR/src/linux-hardware-qualifier
