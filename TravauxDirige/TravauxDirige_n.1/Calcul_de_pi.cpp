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
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...
	if (nargs != 2){
		std::cout << "format : mpiexec -np i Calcul_de_pi.exe N\n";
		return 1;
	}
	MPI_Init( &nargs, &argv );
	// Pour des raisons de portabilité qui débordent largement du cadre
	// de ce cours, on préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
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
			std::cout << recvMsgs[i] << " ";
		}
		std::cout << std::endl;
		pi = pi * nbSamples / nbSample;
		std::cout << "The final result of PI is " << pi << std::endl;
	}
	else{
		pi = approximate_pi(nbSamples);
		MPI_Send(&pi, 1, MPI_DOUBLE, nbp-1, rank, globComm);
		std::cout << rank << "/" << pi << std::endl;
	}
	// Rajout de code....

	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
