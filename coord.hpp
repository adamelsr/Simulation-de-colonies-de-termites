#ifndef COORD_HPP
#define COORD_HPP
 
#include <iostream>
#include <stdexcept>



const int tailleGrille=20; // On initialise car sinon cela renvoie une erreur

class Coord {

	public:
	// Constructeur qui prend en parametre lig et col et retourne une paire de type Coord
	Coord(int lig, int col);
	int getLig() const; // const car on ne modif rien. on cherche juste
	int getCol() const;// pareil
	bool operator==(const Coord &c) const; // on ne modif rien
    bool operator!=(const Coord &c) const; // de même 

	private:
		int lig, col;

	};


std::ostream& operator<<(std::ostream &out, Coord a); // ici on met std :: ostream& car car ostream est dans std 
// ET, on utilise pas namespace std car c'est une mauvaise pratique dans un fichier hpp 


// PARTIE 2 : DIRECTION


//  8 points cardinaux 
enum class Direction { N, NE, E, SE, S, SW, W, NW }; // d'après schema


std::ostream& operator<<(std::ostream &out, Direction d);

// D'après schéma
// Retourne la direction à gauche de celle courante (sens anti-horaire) 
Direction aGauche(Direction d);

// Retourne la direction à droite de celle courante (sens horaire) 
Direction aDroite(Direction d);

Coord devantCoord(Coord c, Direction d);

Direction dirAleatoire(); 
// Suite au TD Projet termite, on veut aussi utiliser cette fonction qui renvoie une direction aléatoire donc on l'ajoute aussi dans ce fichier
#endif