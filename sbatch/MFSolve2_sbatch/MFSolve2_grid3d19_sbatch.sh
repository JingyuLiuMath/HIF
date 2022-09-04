#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/MFSolve/grid3d19/

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d19data/A_grid3d19_16.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d19data/A_grid3d19_32.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d19data/A_grid3d19_64.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d19data/A_grid3d19_128.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true
