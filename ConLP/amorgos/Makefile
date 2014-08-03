#  Uncomment the following line, for Sun compilation at di.uoa.gr domain.
#CPATH = /usr/sfw/bin/

#  Comment out the following line, for gcc versions less than 4.3.
STANDARD = -std=c++0x

# Naxos Directory
NAXOS = ../naxos



####  COMPILER  ####

CXX      = $(CPATH)g++
WARNINGS = -pedantic -Wall -W -Wshadow
CXXFLAGS = $(WARNINGS) $(STANDARD) -O



####  FILENAMES  ####

SOURCES = boundedbt.cpp credit.cpp dbds.cpp dbs.cpp dfs.cpp fns.cpp gns.cpp heuristics.cpp ibroad.cpp isamp.cpp lan.cpp lds.cpp onesamp.cpp rdfs.cpp

HEADERS = heuristics.h onesamp.h dfs.h
OBJECTS = $(SOURCES:.cpp=.o)



####  BUILDING  ####

.PHONY: all
all: $(OBJECTS)

%.o:  %.cpp %.h $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(NAXOS) -c  $<



####  CLEANING  ####

.PHONY: clean
clean:
	$(RM)  $(OBJECTS)
