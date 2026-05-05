#include "termite.hpp"
#include "doctest.h"
#include <cstdlib>
using namespace std;

// Termite : on initialise le constructeur et ses attributs avec les param :
Termite::Termite(int i, Coord pos, Direction d): id{i}, position{pos}, dir{d},porteBrindille{false}, sablier{0}, tourneSurPlace{false} {}

// Rappel : Ces variables ci dessous sont privées, donc elles ne sont accessibles que depuis la classe termite !
int Termite::getId() const {
	return id; 
}
Coord Termite::getPosition() const {
	return position; 
}
Direction Termite::directionTermite() const {
	return dir; 
}
bool Termite::estPorteBrindille() const {
	return porteBrindille; // on utilise l'attribut bool porteBrindille pour voir si le termite porte bien une brindille.
}


string Termite::toString() const{ // Affichage de la direction des termites. On va donc utiliser un switch avec la direction ( enum class) et les str.
// D'après PDF- projet_termites Page 4: Affichage de la direction des termites
	
	string resultat;
	switch(dir){ // on omet juste la fleche qui va du depart à l'arrivée car on peut pas le representer
		case Direction::N:  resultat = "|"; break;
        case Direction::S:  resultat = "|"; break;
        case Direction::E:  resultat = "-"; break;
        case Direction::W:  resultat = "-"; break;
        case Direction::NE: resultat = "/"; break;
        case Direction::SW: resultat = "/"; break;
        case Direction::NW: resultat = "\\"; break; // le "\" ne marche pas bizarrement 
        case Direction::SE: resultat = "\\"; break; // le "\" ne marche pas bizarrement 
	    
      default: resultat = "?"; break; // dans le cas où il y a un problème dans la direction, on met ca pour montrer qu'il y a un bug.
    }
	    if (porteBrindille) 
			resultat = "X";
    return resultat;
}


Coord Termite::devant() const { // le constcar on veut juste renvoyer et pas modif
    return devantCoord(position, dir); // on utilise la fonction qu'on a implementé dans coord en utilisant les coordonnées de la case devant le termite 
}


void Termite::tourneADroite() {
	dir = aDroite(dir); 
}
void Termite::tourneAGauche() { 
	dir = aGauche(dir); 
}
void Termite::tourneAleat()   {
	dir = dirAleatoire(); // comme ici on demande de tourner avec une direction aléatoire donc on a ajouté une fonction dirAleatoire à coord.hpp pour renvoyer une direction aleatoire et on remplace l'attribut dir du termite par le nv
}


	   /** PARTIE 2- Manipule termite avec la grille **/

// IDEE: comme dans nos fonctions bool  on avait mis des cas de gestions d'erreurs, alors on va utiliser la methode try et catch pour implementer ces methodes.

bool Termite::laVoieEstLibre(const Grille &g) const { // renvoie true si une case devant un termite est libre
    try {
        Coord c = devantCoord(position, dir);
        return g.estVide(c);
    } catch (runtime_error &) { // si on est en dehors de la grille avec la fonction devantCoord ( car on renvoie une erreur runtime_error dans la fonction), on rattra)e une erreur car ici c'est normal. On est dans un booléen.
        return false;
    }
}


bool Termite::brindilleEnFace(const Grille &g) const {
    try {
        Coord c = devantCoord(position, dir);
        return g.contientBrindille(c);
    } catch (runtime_error &) {
        return false;
    }
}

int Termite::voisinsLibres(const Grille &g) const {
    int compteur = 0; // compteur du nombre de cases libres autour du termite
    for (int i = 0; i < 8; i++) { // ici on va parcourir toutes les directions possibles
        try {
            Coord c = devantCoord(position, Direction(i)); // un enum class est representé aussi sous forme de int ( avec l'ordre) donc cela marche
            if (g.estVide(c))
				compteur++; // si une case est vide alors on compte +1
        } catch (runtime_error &) {} // si on peut pas voir ce qu'il y a car on est en dehors de la grille, on "attrape l'erreur" pour qu'on puisse continuer et on fait rien.
    }
    return compteur;
}



void Termite::avanceTermite(Grille &g) {
    Coord nouveau = devantCoord(position, dir);
    g.enleveTermite(position); // on enleve le termite de la position ini
    g.poseTermite(nouveau, id); // pour ensuite le mettre dans la nouvelle case sans modifier l'id de la termite
    position = nouveau; // Et on modifie les coordonnées de la termite !
}



