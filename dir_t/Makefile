OBJECTS=\
  debug.o \

MAINS=\
  test1.o \
  test2.o \
  test3.o \
  test4.o \
  test5.o \
  test6.o \
  test7.o \
  test8.o \
  test9.o \
  test11.o \
  test12.o \
  test14.o \
  test15.o \
  test16.o \
  test17.o \
  test18.o \
  test19.o \
  test20.o \

TARGETS=\
  test1 \
  test2 \
  test3 \
  test4 \
  test5 \
  test6 \
  test7 \
  test8 \
  test9 \
  test11 \
  test12 \
  test14 \
  test15 \
  test16 \
  test17 \
  test18 \
  test19 \
  test20 \
  hell \
  hell2


CFLAGS=-I.. -g -O2 -Wall

all: $(OBJECTS) $(MAINS) $(TARGETS)
	@rm -f log; for TARGET in $(TARGETS); do \
	  /bin/echo -n "Running $$TARGET .. "; \
	  echo "Running $$TARGET" >>log; \
	  if time ./$$TARGET >>log 2>&1; then \
	    echo "Ok"; \
	  else \
	    echo "Failed"; \
	  fi; \
	done

clean:
	@rm -f log $(OBJECTS) $(MAINS) $(TARGETS) hell.c hell2.c
	@rm -rf hell.dSYM

test1: test1.o
	@$(CC) $(CFLAGS) -o test1 test1.o $(OBJECTS) ../*.o -lm

test2: test2.o
	@$(CC) $(CFLAGS) -o test2 test2.o $(OBJECTS) ../*.o -lm

test3: test3.o
	@$(CC) $(CFLAGS) -o test3 test3.o $(OBJECTS) ../*.o -lm

test4: test4.o
	@$(CC) $(CFLAGS) -o test4 test4.o $(OBJECTS) ../*.o -lm

test5: test5.o
	@$(CC) $(CFLAGS) -o test5 test5.o $(OBJECTS) ../*.o -lm

test6: test6.o
	@$(CC) $(CFLAGS) -o test6 test6.o $(OBJECTS) ../*.o -lm

test7: test7.o
	@$(CC) $(CFLAGS) -o test7 test7.o $(OBJECTS) ../*.o -lm

test8: test8.o
	@$(CC) $(CFLAGS) -o test8 test8.o $(OBJECTS) ../*.o -lm

test9: test9.o
	@$(CC) $(CFLAGS) -o test9 test9.o $(OBJECTS) ../*.o -lm

test10: test10.o
	@$(CC) $(CFLAGS) -o test10 test10.o $(OBJECTS) ../*.o -lm

test11: test11.o
	@$(CC) $(CFLAGS) -o test11 test11.o $(OBJECTS) ../*.o -lm

test12: test12.o
	@$(CC) $(CFLAGS) -o test12 test12.o $(OBJECTS) ../*.o -lm

test13: test13.o
	@$(CC) $(CFLAGS) -o test13 test13.o $(OBJECTS) ../*.o -lm

test14: test14.o
	@$(CC) $(CFLAGS) -o test14 test14.o $(OBJECTS) ../*.o -lm

test15: test15.o
	@$(CC) $(CFLAGS) -o test15 test15.o $(OBJECTS) ../*.o -lm

test16: test16.o
	@$(CC) $(CFLAGS) -o test16 test16.o $(OBJECTS) ../*.o -lm

test17: test17.o
	@$(CC) $(CFLAGS) -o test17 test17.o $(OBJECTS) ../*.o -lm

test18: test18.o
	@$(CC) $(CFLAGS) -o test18 test18.o $(OBJECTS) ../*.o -lm

test19: test19.o
	@$(CC) $(CFLAGS) -o test19 test19.o $(OBJECTS) ../*.o -lm

test20: test20.o
	@$(CC) $(CFLAGS) -o test20 test20.o $(OBJECTS) ../*.o -lm

hell:
	@echo "Generating hell.c .."
	@/usr/bin/perl hell.pl > hell.c
	@echo "Compiling hell.c .."
	@$(CC) $(CFLAGS) -o hell hell.c $(OBJECTS) ../libbtree.a -lm

hell2:
	@echo "Generating hell2.c .."
	@/usr/bin/perl hell2.pl > hell2.c
	@echo "Compiling hell2.c .."
	@$(CC) $(CFLAGS) -o hell2 hell2.c $(OBJECTS) ../libbtree.a -lm
