CXX = g++
FLAGS = -g -c -std=c++14
#CFLAGS=-I
DEPS = fileReading.hpp hashTable.hpp dataStructs.hpp metrics.hpp dynamicTimeWarping.hpp bruteForce.hpp
OBJ = fileReading.o hashTable.o dataStructs.o metrics.o dynamicTimeWarping.o bruteForce.o
OUT = cube lsh curve_grid_lsh brute bruteCurve curve_grid_hypercube

all: $(OUT)

curve_grid_lsh: mainCurve.o gridCurve.o LSH.o cube.o $(OBJ)
	$(CXX) -g mainCurve.o gridCurve.o LSH.o cube.o $(OBJ) -std=c++14 -o curve_grid_lsh

cube: mainCube.o cube.o $(OBJ)
	$(CXX) -g mainCube.o cube.o $(OBJ) -std=c++14 -o cube

lsh: mainLSH.o LSH.o $(OBJ)
	$(CXX) -g mainLSH.o LSH.o $(OBJ) -std=c++14 -o lsh

bruteCurve: bruteCurves.o bruteForce.o fileReading.o dynamicTimeWarping.o metrics.o dataStructs.o
	$(CXX) -g bruteCurves.o bruteForce.o fileReading.o dynamicTimeWarping.o metrics.o dataStructs.o -std=c++14 -o bruteCurve

curve_grid_hypercube: curve_grid_hypercube.o gridCurve.o LSH.o cube.o $(OBJ)
	$(CXX) -g curve_grid_hypercube.o gridCurve.o LSH.o cube.o $(OBJ) -std=c++14 -o curve_grid_hypercube

brute: brutePoints.o bruteForce.o fileReading.o dynamicTimeWarping.o metrics.o dataStructs.o
	$(CXX) -g brutePoints.o bruteForce.o fileReading.o dynamicTimeWarping.o metrics.o dataStructs.o -std=c++14 -o brute

curve_grid_hypercube.o: curve_grid_hypercube.cpp cube.hpp $(DEPS)
	$(CXX) $(FLAGS) curve_grid_hypercube.cpp -o curve_grid_hypercube.o

mainCube.o: mainCube.cpp cube.hpp $(DEPS)
	$(CXX) $(FLAGS) mainCube.cpp -o mainCube.o

mainLSH.o: mainLSH.cpp LSH.hpp $(DEPS)
	$(CXX) $(FLAGS) mainLSH.cpp -o mainLSH.o

mainCurve.o: mainCurve.cpp gridCurve.hpp LSH.hpp cube.hpp $(DEPS)
	$(CXX) $(FLAGS) mainCurve.cpp -o mainCurve.o

brutePoints.o: brutePoints.cpp bruteForce.hpp fileReading.hpp dataStructs.hpp
	$(CXX) $(FLAGS) brutePoints.cpp -o brutePoints.o

bruteCurves.o: bruteCurves.cpp bruteForce.hpp fileReading.hpp dataStructs.hpp
	$(CXX) $(FLAGS) bruteCurves.cpp -o bruteCurves.o

gridCurve.o: gridCurve.cpp gridCurve.hpp LSH.hpp cube.hpp dataStructs.hpp
	$(CXX) $(FLAGS) gridCurve.cpp -o gridCurve.o

LSH.o: LSH.cpp LSH.hpp hashTable.hpp dataStructs.hpp
	$(CXX) $(FLAGS) LSH.cpp -o LSH.o

cube.o: cube.cpp cube.hpp hashTable.hpp dataStructs.hpp
	$(CXX) $(FLAGS) cube.cpp -o cube.o

bruteForce.o: bruteForce.cpp bruteForce.hpp dynamicTimeWarping.hpp metrics.hpp dataStructs.hpp
	$(CXX) $(FLAGS) bruteForce.cpp -o bruteForce.o

fileReading.o: fileReading.cpp fileReading.hpp dataStructs.hpp
	$(CXX) $(FLAGS) fileReading.cpp -o fileReading.o

hashTable.o: hashTable.cpp hashTable.hpp dataStructs.hpp
	$(CXX) $(FLAGS) hashTable.cpp -o hashTable.o

metrics.o: metrics.cpp metrics.hpp dataStructs.hpp
	$(CXX) $(FLAGS) metrics.cpp -o metrics.o

dynamicTimeWarping.o: dynamicTimeWarping.cpp dynamicTimeWarping.hpp metrics.hpp dataStructs.hpp
	$(CXX) $(FLAGS) dynamicTimeWarping.cpp -o dynamicTimeWarping.o

dataStructs.o: dataStructs.cpp dataStructs.hpp
	$(CXX) $(FLAGS) dataStructs.cpp -o dataStructs.o

clean:
	rm *.o $(OUT)