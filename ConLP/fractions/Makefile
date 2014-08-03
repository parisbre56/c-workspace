#  Uncomment the following line, for Sun compilation at di.uoa.gr domain.
#CPATH = /usr/sfw/bin/

#  Comment out the following line, for gcc versions less than 4.3.
STANDARD = -std=c++0x

# Naxos Directory
NAXOS = ../naxos



####  COMPILER  ####

CXX      = $(CPATH)g++
WARNINGS = -pedantic -Wall -W -Wshadow
CXXFLAGS = $(WARNINGS) $(STANDARD) -s -O3

LD = $(CXX)
LDFLAGS = -s -O3



####  FILENAMES  ####

PROGRAMS = fractions
PR_OBJECTS = $(PROGRAMS:=.o)

OBJECTS = $(addprefix $(NAXOS)/, problemmanager.o expressions.o var_constraints.o array_constraints.o intvar.o bitset_domain.o)

HEADERS = $(addprefix $(NAXOS)/, naxos.h internal.h stack.h)



####  BUILDING  ####

.PHONY: all
all: $(PROGRAMS)

$(OBJECTS):   %.o:   %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<  -o $@

$(PR_OBJECTS):   %.o:   %.cpp
	$(CXX) $(CXXFLAGS) -I$(NAXOS) -c  $<

$(PROGRAMS):   %:   $(OBJECTS) %.o
	$(LD) $(LDFLAGS) $^  -o $@



####  CLEANING  ####

.PHONY: clean
clean:
	$(RM)  $(PROGRAMS) $(PR_OBJECTS)
