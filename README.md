## Minimum Consistent Finite Automaton Solver ##

This program solves the Minimum Consistent Finite Automaton problem using Constraint Programming with the Gecode library in C++. It computes the smallest deterministic finite automaton (DFA) that:
- Accepts a given set of binary sequences
- Rejects another set of binary sequences
- Ensures full consistency with the input

## Requirements
- Linux OS (Ubuntu/Debian preferred)
- `g++` with C++17 support
- Gecode library
- Graphviz (optional, used by the checker tool)

## Installation
Install dependencies on Ubuntu/Debian:

sudo apt update
sudo apt install g++ libgecode-dev graphviz


## Files
- `finite-automaton.cpp`  Main constraint solver
- `checker.cc`  Provided checker to verify DFA correctness and generate diagrams
- `runner.sh`  Optional script to batch process all instances
- `instances/`  Folder containing test instance `.inp` files


## Compile the solver
g++ -std=c++17 -O2 -Wall finite-automaton.cpp -o finite-automaton \
  -lgecodesupport -lgecodekernel -lgecodeint -lgecodesearch \
  -lgecodeset -lgecodefloat -lgecodeminimodel



## Solve a single instance with a 60-second timeout:
timeout 60 ./finite-automaton < instances/example.inp > example.out



## Output Format
The solver writes the following to `stdout`:
1. Original input 
2. Blank line
3. Number of states (n)
4. For each state: `transition_on_0 transition_on_1 is_accepting`


## Batch Execution (Optional)
To run the solver and checker on all input files, use the `runner.sh` script. It will:
- Apply a timeout
- Write results to `out/`
- Validate outputs using the checker

### To use:
chmod +x runner.sh
./runner.sh


