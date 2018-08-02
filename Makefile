CXX=`root-config --cxx`
CXXFLAGS=`root-config --cflags` -ggdb -std=c++11
LDFLAGS=`root-config --ldflags`
LDLIBS=`root-config --glibs` -lAgMD2
SOURCES=waveform.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=waveform

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) -lAgMD2 -o $@ $^ $(LDLIBS)

.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CFLAGS) -W -Wall -c $<

clean:
	rm -f ./*~ ./*.o ./waveform
