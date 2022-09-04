#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/HIFSolve/grid3d7quasi/

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7quasidata/A_grid3d7quasi_16.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7quasidata/A_grid3d7quasi_32.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7quasidata/A_grid3d7quasi_64.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7quasidata/A_grid3d7quasi_128.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7quasidata/A_grid3d7quasi_256.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
