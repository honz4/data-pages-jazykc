
#TODO: DESTDIR
#TODO: antimake.mk, cmake, autotools?

ifeq (0,1)
require GNU Make!!!
endif

#todo/FIXME libtap testy!
test_list_SRC = up/list.c

test_list : FORCE
	$(CC) -I. -o $@ $@.c $($@_SRC) && ./$@

test_value \
test_math : FORCE
	$(CC) -I. -o $@ $@.c -L . -lup && ./$@

tags : FORCE ; ctags -Rv .

prefix = $(HOME)/jazykc
libdir = $(prefix)/lib
includedir = $(prefix)/include


AR = ar

LIBRARY = libup.a


CPPFLAGS = -I.

OBJEXT ?= .o

all : $(LIBRARY)

libup_SOURCES  = up/list.c
libup_HEADERS  = up/portable.h up/list.h

libup_SOURCES += up/math.c
libup_HEADERS += up/math.h

libup_SOURCES += up/value.c
libup_HEADERS += up/value.h

libup_OBJECTS  = $(libup_SOURCES:.c=$(OBJEXT))


$(LIBRARY) : $(libup_OBJECTS)
	$(AR) $(ARFLAGS) $(LIBRARY) $(libup_OBJECTS)


plantuml = ~honza/bin/plantuml
uml : ; $(plantuml) *.txt


INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644

install : $(LIBRARY)
	$(INSTALL_DATA) $(LIBRARY) $(libdir)
	$(INSTALL) -d -m 755 $(includedir)/up
	$(INSTALL_DATA) $(libup_HEADERS) $(includedir)/up

clean : FORCE
	rm -rf -- $(LIBRARY) *.o up/*.o core

##vypise obsah promenne
var-% : ; @echo '$*=|$($*)|'

FORCE :: ;
