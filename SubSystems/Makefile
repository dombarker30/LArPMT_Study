SHELL = /bin/sh

SUBDIRS := CRT DAQDecoder

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
