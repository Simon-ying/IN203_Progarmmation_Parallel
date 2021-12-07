# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <mpi.h>
# include <exception>
static int dim = 0;
int main( int nargs, char* argv[] )
{
	// dim = dimension de l'hypercube : 1, 2, 3
	if (nargs == 2){
		dim = atoi(argv[1]);
	}
	// Initialisation
	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);

	int nbp;
	MPI_Comm_size(globComm, &nbp);
	int rank;
	MPI_Comm_rank(globComm, &rank);

	// Création d'un fichier pour ma propre sortie en écriture :
	std::stringstream fileName;
	fileName << "Output" << std::setfill('0') << std::setw(5) << rank << ".txt";
	std::ofstream output( fileName.str().c_str() );
	int jeton;
	const int tag = 18;
	MPI_Status status;

	// pour dimension = 1
	if (dim == 1){
		if (nbp != 2){
			std::cerr << "Attention : le nombre de processus doit etre 2 !" << std::flush << std::endl;
			MPI_Abort(globComm, -1);
		}
		try{
			if (rank == 0){
				jeton = 27182;
				MPI_Send(&jeton, 1, MPI_INT, 1, tag, globComm);
			}
			if (rank == 1){
				MPI_Recv(&jeton, 1, MPI_INT, 0, tag, globComm, &status);
			}
			output << "Apres echange, je possede le jeton avec la valeur : " << jeton << std::flush << std::endl;
	
			output.close();
		}
		catch(std::exception& e){
			std::cerr << e.what() << std::endl;
		}
	}
	// pour dimension == 2
	else if (dim == 2){
		if (nbp != 4){
			std::cerr << "Attention : le nombre de processus doit etre 4 !" << std::flush << std::endl;
			MPI_Abort(globComm, -1);
		}
		try{
			if (rank == 0){
				jeton = 27182;
				MPI_Send(&jeton, 1, MPI_INT, 1, tag, globComm);
				MPI_Send(&jeton, 1, MPI_INT, 2, tag, globComm);
			}
			if (rank == 1){
				MPI_Recv(&jeton, 1, MPI_INT, 0, tag, globComm, &status);
				MPI_Send(&jeton, 1, MPI_INT, 3, tag, globComm);
			}
			if (rank == 2){
				MPI_Recv(&jeton, 1, MPI_INT, 0, tag, globComm, &status);
			}
			if (rank == 3){
				MPI_Recv(&jeton, 1, MPI_INT, 1, tag, globComm, &status);
			}
			output << "Apres echange, je possede le jeton avec la valeur : " << jeton << std::flush << std::endl;
	
			output.close();
		}
		catch(std::exception& e){
			std::cerr << e.what() << std::endl;
		}
	}
	// cas general
	else{
		if (0 == rank) jeton = 27182;

		int ndim = 0;
		int nbnodes = nbp;
		// Calcul dimension du cube :
		while (nbnodes > 1){
			nbnodes /= 2;
			ndim++;
		}
		if (nbp - (1<<ndim) > 0){
			std::cerr << "Attention : le nombre de processus doit etre une puissance de deux !" << std::flush << std::endl;
			MPI_Abort(globComm, -1);
		}
		/***
		dim = 1: 0=>1
		dim = 2: dim = 1 + 0=>2, 1=>3
		dim = 3: dim = 1 + dim = 2 + 0=>4, 1=>5, 2=>6, 3=>7
		dim = n: sum(dim(1...n-1)) + (0...2^(n-1)-1)=>()+2^(n-1)
		...
		***/

		for (int d=0; d<ndim; d++){
			int nb = (1<<d);
			if (rank < nb){
				MPI_Send(&jeton, 1, MPI_INT, rank+nb, tag, globComm);
			}
			else if (rank < 2*nb){
				MPI_Recv(&jeton, 1, MPI_INT, rank-nb, tag, globComm, &status);
			}
		}

		output << "Apres echange, je possede le jeton avec la valeur : " << jeton << std::flush << std::endl;
	
		output.close();


	}

    
	MPI_Finalize();
	return EXIT_SUCCESS;
}
