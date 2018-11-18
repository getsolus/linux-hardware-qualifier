CC := clang

all: build

build: BUILDDIR
	ninja -C BUILDDIR

BUILDDIR:
	CC=$(CC) meson --buildtype debug BUILDDIR

reconfigure:
	meson --reconfigure --buildtype debug BUILDDIR

install:
	ninja -C BUILDDIR install

clean:
	rm -rf BUILDDIR || exit 0

test:
	./BUILDDIR/src/linux-hardware-qualifier
