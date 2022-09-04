#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/HIFGMRES/grid2dt/

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid2dtdata/A_grid2dt_128.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid2dtdata/A_grid2dt_256.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid2dtdata/A_grid2dt_512.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid2dtdata/A_grid2dt_1024.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid2dtdata/A_grid2dt_2048.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
