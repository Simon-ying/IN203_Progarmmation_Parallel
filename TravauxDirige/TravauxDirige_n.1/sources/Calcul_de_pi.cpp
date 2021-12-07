# include <chrono>
# include <random>
# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>
#include <stdlib.h>

// Attention , ne marche qu'en C++ 11 ou supérieur :
double approximate_pi( unsigned long nbSamples ) 
{
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = beginning.time_since_epoch();
    unsigned seed = d.count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution <double> distribution ( -1.0 ,1.0);
    unsigned long nbDarts = 0;
    // Throw nbSamples darts in the unit square [-1 :1] x [-1 :1]
    for ( unsigned sample = 0 ; sample < nbSamples ; ++ sample ) {
        double x = distribution(generator);
        double y = distribution(generator);
        // Test if the dart is in the unit disk
        if ( x*x+y*y<=1 ) nbDarts ++;
    }
    // Number of nbDarts throwed in the unit disk
    double ratio = double(nbDarts)/double(nbSamples);
    return 4*ratio;
}

int main( int nargs, char* argv[] )
{
	if (nargs != 2){
		std::cout << "format : mpiexec -np i Calcul_de_pi.exe N\n";
		return 1;
	}

	// Initialisation
	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);

	int nbp;
	MPI_Comm_size(globComm, &nbp);
	int rank;
	MPI_Comm_rank(globComm, &rank);

	int nbSample = atoi(argv[1]);
	int nbSamples = nbSample/(nbp-1);
	double pi;
	MPI_Status stat;
	double recvMsgs[nbp-2];
	if (rank == nbp-1){
		// MPI_Recv (&buf,count ,datatype ,source ,tag,comm , &status)
		for (int i=0; i<nbp-1; i++){
			MPI_Recv(recvMsgs+i, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, globComm, &stat);
		}
		for (int i=0; i<nbp-1; i++){
			pi += recvMsgs[i];
		}
		pi = pi * nbSamples / nbSample;
		std::cout << "The final result of PI is " << pi << std::endl;
	}
	else{
		pi = approximate_pi(nbSamples);
		MPI_Send(&pi, 1, MPI_DOUBLE, nbp-1, rank, globComm);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
