SHELL = /bin/bash

SRCDIR      = $(shell echo $$LArAnASRCDir)
BASEDIR     = $(SRCDIR)..
CC          = `root-config --cc`
CXX         = `root-config --cxx`
CXXFLAGS    = `root-config --cflags`
LDFLAGS     = `root-config --ldflags`
SOFLAGS     = -fPIC -shared
LIBS        = `root-config --glibs`
SQL         = -lpqxx -lpq
COPTS       = -std=c++11
LINUX       = -fPIC -DLINUX -O2
OBJDIR      = $(shell echo $$LArANAOBJDir)
OUTDIR      = $(shell echo $$LArANAEXEDir)
