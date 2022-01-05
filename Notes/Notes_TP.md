# Travaux Dirige n.1

## MPI - Message Passing Interface

```bash
sudo apt install  build-essential make g++ gdb libopenmpi-dev
mpiexec [option] ./program.exe [ <args> ]
```



```c++
# include <mpi.h>
MPI_Init(&argc,&argv);
MPI_Comm_size(comm, &size);
MPI_Comm_rank(comm,&rank);
MPI_Abort(comm,errocode);
MPI_Wtime();
MPI_Wtick();
MPI_Finalize();
```



```c++
MPI_Send (&buf,count ,datatype ,dest ,tag,comm);
MPI_Recv (&buf,count ,datatype ,source ,tag,comm , &status);
MPI_Ssend( &buf, count , datatype , dest , tag, comm);
MPI_Sendrecv( &sendBuf , sendCount , sendType , dest , sendTag ,
			  &recvBuf , recvCount , recvType , source , recvTag ,
			  comm , &status );
MPI_Probe( source , tag, comm , &status );
MPI_Get_count( &status , datatype , &count );

MPI_Isend( &buf, count , datatype , dest , tag, comm , &request );
MPI_Irecv( &buf, count , datatype , source , tag, comm , &request );
MPI_Issend( &buf, count , datatype , dest , tag, comm , &request );

MPI_Test(&request , &flag , &status );
MPI_Testany( count , &array_of_requests , &index , &flag , &status );
MPI_Testall( count , &array_of_requests , &flag , &array_of_statuses );
MPI_Testsome( incount , &array_of_requests , &outcount , &array_of_offsets ,
			  &array_of_statuses );

MPI_Wait(&request , &status );
MPI_Waitany( count , &array_of_requests , &index , &status );
MPI_Waitall( count , &array_of_requests , &array_of_statuses );
MPI_Waitsome( incount , &array_of_requests , &outcount , &array_of_offsets ,
			  &array_of_statuses );
MPI_Iprobe( source , tag, comm , &flag , &status );
```

## Autre

- `2^n` : `1<<n`

# Travaux Dirige n.2

## MPI - Message Passing Interface

```c++
MPI_Barrier (comm);
MPI_Bcast (&buffer, count, datatype, root, comm);
MPI_Scatter (&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm);
MPI_Gather (&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm );
MPI_Allgather (&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, comm );
/*
MPI_MAX, MPI_SUM, MPI_PROD, MPI_LAND...
*/
MPI_Reduce (&sendbuf, &recvbuf, count, datatype, op, root, comm );
MPI_Allreduce (&sendbuf, &recvbuf, count, datatype, op, comm );
MPI_Reduce_scatter (&sendbuf, &recvbuf, recvcnt, datatype, op, comm );
MPI_Alltoall ( &sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, comm );
MPI_Scan ( &sendbuf, &recvbuf, count, datatype, op, comm );

MPI_Comm_create;
MPI_Comm_free;
MPI_Comm_split(MPI_Comm A, int color, int rank, &MPI_Comm);
```

# Travaux Dirige n.3

## OpenMP

```c++
# pragma omp parallel for schedule(dynamic)
# pragma omp parallel for num_threads(8) reduction(+:scal)
```

## produit matrice–matrice par bloc  

```c++
Matrix operator*(const Matrix& A, const Matrix& B) {
  Matrix C(A.nbRows, B.nbCols, 0.0);

  # pragma omp parallel for schedule(dynamic)
  for (int ib = 0; ib < A.nbRows; ib+=szBlock)
  	for (int jb = 0; jb < B.nbCols; jb+=szBlock)
  		for (int kb = 0; kb < A.nbCols; kb+=szBlock)
  			prodSubBlocks(ib, jb, kb, szBlock, A, B, C);
  return C;
}
```

## Thread

```c++
#include <thread>
#include <future>
#include <utility>
void Test(int x, int y, std::promise<int> &promiseObj){
    promiseObj.set_value(x+y);
}
int main(){
    int x=1,y=2,a=3,b=4;
    std::promise<int> promiseObj1, promiseObj2;
    std::future<int> futureObj1=promiseObj1.get_future(),
    				futureObj2=promiseObj2.get_future();
    std::thread t1(Test, x, y, std::ref(promiseObj1));
    std::thread t2(Test, a, b, std::ref(promiseObj2));
    t1.join();t2.join();
    int sum1 = futureObj1.get();
    int sum2 = futureObj2.get();
    
    return 0;
}
```



# Travaux Dirige n.4

## TBB

# Travaux Dirige n.5

## cuda