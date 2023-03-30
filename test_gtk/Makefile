
PKG-CONFIG = $(shell which pkg-config)

CFLAGS = $(shell $(PKG-CONFIG) --cflags gtk4)

LIBS = $(shell $(PKG-CONFIG) --libs gtk4)

test: $(clean) main.c
	gcc $(CFLAGS) -o test main.c $(LIBS)

check_ui:
	gtk4-builder-tool validate test.ui

clean:
	rm -f test
