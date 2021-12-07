# TD de IN203
## Introduction
<font color=blue>Prénom: </font> Zhuoye 
<font color=blue>Nom: </font> YING 
<font color=red>Groupe: </font> 2  

## TD 1

- Resultat de Calcule_de_pi.exe : la difference entre mpiexec et mpirun

  ![](TravauxDirige/TravauxDirige_n.1/Calcul_de_pi.png)

- `2^n` : `1<<n`
- 

## TD 2

- Les question 1 et 2 ?
- Temps total ? 
- realloc pour vector : `std::vector<int>(W*H).swap(pixels);`
- MPI_Recv(..., MPI_Status& status) : `status.MPI_TAG`, `status.MPI_SOURCE`
- `default`, `delete`

## TD 3

- `# pragma omp parallel for num_threads(8) reduction(+:scal) `

- num_threads, omp_set_num_threads() > OMP_NUM_THREADS

- ```c++
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

  

- Expliquez pourquoi la permutation des boucles optimales que vous avez trouvée est bien la façon optimale d’ordonner les boucles en vous servant toujours du support de cours.  memery bound et cpu bound

## TD 4
