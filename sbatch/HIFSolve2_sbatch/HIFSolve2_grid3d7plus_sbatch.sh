#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/HIFSolve/grid3d7plus/

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7plusdata/A_grid3d7plus_16.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7plusdata/A_grid3d7plus_32.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7plusdata/A_grid3d7plus_64.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7plusdata/A_grid3d7plus_128.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid3d7plusdata/A_grid3d7plus_256.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
