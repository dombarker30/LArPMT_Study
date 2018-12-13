SHELL = /bin/sh                                                                          

include ./makeconfig.mk

SUBDIRS := SubSystems TPC       

all: $(SUBDIRS)                                      

$(SUBDIRS):                
	$(MAKE) -C $@                 

.PHONY: all $(SUBDIRS)   

clean: 
	@rm -rf $(OBJDIR)/*
	@rm -rf $(OUTDIR)/* 

