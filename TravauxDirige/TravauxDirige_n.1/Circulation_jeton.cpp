# include <iostream>
# include <cstdlib>
# include <mpi.h>
# include <random>
int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...

	MPI_Init( &nargs, &argv );
	// Pour des raison préfère toujours cloner le communicateur global
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

	// Le message reçu ou envoyé
	int outmsg;
	int inmsg;
	MPI_Status stat;
	outmsg = (rank + 18) * 2;
	if (rank == 0){
		// outmsg = 1;
		
		// MPI_Ssend( &buf, count , datatype , dest , tag, comm)
		/*
		MPI_Sendrecv(&sendBuf , sendCount , sendType , dest , sendTag ,
					 &recvBuf , recvCount , recvType , source , recvTag ,
					 comm , &status);
		*/
		/*
		MPI_Sendrecv(&outmsg, 1, MPI_INT, rank+1, 18, 
					 &inmsg, 1, MPI_INT, nbp-1, 18,
					 globComm, &stat);
		*/
		MPI_Sendrecv(&outmsg, 1, MPI_INT, rank+1, rank, 
					 &inmsg, 1, MPI_INT, nbp-1, nbp-1,
					 globComm, &stat);
		std::cout << "The Message sent by processor n." << nbp-1 << ", recieved by processor n." << rank << " is: " << inmsg << std::endl;
	}
	else if (rank == nbp - 1){
		// MPI_Recv (&buf,count ,datatype ,source ,tag,comm , &status)
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
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
