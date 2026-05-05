#include "grille.hpp"
#include "doctest.h"
using namespace std;

/** REGLES DU JEU IMPORTANTES : 
Chaque case de la grille peut être vide, contenir une brindille ou un termite. Comme vu en TD,
les termites seront rangés dans un tableau et chaque termite aura un numéro correspondant à sa position dans le tableau.
**/

Grille::Grille() : grille{} {}

// IDEE: On peut reutiliser les methodes entre eux pour faire nos fonctions. En particulier le estVide. Cela est possible grâce au fichier hpp ou il y a toutes nos fonctions qui sont déclarées et le compilateur peut donc utiliser cela !


void Grille::poseBrindille(Coord c) {
    if (not estVide(c)) // car d'après la règle du jeu, on peut mettre une brindille seulement quand il y pas de termite et de brindille. (une case peut etre : soit rien, soit brindille, soit termite)
        throw invalid_argument(" OH, La case est pas vide ! C'est impossible de poser une brindille ici !\n");
    grille[c.getLig()][c.getCol()].brindille = true; // s'il n'y a pas d'exception alors après avoir posé la brindille, on peut mettre true car c'est bon.
}


void Grille::enleveBrindille(Coord c){ // Cela est différent de la méthode précédente
    if (not contientBrindille(c))
        throw invalid_argument(" OH, La case n'a pas de brindille ici ! On peut pas enlever de brindille sur cette case !");
    grille[c.getLig()][c.getCol()].brindille = false; // après avoir verfiée l'existence de la brindille, on peut mettre false car on l'a enlevé .
// car si on met estVide à la place de contientBrindille bah s'il y a un termite, cela va enlever une brindille qui n'existe pas car l'exception ne sera pas levée.
}
	

bool Grille::contientBrindille(Coord c) const {
	return grille[c.getLig()][c.getCol()].brindille; // renvoie true si cette condition est vraie
}


void Grille::poseTermite(Coord c, int idT){
	if (not estVide(c))
		throw invalid_argument(" OH, La case est pas vide ! C'est impossible de poser une termite ici !\n");
// car d'après la règle du jeu, on peut mettre une termite seulement quand il y pas de termite et de brindille. ( Une case peut etre :soit rien, soit brindille, soit termite)
	grille[c.getLig()][c.getCol()].termite=idT;

}


void Grille::enleveTermite(Coord c){
	if (grille[c.getLig()][c.getCol()].termite == -1)
		throw invalid_argument(" OH, La case n'a pas de de termite ici ! On peut pas enlever de termite sur cette case !");
	grille[c.getLig()][c.getCol()].termite=-1;
	
}


int Grille::numeroTermite(Coord c) const{ // ICI , j'ai enlevé l'accent car cela peut engendrer des erreurs.
	return grille[c.getLig()][c.getCol()].termite;
}



bool Grille::estVide(Coord c) const {
	if ((grille[c.getLig()][c.getCol()].brindille == false) and (grille[c.getLig()][c.getCol()].termite == -1)) // car brindille est un booléen et termite est un int et les 2 attributs font partie de la struct Case et donc comme on utilise un tableau 2D de case alors on veut accéder à l'un des 2 attributs pour chaque case;
        return true;
     else 
        return false;
}


ostream& operator<<(ostream &out, const Grille &g) {
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            Coord c(i, j); // on initialise une coordonnée c
            if (g.contientBrindille(c))
                out << "* ";
            else if (g.numeroTermite(c) != -1) // s'il ya bien une termite
                out << "T "; // je voulais mettre l'ID du termite mais malheureusement cela décale après la grille.
            else 
                out << ". "; // cas ou la case est vide
        }
        out <<endl;
    }
    return out;
}