void Termite::chargeBrindille(Grille &g) { // ramasse brindille devant lui
    Coord c = devantCoord(position, dir);
    g.enleveBrindille(c); // on enleve la brindille de la case
    porteBrindille = true; // et on met que la termite porte bien la brindille grâce à l'attribut.
}


void Termite::dechargeBrindille(Grille &g) { // Pose la brindille portée dans la case devant le termite
    Coord c = devantCoord(position, dir);
    g.poseBrindille(c);
    porteBrindille = false;
}

void Termite::marcheAleatoire(Grille &g) {
    if (not laVoieEstLibre(g)) 
        // si il y a un obstacle devant et bah on retourne aleatoirement jusqu'à que cela soit possible
        tourneAleat();
		else {
	// Le cas 2 (else) vient de ici : l’énoncé du projet (la partie “Comportement des termites”) : 
// “Lors du déplacement aléatoire, si la case devant est libre, le termite a probaTourner de chance de tourner dans une direction au hasard, sinon il avance tout droit.”
        // si la voie est libre : on avance sauf si on décide de tourner
        if (float(rand()) / RAND_MAX < probaTourner) {
		// transforme un nombre entier aléatoire en nombre réel entre 0 et 1 et on compare avec la proba de tourner. float car sinon divison entre int = division entiere. 
            tourneAleat();
        } else {
            avanceTermite(g);
        }
    }
}


void Termite::vieTermite(Grille &g) {
	
	// CAS 1: Termite a une brindille
    if (porteBrindille) {
        // si Le termite porte une brindille 
		
        if (sablier > 0) {
            // Et qu'il reste un nombre sablier de deplacement avant  de pouvoir poser la brindille, alors on continue
            sablier--;
            marcheAleatoire(g);

		// CAS 1.1: Termite a une brindille et peut poser dans un tas
        } else {
            // Dans le cas ou Sablier=0  : On peut poser mais il faut chercher où poser
			
            if (tourneSurPlace) {
                // Chercher une case libre devant, en s'assurant de ne pas s'enfermer en tournant sur lui même car il a deja trouvé un tas ou poser la brindille
                if (laVoieEstLibre(g) and voisinsLibres(g) >= 2) {
	// On veut que la case devant soit libre pour poser la brindille mais il faut aussi ne pas s'enfermer! D'ou le fait d'avoir 2 cases voisins libres pour pouvoir après sortir ! Car si on a juste une case bah on s'enferme
                    dechargeBrindille(g); // dans le cas ou cela respecte la condition on peut poser la brindille
                    sablier = dureeSablier;
				// Mais, il ne va pas reprendre directement la brindille. Et une fois placé, on reinitialise le compteur sablier à dureeSablier pour quand un termite ramasse de nouveau une brindille.
                    tourneSurPlace = false; 
			// il a trouvé ou poser donc, plus besoin de tourner.
					
                } else {
                    tourneADroite();
			// Si on trouve pas de case libre qui nous enferment pas : Le termite ne tourne que d’un 8e de tour( donc de 1 à droite)  à chaque étape de simulation.il est donc possible qu’il tourne sur place pendant un moment avant de trouver une case où poser la brindille. d'après projet_termite p3
				}
				
		// CAS 1.2 : Termite a une brindille et peut poser mais n'a pas d'endroits encore.
            } else {
                // Pas encore en mode dépôt donc quand termite porte bien une brindille mais n’a pas encore trouvé un tas où la déposer
                if (brindilleEnFace(g)) { // s'il trouve une brindille en face / un tas.
                    tourneSurPlace = true;
				//car selon PDF, le termite doit poser près d’un tas existant. Donc si il trouve un tas, il tourneSurPlace i.e cherche une case libre autour de ce tas.
                } else { 
				// s'il trouve pas de brindille, de tas devant alors il continue à marcher aleatoirement
                    marcheAleatoire(g);
                }
            }
        }
// CAS 2 : termite ne porte pas de brindille 
    } else {
        if (sablier > 0) {
            // Délai après avoir posé donc on veut pas reprendre immédiatement dans le cas où on a posé une brindille selon PDF Page  3
            sablier--;
            marcheAleatoire(g);
		// après avoir posé une brindille,le termite doit attendre avant d’en reprendre une autre donc faire dureesablier deplacements.
			
        } else {
            // Si on a pas posé de brindille et qu'on est prêt à en ramasser une
            if (brindilleEnFace(g)) {
                chargeBrindille(g);
                sablier = dureeSablier;
			// s'il y a une brindille devant moi, on le ramasse et on remet le compteur à dureeSablier.
            } else {
                marcheAleatoire(g);
			// Sinon on continue de marcher dans une dir aleatoire !
            }
        }
    }
}
	
