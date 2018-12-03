SHELL = /bin/sh                                                                                                                                                                     

include ./makeconfig.mk

SUBDIRS := SubSystems       

all: $(SUBDIRS)                                                                                                                                              

$(SUBDIRS):                                                                                                                                           
	$(MAKE) -C $@                                                                                                                     

.PHONY: all $(SUBDIRS)   

clean: 
	@rm -rf OBJDIR/*
	@rm -rf OUTDIR/* 

# CXX=`root-config --cxx`
# CC=g++
# CXXFLAGS=`root-config --cflags` -ggdb -std=c++11
# CPPFLAGS=`root-config --cflags --glibs`
# LDFLAGS=`root-config --ldflags`
# ROOT=`root-config`
# LDLIBS=`root-config --glibs` -lAgMD2
# LIBS=`root-config --glibs`
# EXECUTABLES=waveform calibration coincidence
# DEPS=Event.h ChannelInfo.h

# all: $(EXECUTABLES)

# waveform.o: waveform.cc $(DEPS)
# 	$(CXX) $(CXXFLAGS) $(CFLAGS) -W -Wall -c $<
# waveform: waveform.o 
# 	$(CXX) $(LDFLAGS) -lAgMD2 -o $@ $^ $(LDLIBS)

# calibration.o: calibration.cc $(DEPS)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -W -Wall -c $<
# calibration: calibration.o
# 	$(CXX) $(LDFLAGS)  -o $@ $^ $(LIBS)

# coincidence.o: coincidence.cc $(DEPS)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -W -Wall -c $<
# coincidence: coincidence.o
# 	$(CXX) $(LDFLAGS)  -o $@ $^ $(LIBS)

# clean:
# 	rm -f ./*~ ./*.o ./waveform
# 	rm -f ./*~ ./*.o ./calibration
# 	rm -f ./*~ ./*.o ./coincidence

# CXX=`root-config --cxx`
# CXXFLAGS=`root-config --cflags` -ggdb -std=c++11
# LDFLAGS=`root-config --ldflags`
# LDLIBS=`root-config --glibs` -lAgMD2
# SOURCES=waveform.cc
# OBJECTS=$(SOURCES:.cc=.o)
# EXECUTABLE=waveform

# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CXX) $(LDFLAGS) -lAgMD2 -o $@ $^ $(LDLIBS)

# .cc.o:
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CFLAGS) -W -Wall -c $<

# clean:
# 	rm -f ./*~ ./*.o ./waveform
