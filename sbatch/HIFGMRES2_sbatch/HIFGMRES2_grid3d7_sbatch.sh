#!/bin/bash
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
export LD_LIBRARY_PATH=/home/jyliu/HIFCpp/HIF/packages/elemental/lib:/home/jyliu/HIFCpp/HIF/packages/elemental/lib64:$LD_LIBRARY_PATH

cd /home/jyliu/new_results/HIFGMRES/grid3d7/

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid3d7data/A_grid3d7_16.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid3d7data/A_grid3d7_32.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid3d7data/A_grid3d7_64.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid3d7data/A_grid3d7_128.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true

/home/jyliu/HIFCpp/HIF/build/bin/HIFGMRES/testHIFGMRES2 --input_A "/home/jyliu/Axbdata/grid3d7data/A_grid3d7_256.txt" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
