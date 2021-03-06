# include <iostream>
# include <cstdlib>
# include <mpi.h>
# include <sstream>
# include <fstream>
# include <string>
# include <iomanip>

int main( int nargs, char* argv[] )
{
    int version = 0;
    std::string flag = "-o";
    if (nargs > 1) {
      if (!flag.compare(argv[1])) version = 1;
      else {
        std::cout << "Format : mpiexec -np [number] ./HelloWorld.exe -o\n";
        return 1;
      }
    }
    MPI_Init(&nargs, &argv);

    int numero_du_processus, nombre_de_processus;

    MPI_Comm_rank(MPI_COMM_WORLD,
                  &numero_du_processus);
    MPI_Comm_size(MPI_COMM_WORLD, 
                  &nombre_de_processus);
    if (version == 1){
      std::stringstream fileName;
      fileName << "Output" << std::setfill('0') << std::setw(5) << numero_du_processus << ".txt";
      std::ofstream output( fileName.str().c_str() );
      output << "Bonjour, je suis la tâche n° " << numero_du_processus << " sur " << nombre_de_processus << " tâches." << std::endl;
      output.close();
    }
    
    else
      // affichier Bonjour, je suis la tâche n° xx sur yy tâches
      std::cout << "Bonjour, je suis la tâche n° " 
                << numero_du_processus << " sur "
                << nombre_de_processus << " tâches" 
                << std::endl;
    MPI_Finalize();
    return EXIT_SUCCESS;
}
