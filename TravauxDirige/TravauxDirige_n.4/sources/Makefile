#include Make_mac.inc
#include Make_other.inc
include Make_linux.inc
LIBS = -lm -ltbb
CXXFLAGS = -std=c++17 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -Weffc++ -D_GLIBCXX_DEBUG
CXXFLAGS2 = CXXFLAGS
else
CXXFLAGS += -O3 -march=native
CXXFLAGS2 = ${CXXFLAGS} -O2 -march=native -Wall
endif

CXXFLAGS3 = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS3 += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS3 += -O3 -march=native -Wall
endif

CXXFLAGS4 = -std=c++14
ifdef DEBUG
CXXFLAGS4 += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS4 += -O3 -march=native -Wall
endif

ALL = raytracer.exe Mandelbrot_tbb.exe Mandelbrot_omp.exe Mandelbrot_mpi.exe Mandelbrot_sm.exe Mandelbrot_seq.exe

default: help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~ *.ppm *.tga

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# dependances particulieres :
raytracer.exe: raytracer.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)	
Mandelbrot_mpi.exe: Mandelbrot_mpi.cpp
	$(CXX2) $(CXXFLAGS3) $^ -o $@ $(LIB)	
Mandelbrot_sm.exe: Mandelbrot_sm.cpp
	$(CXX2) $(CXXFLAGS3) $^ -o $@ $(LIB)	
Mandelbrot_omp.exe: Mandelbrot_omp.cpp
	$(CXX3) $(CXXFLAGS4) $^ -o $@ $(LIB2)	


help:
	@echo "Available targets:"
	@echo "    all: compile all executables"
	@echo "       $(ALL)"
	@echo "Configuration:"
	@echo "    CXX     : $(CXX)"
	@echo "    CXXFLAGS: $(CXXFLAGS)"
	@echo "Add DEBUG=yes to compile in debug"


# Rapport
%.html: %.md
	pandoc -s --toc $< --css=./github-pandoc.css -o $@
