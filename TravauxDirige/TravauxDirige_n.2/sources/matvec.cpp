// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>
# include <mpi.h>
# include <utility>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
public:
    Matrix (int dim);
    Matrix( int nrows, int ncols );
    Matrix( const Matrix& A ) = delete;
    Matrix( Matrix&& A ) = default;
    ~Matrix() = default;

    Matrix& operator = ( const Matrix& A ) = delete;
    Matrix& operator = ( Matrix&& A ) = default;
    
    double& operator () ( int i, int j ) {
        return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () ( int i, int j ) const {
        return m_arr_coefs[i + j*m_nrows];
    }
    
    std::vector<double> operator * ( const std::vector<double>& u ) const;
    
    std::ostream& print( std::ostream& out ) const
    {
        const Matrix& A = *this;
        out << "[\n";
        for ( int i = 0; i < m_nrows; ++i ) {
            out << " [ ";
            for ( int j = 0; j < m_ncols; ++j ) {
                out << A(i,j) << " ";
            }
            out << " ]\n";
        }
        out << "]";
        return out;
    }
    std::pair<int,int> Size(){
        return std::make_pair(m_nrows, m_ncols);
    }

private:
    int m_nrows, m_ncols;
    std::vector<double> m_arr_coefs;
};
// ---------------------------------------------------------------------
inline std::ostream& 
operator << ( std::ostream& out, const Matrix& A )
{
    return A.print(out);
}
// ---------------------------------------------------------------------
inline std::ostream&
operator << ( std::ostream& out, const std::vector<double>& u )
{
    out << "[ ";
    for ( const auto& x : u )
        out << x << " ";
    out << " ]";
    return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * ( const std::vector<double>& u ) const
{
    const Matrix& A = *this;
    assert( u.size() == unsigned(m_ncols) );
    std::vector<double> v(m_nrows, 0.);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v[i] += A(i,j)*u[j];
        }            
    }
    return v;
}

// =====================================================================
Matrix::Matrix (int dim) : m_nrows(dim), m_ncols(dim),
                           m_arr_coefs(dim*dim)
{
    for ( int i = 0; i < dim; ++ i ) {
        for ( int j = 0; j < dim; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
    }
}
// ---------------------------------------------------------------------
Matrix::Matrix( int nrows, int ncols ) : m_nrows(nrows), m_ncols(ncols),
                                         m_arr_coefs(nrows*ncols)
{
    int dim = (nrows > ncols ? nrows : ncols );
    for ( int i = 0; i < nrows; ++ i ) {
        for ( int j = 0; j < ncols; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
    }    
}
// =====================================================================
Matrix getCols(Matrix& A, int begin, int cols){

    Matrix res(A.Size().first, cols);
    for (int i=0; i < A.Size().first; i++){
        for (int j=0; j<cols; j++){
            res(i,j) = A(i, j+begin);
        }
    }
    return res;
}
// =====================================================================
Matrix getRows(Matrix& A, int begin, int rows){

    Matrix res(rows, A.Size().second);
    for (int i=0; i < rows; i++){
        for (int j=0; j<A.Size().second; j++){
            res(i,j) = A(i+begin, j);
        }
    }
    return res;
}
// =====================================================================
int main( int nargs, char* argv[] )
{
    MPI_Init(&nargs, &argv);
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    int nbp, rank;
    MPI_Comm_size(globComm, &nbp);
    MPI_Comm_rank(globComm, &rank);

    const int N = 120;
    Matrix A(N);
    //std::cout  << "A : " << A << std::endl;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    //std::cout << " u : " << u << std::endl;

    int interval = N / nbp;
    int lastSize = N - (nbp-1)*interval;
    int size;

    std::vector<double> outMsg(N);
    std::vector<double> outMsg_l;
    std::vector<double> v(1);
    std::vector<double> vl(1);
    if (rank == 0){
        std::vector<double>(N).swap(v);
        std::vector<double>(N).swap(vl);
    }
    if (rank == nbp-1){
        size = lastSize;
    }
    else{
        size = interval;
    }

    outMsg =  getCols(A, interval*rank, size) * std::vector<double>(u.begin()+interval*rank, u.begin()+interval*rank+size);
    outMsg_l =  getRows(A, interval*rank, size) * u;



    MPI_Reduce(outMsg.data(), v.data(), N, MPI_DOUBLE, MPI_SUM,0 , globComm);
    MPI_Gather(outMsg_l.data(), size, MPI_DOUBLE, vl.data(), size, MPI_DOUBLE, 0, globComm);
    // std::vector<double> v = A*u;
    if (rank == 0){
        std::vector<double> v_test = A*u;
        std::cout << "A.u_test = " << v_test << " , size : " << v_test.size() << std::endl;
        std::cout << "A.u = " << v << " , size : " << v.size() << std::endl;
        std::cout << "A.u_l = " << vl << " , size : " << vl.size() << std::endl;

    }
    

    MPI_Finalize();
    return EXIT_SUCCESS;
}
