SHELL = /bin/sh

include ../../makeconfig.mk

DEPS        = ./Event.h ./ChannelInfo.h
SRCFILES    = KeySight_DAQDecoder.cc 
OBJFILES    = $(patsubst %.cc, $(OBJDIR)/%.o, $(SRCFILES))
EXECUTABLES = $(patsubst %.cc, $(OUTDIR)/%, $(SRCFILES))

all: $(EXECUTABLES)

.PRECIOUS: $(OBJFILES)

$(OBJDIR)/%.o: %.cc $(DEPS)
	@echo $(SRCDIR)
	@echo Building Objectfile $@ 
	@mkdir -p $(@D)
	@$(CC) $(COPTS) $(CPPFLAGS) $(CFLAGS) -lAgMD2 -c $< -o $@ 

$(OUTDIR)/%: $(OBJDIR)/%.o
	@echo Compiling Exectuable $@
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) -o $@ $< $(LIBS)

.PHONY: clean

clean:
	@rm -f ./*~ $(OBJFILES) $(EXECUTABLES)


