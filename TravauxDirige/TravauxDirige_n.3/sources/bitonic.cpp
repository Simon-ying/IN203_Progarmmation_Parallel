# include <algorithm>
# include <utility>
# include <vector>
# include <iostream>
# include <functional>
# include <random>
# include <cassert>
# include <exception>
# include <chrono>
# include <stdexcept>
# include "Vecteur.hpp"
# include <thread>
# include <future>
using namespace Algebra;

// Tri Parallèle Bitonic
namespace Bitonic
{
  template<typename Obj>
  void _compare( bool up, Obj* objs, int len )
  {
    int dist = len/2;
    for ( int i = 0; i < dist; ++i ) {
      if ( (objs[i] > objs[i+dist]) == up ) {
	std::swap(objs[i],objs[i+dist]);
      }
    }
  }
  // --------------------------------------------
  template<typename Obj> std::pair<Obj*,int>
  _merge( bool up, Obj* objs, int len )
  {
    if (len == 1) return std::make_pair(objs,len);
    _compare(up,objs,len);
    auto first  = _merge(up, objs, len/2);
    auto second = _merge(up, objs+len/2, len-(len/2));
    return std::make_pair(first.first, first.second + second.second);
  }
  // --------------------------------------------
  template<typename Obj> void
  _sort( bool up, Obj* objs, int len, std::promise<std::pair<Obj*,int>> &promiseObj )
  {
    if (len <= 1){
      promiseObj.set_value(std::make_pair(objs,1));
      return;
    }
    std::promise<std::pair<Obj*,int>> promiseObjf;
    std::future<std::pair<Obj*,int>> futureObjf = promiseObjf.get_future();
    std::promise<std::pair<Obj*,int>> promiseObjs;
    std::future<std::pair<Obj*,int>> futureObjs = promiseObjs.get_future();

    _sort(true, objs, len/2, promiseObjf);
    _sort(false, objs + len/2, len - (len/2), promiseObjs);
    auto first  = futureObjf.get();
    auto second = futureObjs.get();
    promiseObj.set_value(_merge(up, first.first, first.second + second.second));
    return;
  }

  template<typename Obj> std::pair<Obj*,int>
  _sort_thread( bool up, Obj* objs, int len)
  {
    if (len <= 1){
      return std::make_pair(objs,1);
    }

    std::promise<std::pair<Obj*,int>> promiseObjf;
    std::future<std::pair<Obj*,int>> futureObjf = promiseObjf.get_future();
    std::promise<std::pair<Obj*,int>> promiseObjs;
    std::future<std::pair<Obj*,int>> futureObjs = promiseObjs.get_future();

    std::thread t1(_sort<Obj>, true, objs, len/2, std::ref(promiseObjf));
    std::thread t2(_sort<Obj>, false, objs + len/2, len - (len/2), std::ref(promiseObjs));
    t1.join();
    t2.join();
    auto first  = futureObjf.get();
    auto second = futureObjs.get();
    return _merge(up, first.first, first.second + second.second );
  }
  // --------------------------------------------
  template<typename Obj> std::vector<Obj>&
  sort( bool up, std::vector<Obj>& x )
  {
    _sort_thread(up, x.data(), int(x.size()));
    return x;
  }  
}

int main()
{     
    std::chrono::time_point<std::chrono::system_clock> start, end;
    const size_t N = (1UL << 21);
    const size_t dim = 40;
    std::random_device rd;
    std::mt19937 generator1(rd());
    std::mt19937 generator2(rd());
    
    std::uniform_int_distribution<int> intDistrib(-163845,163845);
    auto genInt = std::bind( intDistrib, generator1 );
    std::uniform_real_distribution<double> coordDistrib(-10.,10.);
    auto genDouble = std::bind( coordDistrib, generator2 );

    // Trie sur les entiers :
    std::vector<double> tab(N);
    for ( auto& x : tab ) x = genInt();
    start = std::chrono::system_clock::now();
    Bitonic::sort(true, tab);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Temps calcul tri sur les entiers : " << elapsed_seconds.count() 
              << std::endl;
    for ( size_t i = 1; i < tab.size(); ++i ) {
        if ( tab[i] < tab[i-1] ) {
            throw std::logic_error("Erreur de tri pour les entiers !");
        }
    }
  
    // Trie sur les vecteurs :
    std::vector<Vecteur> vtab(N);
    for ( auto& x : vtab ) {
        x = Vecteur(dim);
        x[0] = genDouble();
        x[1] = genDouble();
        x[2] = genDouble();
        x[3] = genDouble();
    }
    start = std::chrono::system_clock::now();
    Bitonic::sort(true, vtab);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "Temps calcul tri sur les vecteurs : " << elapsed_seconds.count() 
              << std::endl;
    for ( size_t i = 1; i < vtab.size(); ++i ) {
        if ( vtab[i] < vtab[i-1] ) {
            throw std::logic_error("Erreur de tri pour les vecteurs !");
        }
    }
    
    return 0;
}
