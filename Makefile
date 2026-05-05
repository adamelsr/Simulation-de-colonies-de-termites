CXX=clang++
CXXFLAGS= -Wall -std=c++11 -g -O3
EXEC_FILES= coord.cpp test.cpp grille.cpp projet.cpp termite.cpp

all: tests projet
# Regle generique
%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# Pour exécuter les tests
tests: coord.o test.o grille.o termite.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# Pour exécuter le truc principal ( la simulation et affichage)
projet: projet.o coord.o grille.o termite.o
	$(CXX) $(LDFLAGS) -o $@ $^

coord.o: coord.hpp
test.o:  coord.hpp
grille.o: grille.hpp
projet.o:  projet.cpp  coord.hpp grille.hpp termite.hpp
termite.o: termite.cpp termite.hpp grille.hpp coord.hpp

check: tests
	./tests

clean:
	rm -f *.o tests projet
