SHELL = /bin/sh

SUBDIRS := DAQDecoder Analysis OnlineAnalysis

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
