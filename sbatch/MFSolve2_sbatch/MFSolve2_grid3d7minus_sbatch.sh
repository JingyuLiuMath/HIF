#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/MFSolve/grid3d7minus/

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7minusdata/A_grid3d7minus_16.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7minusdata/A_grid3d7minus_32.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7minusdata/A_grid3d7minus_64.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7minusdata/A_grid3d7minus_128.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true
