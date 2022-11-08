#!/bin/bash

# -
# |
# | This is a batch script for running a MPI parallel job on Summit
# |
# | (o) To submit this job, enter:  sbatch --export=CODE='/home/scru5660/HPSC/codes/fd_mpi/src' ex_01.bat 
# |
# | (o) To check the status of this job, enter: squeue -u <username>
# |
# -

# -
# |
# | Part 1: Directives
# |
# -

#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --time=10:00:00
#SBATCH --partition=amilan
#SBATCH --output=../out/ex01-%j.out

# -
# |
# | Part 2: Loading software
# |
# -

module purge
module load intel
module load impi 

# -
# |
# | Part 3: User scripting
# |
# -

echo "=="
echo "||"
echo "|| Begin Execution of fd in slurm batch script."
echo "||"
echo "=="


rm -rf *.plt
rm -rf phi_dump.bin

rm pc_phi_*

#--------------------------------------------------------------------------------------------------------
# 4PE run
#--------------------------------------------------------------------------------------------------------
ls

mpirun -n 4 ./transientDiffusion -nPEx 2 -nPEy 2 -nCellx  3 -nCelly 3 \
-solver jacobi -tEnd .15  -dt .001 -tPlot .001 

./writePlotCmd_py3.py

./sb_py3.py -f output.bin -c 4 -n 2 > python_output.txt

echo "=="
echo "||"
echo "|| Execution of solver in slurm batch script complete."
echo "||"
echo "=="

