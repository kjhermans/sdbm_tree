all:
	@TESTS=`find . -type f | grep '\.ut' | sort -n` ; \
	  for TEST in $$TESTS; do \
	    ./unittest.pl $$TEST; \
	  done
