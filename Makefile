MESON_OPTS := --buildtype debug -Db_sanitize=none -Db_lundef=false
CC := clang
PREFIX ?= /usr
DATADIR ?= $(PREFIX)/share/linux-hardware-qualifier

all: build

build: BUILDDIR
	ninja -C BUILDDIR

BUILDDIR:
	CC=$(CC) meson $(MESON_OPTS) BUILDDIR

reconfigure:
	meson $(MESON_OPTS) --reconfigure BUILDDIR

install: download
	ninja -C BUILDDIR install
	install -Dm00644 data/lkddb.list $(DESTDIR)$(DATADIR)/lkddb.list
	install -Dm00644 data/ids.list $(DESTDIR)$(DATADIR)/ids.list

clean:
	rm -rf BUILDDIR || exit 0
	rm -rf data || exit 0

check:
	meson test -C BUILDDIR -q

valgrind:
	meson test -C BUILDDIR --wrap=valgrind -v

bench:
	meson test -C BUILDDIR --benchmark

download:
	install -d data
	if [[ ! -a data/counts ]]; then \
	    (cd data && wget https://cateee.net/sources/lkddb/counts); \
	fi
	if [[ ! -a data/ids.list ]]; then \
	    (cd data && wget https://cateee.net/sources/lkddb/ids.list); \
	fi
	if [[ ! -a data/lkddb.list ]]; then \
	    (cd data && wget https://cateee.net/sources/lkddb/lkddb.list); \
	fi
