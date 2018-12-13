SHELL = /bin/sh

SUBDIRS := DAQDecoder Analysis

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
