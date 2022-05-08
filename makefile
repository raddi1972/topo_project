
topo_proj : main.o Matrix.o
	g++ -o -Ofast topo_proj main.cpp Matrix.cpp