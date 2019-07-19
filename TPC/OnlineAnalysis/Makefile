SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = OnlineDataBaseHist.hh OnlineMonitor.hh  ../DAQDecoder/Event.h ../DAQDecoder/ChannelInfo.h /home/argonshef/CAENDAQ/PixelDAQ/include/ChannelMap.hh  
SRCFILES    = GetOnlineParams.cc PixelTPC_RunOnlineMonitoring.cc BuildDataBaseTables.cc
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.so, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.so: %.cc $(DEPS) ../OnlineAnalysis/OnlineDataBaseHist.cc OnlineMonitor.cc ../DAQDecoder/EventDict.cxx ../DAQDecoder/ChannelInfo.c /home/argonshef/CAENDAQ/PixelDAQ/srcs/ChannelMap.cc
	@echo Building Objectfile $@
	@mkdir -p $(@D)
	@$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ -I$(ROOTSYS)/include $(SQL) $^ $(LIBS)

$(OUTDIR)/%: $(OBJDIR)/%.so 
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -Wall -o $@ $< $(LIBS) $(SQL)

.PHONY: clean

clean:
	@rm -f ./*~ $(OBJFILES) $(EXECUTABLES)

# ../OnlineAnalysis/OnlineDict.cxx: $(DEPS) ../OnlineAnalysis/Linkdef.h 
# 	@echo "Generating dictionary ..."
# 	@rootcint -f $@ -c -p $^
# 	@mv ../OnlineAnalysis/OnlineDict_rdict.pcm $(OBJDIR)/OnlineDict_rdict.pcm

