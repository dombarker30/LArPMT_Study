SHELL = /bin/bash

SRCDIR      = $(shell echo $$LArAnASRCDir)
BASEDIR     = $(SRCDIR)..
CC          = g++
CXX         = `root-config --cxx`
CPPFLAGS    = `root-config --cflags --glibs`
LDFLAGS     = `root-config --ldflags`
LIBS        = `root-config --glibs`
COPTS       = -std=c++11
OBJDIR      = $(shell echo $$LArANAOBJDir)
OUTDIR      = $(shell echo $$LArANAEXEDir)

