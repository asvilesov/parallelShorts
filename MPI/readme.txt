run "make"

run this command to pass a message between processors (-np is a flag for how many procs are used)
"mpirun -np 4 ./ring"

run variants of this command to add the numbers in numbers.txt
"mpirun -np 4 ./sum1"
"mpirun -np 4 ./sum2"
"mpirun -np 4 ./sum3"