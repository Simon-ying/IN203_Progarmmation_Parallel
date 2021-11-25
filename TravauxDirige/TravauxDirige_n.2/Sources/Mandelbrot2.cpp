/**Exercice 3.2
 **/

# include <iostream>
# include <cstdlib>
# include <string>
# include <chrono>
# include <cmath>
# include <vector>
# include <fstream>
# include <mpi.h>


/** Une structure complexe est définie pour la bonne raison que la classe
 * complex proposée par g++ est très lente ! Le calcul est bien plus rapide
 * avec la petite structure donnée ci--dessous
 **/
struct Complex
{
    Complex() : real(0.), imag(0.)
    {}
    Complex(double r, double i) : real(r), imag(i)
    {}
    Complex operator + ( const Complex& z )
    {
        return Complex(real + z.real, imag + z.imag );
    }
    Complex operator * ( const Complex& z )
    {
        return Complex(real*z.real-imag*z.imag, real*z.imag+imag*z.real);
    }
    double sqNorm() { return real*real + imag*imag; }
    double real,imag;
};

std::ostream& operator << ( std::ostream& out, const Complex& c )
{
  out << "(" << c.real << "," << c.imag << ")" << std::endl;
  return out;
}

/** Pour un c complexe donné, calcul le nombre d'itérations de mandelbrot
 * nécessaires pour détecter une éventuelle divergence. Si la suite
 * converge, la fonction retourne la valeur maxIter
 **/
int iterMandelbrot( int maxIter, const Complex& c)
{
    Complex z{0.,0.};
    // On vérifie dans un premier temps si le complexe
    // n'appartient pas à une zone de convergence connue :
    // Appartenance aux disques  C0{(0,0),1/4} et C1{(-1,0),1/4}
    if ( c.real*c.real+c.imag*c.imag < 0.0625 )
        return maxIter;
    if ( (c.real+1)*(c.real+1)+c.imag*c.imag < 0.0625 )
        return maxIter;
    // Appartenance à la cardioïde {(1/4,0),1/2(1-cos(theta))}    
    if ((c.real > -0.75) && (c.real < 0.5) ) {
        Complex ct{c.real-0.25,c.imag};
        double ctnrm2 = sqrt(ct.sqNorm());
        if (ctnrm2 < 0.5*(1-ct.real/ctnrm2)) return maxIter;
    }
    int niter = 0;
    while ((z.sqNorm() < 4.) && (niter < maxIter))
    {
        z = z*z + c;
        ++niter;
    }
    return niter;
}

/**
 * On parcourt chaque pixel de l'espace image et on fait correspondre par
 * translation et homothétie une valeur complexe c qui servira pour
 * itérer sur la suite de Mandelbrot. Le nombre d'itérations renvoyé
 * servira pour construire l'image finale.
 
 Sortie : un vecteur de taille W*H avec pour chaque case un nombre d'étape de convergence de 0 à maxIter
 MODIFICATION DE LA FONCTION :
 j'ai supprimé le paramètre W étant donné que maintenant, cette fonction ne prendra plus que des lignes de taille W en argument.
 **/
void 
computeMandelbrotSetRow( int W, int H, int maxIter, int num_ligne, int* pixels)
{
    // Calcul le facteur d'échelle pour rester dans le disque de rayon 2
    // centré en (0,0)
    double scaleX = 3./(W-1);
    double scaleY = 2.25/(H-1.);
    //
    // On parcourt les pixels de l'espace image :
    for ( int j = 0; j < W; ++j ) {
       Complex c{-2.+j*scaleX,-1.125+ num_ligne*scaleY};
       pixels[j] = iterMandelbrot( maxIter, c );
    }
}

std::vector<int>
computeMandelbrotSet( int W, int H, int maxIter, int debut, int ligne)
{// debut est le numéro de ligne pour commencer, est ligne est le nombre de lignes à calculer
    
    std::vector<int> pixels(W*ligne);
    // On parcourt les pixels de l'espace image :
    for ( int i = 0; i < ligne; ++i ) {
      computeMandelbrotSetRow(W, H, maxIter, debut+i, pixels.data() + W*(ligne-i-1) );
    }
    return pixels;
}

