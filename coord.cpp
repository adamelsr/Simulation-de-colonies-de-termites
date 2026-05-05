#include "coord.hpp"
#include <cstdlib>
using namespace std;


Coord::Coord(int lig, int col): lig{lig}, col{col} {
	if ( lig <0 or lig >= tailleGrille or col<0 or col >= tailleGrille ){
		throw runtime_error("Les coordonnées ne sont pas dans la grille !");
	}
}

int Coord::getLig() const {
    return lig;
}

int Coord::getCol() const {
    return col;
}

ostream &operator<<(ostream &out, Coord a){
	out << "(" << a.getLig() << "," << a.getCol() << ")";
	return out;
}


bool Coord::operator==(const Coord &c) const {
    return lig== c.lig and col == c.col;
}

bool Coord::operator!=(const Coord &c) const {
	return lig != c.lig or col != c.col;
}

// PARTIE 2 : DIRECTION

ostream& operator<<(ostream &out, Direction d) {
    switch (d) { // On utilise un switch car on a un type énuméré dans notre cas
        case Direction::N:  out << "N";  break;
        case Direction::NE: out << "NE"; break;
        case Direction::E:  out << "E";  break;
        case Direction::SE: out << "SE"; break;
        case Direction::S:  out << "S";  break;
        case Direction::SW: out << "SW"; break;
        case Direction::W:  out << "W";  break;
        case Direction::NW: out << "NW"; break;
    }
    return out;
}

// Ici les types enumérés sont représentés comme des entiers : 0,1,2,3,4,5,6,7
// D'ailleurs on aurait pu reutilisé un switch pour renvoyer manuellement le resultat pour chaque direction utilisée  dans chaque fonction ci dessous mais c'est très long

Direction aDroite(Direction d) {
    return Direction((int(d) + 1) % 8); 
	// ici on veut tourner à droite donc on fait +1 et modulo 8 pour que cela reste valide dans le cas ou on est vers la fin
}

Direction aGauche(Direction d) {
    return Direction((int(d) + 7) % 8);
	// ici on veut tourner à gauche mais si on fait -1 alors qu'on est au début c'est invalide. Alors on fait +7 pour faire presque un tour complet ( on s'arrête juste avant la direction courante) et on fait %8 pour avoir la nouvelle direction à gauche.
}


Coord devantCoord(Coord c, Direction d){
	int l = c.getLig();
	int col= c.getCol();
	switch (d){
		case Direction::N: return Coord(l-1,col);
        case Direction::NE: return Coord(l-1,col+1);
        case Direction::E:  return Coord(l,col+1);
        case Direction::SE: return Coord(l+1,col+1);
        case Direction::S:  return Coord(l+1,col);
        case Direction::SW: return Coord(l+1,col-1);
        case Direction::W: return Coord(l,col-1); 
        case Direction::NW: return Coord(l-1,col-1);
		
	}
	throw runtime_error("On est au bord de la grille chef !");
	
}


// Suite au TD Projet termite, on veut aussi utiliser cette fonction qui renvoie une direction aléatoire donc on l'ajoute aussi dans ce fichier coord.cpp 
Direction dirAleatoire() {
    return Direction(rand() % 8); // et on utilise la fonction random 
}