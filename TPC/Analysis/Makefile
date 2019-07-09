SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ../DAQDecoder/Event.h ../DAQDecoder/ChannelInfo.h 
SRCFILES    = PixelTPC_AnalysisExample.cc PixelTPC_DrawWaveforms.cc 
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(OBJDIR)/keyb.o $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/keyb.o: keyb.c 
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(LINUX) keyb.h -c -o $@ $<

$(OBJDIR)/%.so: %.cc ../DAQDecoder/EventDict.cxx ../DAQDecoder/ChannelInfo.c 
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@  -I $(ROOTSYS)/include $^ $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so 
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $(OBJDIR)/keyb.o $< $(LIBS) 

.PHONY: clean

clean:
	@rm -f ./*~ ../DAQDecoder/EventDict* $(OBJFILES) $(EXECUTABLES)

../DAQDecoder/EventDict.cxx: $(DEPS) ../DAQDecoder/Linkdef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^
	@mv ../DAQDecoder/EventDict_rdict.pcm $(OBJDIR)/EventDict_rdict.pcm

