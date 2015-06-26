OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')

TARGET=libbtree.a

CFLAGS=-g -O2 -Wall -Wextra -D_DEBUG
#CFLAGS=-g -O2 -Wall -D_DEBUG -DOLD_TD_ITERATE

all: headers $(TARGET)

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
	@rm -f $(TARGET) $(OBJECTS) td_put td_dump td_debug
	@cd t && make clean
	@cd d && rm -rf html/

test:
	@cd t && make

archive: clean
	@RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/btree-$$RELEASE.tar.gz"; \
	cd .. && tar czf ~/btree-$$RELEASE.tar.gz btree/

cmdline: all td_put td_dump td_debug

td_put: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_PUT_ mains.c $(TARGET) -o td_put -lm

td_dump: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_DUMP_ mains.c $(TARGET) -o td_dump -lm

td_debug: $(TARGET)
	$(CC) -D_TD_FD_MAINS_ -D_TD_FD_DEBUG_ mains.c $(TARGET) -o td_debug -lm

%.o: %.c
	@/bin/echo -n "  [CC] $< .. "; \
	if $(CC) $(CFLAGS) -c -o $@ $<; then \
	  echo "Ok"; \
	else \
	  echo "Failed: $(CC) $(CFLAGS) -c -o $@ $<"; \
	  exit -1; \
	fi
