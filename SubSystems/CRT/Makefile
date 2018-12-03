SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ../DAQDecoder/Event.h ../DAQDecoder/ChannelInfo.h
SRCFILES    = crt_coincidence.cc crt_calibration.cc 
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.o, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.o: %.cc $(DEPS)
	@echo $(SRCDIR)
	@echo Building Objectfile $@ 
	@mkdir -p $(@D)
	@$(CC) $(COPTS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

$(OUTDIR)/%: $(OBJDIR)/%.o
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ $(OBJFILES) $(EXECUTABLES)


