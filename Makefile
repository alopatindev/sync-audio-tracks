SUFFIXES += .d
CXX = g++

CFLAGS = -Werror -Wall -Wextra -Wfatal-errors -pedantic-errors -pedantic -march=native -O3 -I. -std=c++14

CFLAGS += $(shell pkg-config --cflags sndfile)
LFLAGS =  $(shell pkg-config --libs sndfile)

CFLAGS += $(shell pkg-config --cflags fftw3)
LFLAGS += $(shell pkg-config --libs fftw3)

TARGETS = compute_sound_offset

all: $(TARGETS)

compute_sound_offset: sound.o

$(TARGETS): %: %.o
	$(CXX) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CXX) -M $(CFLAGS) $< > $*.d
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGETS) *.d

-include $(wildcard *.d)
