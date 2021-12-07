# GPGPUs

## Architecture des GPGPUs

- Relation CPU–GPGPU : 
  - Le GPGPU est contrôlé par le CPU comme calculateur hybride MIMD–SIMD pour exécuter des algorithmes adaptés à son architecture;
  - CPU et GPGPU sont des calculateurs multi-cœurs et ont une mémoire architecturée sous forme hiérarchique.  
- “Qualifieurs” de CUDA  : <font color=blue>(p.20)</font>

## Modèle de programmation  

- Allocation mémoire : `cudaMalloc()`, `cudaFree()`, `cudaMemcpy()`

## Travaux dirigés n°4

- Installer TBB : `sudo apt install libtbb-dev  `