#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <array>
#include <iostream>
#include <stdexcept>
#include "coord.hpp"

  // Pas besoin de remettre le taille de la grille pour le tableau qu'on va utiliser ensuite car on a deja inclue le coord.hpp

struct Grille {

	private:
	
		struct Case { 
			int termite; // son numéro de termite et  -1 s'il y a pas de termite
			bool brindille; // renvoie true (1) s'il y a une brindille et false (0) s'il y a pas 
			Case() : termite{-1}, brindille{false} {} //  constructeur avec liste d’initialisation donc quand on crée une case vide on ini directe les attributs avec  la brindille en faux et le termite en -1 !
		};
		
		std::array<std::array<Case, tailleGrille>, tailleGrille> grille; // q25 Tableau 2D de case 

	

	public:
	    // Constructeur  grille vide 
	    Grille();
		void poseBrindille(Coord c);
		void enleveBrindille(Coord c);
		bool contientBrindille(Coord c) const;
		void poseTermite(Coord c, int idT);
		void enleveTermite(Coord c);
		int numeroTermite(Coord c) const;
		bool estVide(Coord c) const;
};


/** On peut representer la grille de cette manière  : 
"*" brindille, "T" termite, "." vide **/
std::ostream& operator<<(std::ostream &out, const Grille &g);

#endif