SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ./Event.h ./ChannelInfo.h
SRCFILES    = KeySight_DAQDecoder.cc
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.so: %.cc SubSysEventDict.cxx 
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ -I$(ROOTSYS)/include $^ -lAgMD2 $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ ./SubSysEventDict* $(OBJFILES) $(EXECUTABLES)

SubSysEventDict.cxx: $(DEPS) Linkdef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^
	@mv ./SubSysEventDict_rdict.pcm $(OBJDIR)/SubSysEventDict_rdict.pcm

