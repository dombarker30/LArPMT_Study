SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ../DAQDecoder/Event.h ../DAQDecoder/ChannelInfo.h
SRCFILES    = crt_coincidence.cc crt_calibration.cc
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.so: %.cc ../DAQDecoder/SubSysEventDict.cxx 
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ -I$(ROOTSYS)/include $^ $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ ../DAQDecoder/SubSysEventDict* $(OBJFILES) $(EXECUTABLES)

../DAQDecoder/SubSysEventDict.cxx: $(DEPS) ../DAQDecoder/Linkdef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^
	@mv ../DAQDecoder/SubSysEventDict_rdict.pcm $(OBJDIR)/SubSysEventDict_rdict.pcm

