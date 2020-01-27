OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')

TARGET=libbtree.a

CFLAGS=-g -O2 -Wall -Wextra 
#CFLAGS=-g -O2 -Wall -Wextra -D_DEBUG
#CFLAGS=-g -O2 -Wall -D_DEBUG -DOLD_TD_ITERATE

all: headers $(TARGET)

debug:
	make all CFLAGS='-g -O2 -Wall -Wextra -D_DEBUG'

headers: td_functions.h

td_functions.h: *.c
	@echo "  [GENFUNCTION]"
	@perl ./gen_td_functions.pl > td_functions.h

$(TARGET): $(OBJECTS)
	@echo "Creating library .. Ok"; ar -rcs $(TARGET) $(OBJECTS)

doc:
	@echo "  [DOC]"
	@doxygen

clean:
	@rm -f $(TARGET) $(OBJECTS) td_put td_dump td_debug \
	  td_ar td_arlist td_unar
	@cd dir_t && make clean
	@cd dir_d && rm -rf html/

crypto: crypto_objects crypto_lib

CRYPTO_OBJECTS:=$(shell ls tdcrypt/*.c | sort | sed -e 's/\.c$$/.o/')

crypto_objects: $(CRYPTO_OBJECTS)

tdcrypt/tdcrypt.o: tdcrypt/tdcrypt.c
	$(CC) $(CFLAGS) -I. -D_TD_CRYPT_ \
	  -c tdcrypt/tdcrypt.c \
	  -o tdcrypt/tdcrypt.o

crypto_lib: all
	ar rcs $(TARGET) $(CRYPTO_OBJECTS)

test:
	@cd dir_t && make

unittest:
	@cd dir_u && make

archive: clean
	@RELEASE=$$(cat release); \
	DIR=`basename $$PWD`; \
	/bin/echo "  [TAR] ~/btree-$$RELEASE.tar.gz"; \
	cd .. && tar czf ~/btree-$$RELEASE.tar.gz $$DIR/

cmdline: all td_put td_dump td_debug td_ar td_arlist td_unar

td_put: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_PUT_ $(CFLAGS) mains.c $(TARGET) -o td_put -lm
	strip td_put

td_dump: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_DUMP_ $(CFLAGS) mains.c $(TARGET) -o td_dump -lm
	strip td_dump

td_debug: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_DEBUG_ $(CFLAGS) mains.c $(TARGET) -o td_debug -lm
	strip td_debug

td_ar: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_AR_ $(CFLAGS) mains.c $(TARGET) -o td_ar -lm
	strip td_ar

td_arlist: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_ARLIST_ $(CFLAGS) mains.c $(TARGET) -o td_arlist -lm
	strip td_arlist

td_unar: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_UNAR_ $(CFLAGS) mains.c $(TARGET) -o td_unar -lm
	strip td_unar

%.o: %.c
	@if $(CC) $(CFLAGS) -c -o $@ $<; then \
	  echo "[CC] $< .. Ok"; \
	else \
	  echo "Failed: $(CC) $(CFLAGS) -c -o $@ $<"; \
	  exit -1; \
	fi
