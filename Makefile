SUBDIRS := $(wildcard */.)

.PHONY: all clean

all:
	@for dir in $(SUBDIRS); do \
		echo \\nMaking executable in $$dir; \
		$(MAKE) --no-print-directory -C $$dir -f Makefile; \
  done

clean:
	@for dir in $(SUBDIRS); do \
		echo \\nCleaning files in $$dir; \
		$(MAKE) --no-print-directory -C $$dir -f Makefile $@; \
  done
