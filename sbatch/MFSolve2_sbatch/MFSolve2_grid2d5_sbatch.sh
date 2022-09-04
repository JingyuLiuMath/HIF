#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/MFSolve/grid2d5/

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_64.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_128.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_256.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_512.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_1024.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_2048.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFSolve/testHIFSolve2 --input_A "/home/jyliu/Axbdata/grid2d5data/A_grid2d5_4096.txt" --minvtx 64 --HIFbutton false --tol 1e-3 --logApp true
