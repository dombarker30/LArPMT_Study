SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ./Event.h ./ChannelInfo.h /home/argonshef/CAENDAQ/PixelDAQ/include/PixelReadout.hh /home/argonshef/CAENDAQ/PixelDAQ/include/ChannelMap.hh
SRCFILES    = PixelTPC_DAQDecoder.cc
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.so: %.cc EventDict.cxx ChannelInfo.c /home/argonshef/CAENDAQ/PixelDAQ/srcs/ChannelMap.cc
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ -I$(ROOTSYS)/include $^ -lCAENDigitizer $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ ./EventDict* $(OBJFILES) $(EXECUTABLES)

EventDict.cxx: $(DEPS) Linkdef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^
	@mv ./EventDict_rdict.pcm $(OBJDIR)/EventDict_rdict.pcm

