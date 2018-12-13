SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ../DAQDecoder/Event.h ../DAQDecoder/ChannelInfo.h
SRCFILES    = PixelTPC_AnalysisExample.cc
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.so: %.cc ../DAQDecoder/EventDict.cxx ../DAQDecoder/ChannelInfo.c
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ -I$(ROOTSYS)/include $^ $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ ../DAQDecoder/EventDict* $(OBJFILES) $(EXECUTABLES)

../DAQDecoder/EventDict.cxx: $(DEPS) ../DAQDecoder/Linkdef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^
	@mv ../DAQDecoder/EventDict_rdict.pcm $(OBJDIR)/EventDict_rdict.pcm

