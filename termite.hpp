#ifndef TERMITE_HPP
#define TERMITE_HPP

#include <string>
#include "grille.hpp"
#include "coord.hpp"

// D'après projet_termite page 2:
const float probaTourner = 0.1; // Probabilité de tourner au hasard quand la voie est libre 
const int dureeSablier = 6; // Nombre de déplacements min a attendre avant de poser ou ramasser une brindille 

// D'après projet_termite page 4 : 
const float densiteBrindille=0.05; // densité ini d'une brindille
const int nbTermites=20; // le nombre de termites


class Termite { 
// d'après TD Page 2: PARTIE 1 - types de fonctions qui concernent seulement le termite

	private: 
	    int id;
	    Coord position;
	    Direction dir; // car on veut renvoyer la direction d'un termite
		// Page 7- On ajoute leur comportement complet
	    bool porteBrindille; // on veut renvoyer si termite porte brindille ou non
	    int sablier; // stocker le compteur de deplacement qui reste à un termite avant de pouvoir reposer brindille. 
// D'après projet_termites- Page 3 : "Quand il a ramassé une brindille, il doit attendre un certain nombre de déplacements (dureeSablier= 6 dans l’exemple ci-dessus) avant de pouvoir reposer la brindille." 

	    bool tourneSurPlace; /**puis ils disent : 
"Après s’être déplacé au moins dureeSablier fois, dès qu’il trouve devant lui une autre brindille, il tourne sur place ( sur lui même) jusqu’à avoir trouvé une case libre et pose sa brindille dans cette case." Donc cela peut nous être utile dans ce cas la. 
**/


/**En résume : 
porteBrindille=	est-ce qu’il porte quelque chose ?

sablier = combien de déplacements avant de pouvoir  poser quand on ramasse et vice verca aussi d'après : " mais il ne ramassera pas de nouvellebrindille avant dureeSablier nouveaux déplacements. Ainsi, il ne va pas reprendre la brindille qu’il vient juste de poser."

tourneSurPlace=	est-ce qu’il cherche une case libre si il a un trouvé un tas ( brindille devant) = true / false= cherche pas de case libre car il a pas trouvé encore de tas où poser sa brindille 
**/

	public: 
		/** un constructeur : crée un termite avec un id, placé en position, regardant dir
    -id  : indice dans le vecteur des termites
    -position : position sur la grille
    -dir : direction ini
     **/
    Termite(int id, Coord position, Direction dir);

	/** On veut pouvoir  juste  lire l'etat d'un termite ( sans le modif)  i.e pouvoir lire ces infos depuis l’extérieur (grille, affichage, tests)**/

	// Retourne l'identifiant 
    int getId() const;

    // Retourne la position 
    Coord getPosition() const;

    // Retourne la direction 
    Direction directionTermite() const;


	/** Accès **/
    // Convertit et retourne l'animal en chaines de caractères (pour l'affichage) i.e on veut afficher de la direction des termites

    std::string toString() const;

	// Retourne les coordonnées de la case devant le termite 
    Coord devant() const;


	/** Prédicats **/
	// Retourne true si le termite porte une brindille 
    bool estPorteBrindille() const;

	 /** Modifications de direction donc pas de const **/

    void tourneADroite();
    void tourneAGauche();
    void tourneAleat(); // fait tourner termite dans un sens aléatoire


	   /** PARTIE 2- Manipule termite avec la grille **/

    // Retourne true si la case devant est libre (dans les bornes et vide) 
    bool laVoieEstLibre(const Grille &g) const;

    // Retourne true si la case devant contient une brindille 
    bool brindilleEnFace(const Grille &g) const;

    // Retourne le nombre de cases voisines libres (parmi les 8 directions !) 
    int voisinsLibres(const Grille &g) const;

    // Avance d'une case dans la dir actuelle et modif la grille avec la termite
    void avanceTermite(Grille &g);

    // Déplacement aléatoire du termite d'une étape 
    void marcheAleatoire(Grille &g);

    // Ramasse la brindille qui est devant le termite 
    void chargeBrindille(Grille &g);

    // Pose la brindille portée dans la case devant le termite 
    void dechargeBrindille(Grille &g);

    // Algorithme principal d'une étape de vie du termite ( naissance + mort ?) 
    void vieTermite(Grille &g);
};
 
#endif
