# include <iostream>
# include <cstdlib>
# include <mpi.h>
# include <random>
int main( int nargs, char* argv[] )
{
	// Initialisation
	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);

	int nbp;
	MPI_Comm_size(globComm, &nbp);
	int rank;
	MPI_Comm_rank(globComm, &rank);

	// Le message reçu ou envoyé
	int outmsg;
	int inmsg;
	MPI_Status stat;
	outmsg = (rank + 18) * 2;
	if (rank == 0){
		MPI_Sendrecv(&outmsg, 1, MPI_INT, rank+1, rank, 
					 &inmsg, 1, MPI_INT, nbp-1, nbp-1,
					 globComm, &stat);
		std::cout << "The Message sent by processor n." << nbp-1 << ", recieved by processor n." << rank << " is: " << inmsg << std::endl;
	}
	else if (rank == nbp - 1){
		// MPI_Recv(&buf,count ,datatype ,source ,tag,comm , &status)
		// MPI_Recv(&inmsg, 1, MPI_INT, rank-1, 18, globComm, &stat);
		MPI_Recv(&inmsg, 1, MPI_INT, rank-1, rank-1, globComm, &stat);
		// outmsg = inmsg;
		// MPI_Send(&outmsg, 1, MPI_INT, 0, 18, globComm);
		MPI_Send(&outmsg, 1, MPI_INT, 0, rank, globComm);
		std::cout << "The Message sent by processor n." << rank-1 << ", recieved by processor n." << rank << " is: " << inmsg << std::endl;
	}
	else{
		// MPI_Recv(&inmsg, 1, MPI_INT, rank-1, 18, globComm, &stat);
		// outmsg = inmsg;
		// MPI_Send(&outmsg, 1, MPI_INT, rank+1, 18, globComm);
		MPI_Recv(&inmsg, 1, MPI_INT, rank-1, rank-1, globComm, &stat);
		MPI_Send(&outmsg, 1, MPI_INT, rank+1, rank, globComm);
		std::cout << "The Message sent by processor n." << rank-1 << ", recieved by processor n." << rank << " is: " << inmsg << std::endl;
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
