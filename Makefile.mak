# Makefile for compiling MCFA solver and checker

all: finite-automaton checker

finite-automaton: finite-automaton.cpp
	g++ -std=c++17 -O2 -Wall -o finite-automaton finite-automaton.cpp \
		-lgecodesupport -lgecodekernel -lgecodeint -lgecodesearch \
		-lgecodeset -lgecodefloat -lgecodeminimodel

checker: checker.cc
	g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -o checker checker.cc

run: all
	chmod +x runner.sh
	./runner.sh

clean:
	rm -f finite-automaton checker tmp.dot tmp.png *.out
	rm -rf out/