/** Construit et sauvegarde l'image finale **/
void savePicture( const std::string& filename, int W, int H, const std::vector<int>& nbIters, int maxIter )
{
    double scaleCol = 1./maxIter;//16777216
    std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
    ofs << "P6\n"
        << W << " " << H << "\n255\n";
    for ( int i = 0; i < W * H; ++i ) {
        double iter = scaleCol*nbIters[i];
        unsigned char r = (unsigned char)(256 - (unsigned (iter*256.) & 0xFF));
        unsigned char b = (unsigned char)(256 - (unsigned (iter*65536) & 0xFF));
        unsigned char g = (unsigned char)(256 - (unsigned( iter*16777216) & 0xFF));
        ofs << r << g << b;
    }
    ofs.close();
}

int main(int argc, char *argv[] ) 
 { 
    const int W = 800;
    const int H = 600;
    // Normalement, pour un bon rendu, il faudrait le nombre d'itérations
    // ci--dessous :
    //const int maxIter = 16777216;
    const int maxIter = 8*65536;
    MPI_Init(&argc, &argv);
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    int nbp;
    MPI_Comm_size(globComm, &nbp);
    int rank;
    MPI_Comm_rank(globComm, &rank);
    if(nbp == 1){
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        std::vector<int> iters = computeMandelbrotSet(W, H, maxIter, 0, H);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "Temps calcul ensemble mandelbrot : " << elapsed_seconds.count() 
                << std::endl;
        savePicture("mandelbrot.tga", W, H, iters, maxIter);
    }
    else if(rank == 0){
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        int ligne[2] = {0, H/(nbp-1)}; // le premier élément est le début de lignes à calculer (à déterminer après), le deuxième élément est le nombre de lignes à calculer
        for (int i=1; i<nbp-1; ++i){
            ligne[0] = H-i*(H/(nbp-1));
            MPI_Send(ligne, 2, MPI_INT, i, i, globComm);
        }
        if ((int)(H/(nbp-1))*(nbp-1)!=H){
            ligne[0] = 0;
            ligne[1] = H - (int)(H/(nbp-1))*(nbp-2);
            MPI_Send(ligne, 2, MPI_INT, nbp-1, nbp-1, globComm);
        }
        else{
            ligne[0] = 0;
            MPI_Send(ligne, 2, MPI_INT, nbp-1, nbp-1, globComm);
        }
        std::vector<int> iters(W*H);
        MPI_Status status;
        for(int i=1; i<nbp-1; ++i){
            MPI_Recv(&(iters[W*(i-1)*(int)(H/(nbp-1))]), W*(int)(H/(nbp-1)), MPI_INT, i, nbp+i, globComm, &status);
        }
        if ((int)(H/(nbp-1))*(nbp-1)!=H){
            MPI_Recv(&(iters[W*(nbp-2)*(int)(H/(nbp-1))]), W*(H-(int)(H/(nbp-1))*(nbp-2)), MPI_INT, nbp-1, 2*nbp-1, globComm, &status);
        }
        else{
            MPI_Recv(&(iters[W*(nbp-2)*(int)(H/(nbp-1))]), W*(int)(H/(nbp-1)), MPI_INT, nbp-1, 2*nbp-1, globComm, &status);
        }
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "Temps calcul ensemble mandelbrot : " << elapsed_seconds.count() 
                << std::endl;
        savePicture("mandelbrot.tga", W, H, iters, maxIter);
    }
    else{
        int ligne[2];
        MPI_Status status;
        MPI_Recv(ligne, 2, MPI_INT, 0, rank, globComm, &status);
        auto iters_p = computeMandelbrotSet(W, H, maxIter, ligne[0], ligne[1]);
        MPI_Send(&(iters_p)[0], ligne[1]*W, MPI_INT, 0, nbp+rank, globComm);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
 }
    
