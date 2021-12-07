# Calculateurs à mémoire partagée

## Programmation sur machine à mémoire partagée

<font color=red>Définition: </font>**Processus** - Créée par l’OS ou un exécutable  

<font color=red>Définition: </font>**Thread** - Exécutable léger créer par un processus

<font color=red>Définition: </font>**hyperthreading** - Deux fois plus de décodeur d’instruction que d’unités de calcul; Deux threads sur une unité de calcul peuvent se partager les ressources de cette unité

<font color=red>Définition: </font>**Datarace condition** - Un thread lit une variable partagée “pendant” qu’un deuxième thread la modifie; Les deux threads tentent de modifier “simultanément” la même variable. 

<font color=red>Définition: </font>**Affnité** - Le système d’exploitation attache un thread à une unité de calcul spécifique

- Memory bound : Quand la vitesse d’exécution de l’application limitée par la vitesse d’accès aux données  
- cpu bound

## Utilisation des threads en C++ 2011

```c++
#include <thread>
std::thread t(Fonction, args...);
t.join();

#include <mutex>
std::mutex m; //m.lock(), m.unlock()

#include <atomic>
```



## OpenMP

<font color=blue>(p.30)</font>

```C++
#include <omp.h>
# pragma omp for ...{}
# pragma omp single{}
# pragma omp master{}
```



## Autres outils existant

- la difference entre memoire cache et interlace

