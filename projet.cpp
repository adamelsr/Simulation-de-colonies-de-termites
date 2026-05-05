#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <string>
#include <cstdio>

#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"

using namespace std;

/** Ordre d'implementation selon le projet ? :
1) compteBrindilles
2) afficheJeu
3) initialise
4) verifieCoherence
5) unePasse
6) main (qui change logiquement au fur et a mesure)
**/

//Fonction n°2 ( par ordre d'implementation):  Affichage de la grille avec direction des termites

/** On va afficher la grille avec les directions des termites
 En particulier, avec toString() du termite pour le caractère d'affichage.
 **/
void afficheJeu(const Grille &g, const vector<Termite> &termites) {
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) { //  Boucles imbriquées car tableau 2D
            Coord c(i, j); 
            if (g.contientBrindille(c)) {
                cout << "* ";
            } else if (g.numeroTermite(c) != -1) {
                int idT = g.numeroTermite(c); // id du termite si il existe l’identifiant dans la case (i, j)
                cout << termites[idT].toString() << " "; // symbole du termite et un espace pour la lisibilité
            } else {
                cout << ". "; // represente case vide ici
            }
        }
        cout << endl;
    }
    cout << endl;
}


// Fonction 5: Permet de verifier que la grille et le tableau de termites sont cohérentes ! ( on stocke informat° en double )
void verifieCoherence(const Grille &g, const vector<Termite> &termites) { // on ne modif rien, on veut juste verifier
	
// Partie 1: Vérifier chaque termite 
	
    for (int i = 0; i < int(termites.size()); i++) {
		// On parcourt tout d'abord le tab de termite
        Coord pos = termites[i].getPosition(); // on recucp la position que la termite courante pense avoir dans le tab
        int numGrille = g.numeroTermite(pos); // on regarde à la position pos , quel termite est présente ?
		
        if (numGrille != i) {
			// si la termite dans le tab est dans une case (x,y) mais que la grille voit qu'à cette position/ case, il y a un autre termite ou rien ( à cause de l'ID ) alors il y a une incohérence et on renvoie une erreur.
            throw runtime_error(" Il y a une incoherence!! Le termite " + to_string(i) + " pense etre en (" + to_string(pos.getLig()) + "," + to_string(pos.getCol()) + ") mais la grille dit " + to_string(numGrille));
        }
		
        if (g.contientBrindille(pos)) {
		// Maintenant, grâce à avant, si on a un termite à une case (x,y) et qu'on a aussi une brindille à cette même position , alors il y a une erreur d'après la consigne car on peut pas avoir les 2 à la fois.
            throw runtime_error("Une incoherence est là !! Le termite " + to_string(i) + " est sur une case avec brindille. Or, on ne peut pas voir 2 choses à la fois sur une même position");
        }
    }
	
    // Partie 2:  Vérifier chaque case contenant un termite
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
		// On parcourt mtn la grille ( Tableau 2D)
            Coord c(i, j); // on verifie pour chaque case du tab
            int idT = g.numeroTermite(c); // on stocke l'ID du termite pour comparer
            if (idT != -1) { // dans le cas ou il y a bien une termite à cette position
                if (idT >= int(termites.size())) { // >= car dans le tab on commence à 0 donc l'ID s'arrete à size -1.
	// si l'ID du termite dans la case courante de la grille a un nombre sup aux nombres total de termites dans le tab , cela veut dire que la grille a plus et donc a pas le même nombre de termite que dans le tab.
                    throw runtime_error("Incoherence: case (" + to_string(i) + "," +to_string(j) + ") contient id=" + to_string(idT) + " inexistant");
                }
                if (not (termites[idT].getPosition() == c)) {
			// Si dans le tab de termite, la position du termite idT n'a pas la même position que dans la grille à la case c, alors il y a une autre incohérence entre grille et tab de termites.
                    throw runtime_error(" Il y a une incoherence !! La case (" + to_string(i) + "," + to_string(j) + ") dit contenir termite " + to_string(idT) +" mais ce termite pense etre ailleurs");
                }
            }
        }
    }
}


//  Fonction n°1 ( par ordre d'implementation):  Permet de compter le nombre de brindilles sur la grille ( utile pour simulation et verifier cohérence)
int compteBrindilles(const Grille &g) {
    int compteur = 0;
    for (int i = 0; i < tailleGrille; i++)
        for (int j = 0; j < tailleGrille; j++)
            if (g.contientBrindille(Coord(i, j)))
				compteur++;
    return compteur;
}


/** Fonction n°3: Initialisation de la simulation **/

// Initialise la grille avec des brindilles et des termites placés aléatoirement 
void initialise(Grille &g, vector<Termite> &termites) {
    // On pose les brindilles aléatoirement selon densiteBrindille
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            if (float(rand()) / RAND_MAX < densiteBrindille) {
			// meme logique que marcheAleatoire
                g.poseBrindille(Coord(i, j));
            }
        }
    }
    // On commence à placer les termites sur des cases vides
    int idT = 0; // ici correspond à l'index du termite dans le vector qui est une partie integrante des paramètres du vector termite. 
	
    while (idT < nbTermites) { // tant qu'on a pas 20 Termites avec leur id en gros
        int lig = rand() % tailleGrille; // ligne aleatoirement qui ne depasse par tailleGrille
        int col = rand() % tailleGrille; // de même pour colonne
        Coord c(lig, col); // on ini une case avec les lignes et colonnes
        if (g.estVide(c)) { // si la case de la grille courante est vide, alors on peut commencer à mettre une termite avec ses attributs
            Direction d = dirAleatoire(); // on met une dir aléatoire pour ce termite 
            termites.push_back(Termite(idT, c, d)); //et on le rajoute dans le tableau de termites
            g.poseTermite(c, idT); // on peut poser la termite dans la grille
            idT++; // +1 à l'index termite car on ajoute un termite dans le vector jusqu'à arrriver à nbTermite
        }
    }
}

/** Fonction 4 : Boucle de simulation **/
// Effectue une passe = chaque termite effectue une étape, tour de simulation i.e  un cycle complet où chaque termite agit une fois
void unePasse(Grille &g, vector<Termite> &termites) {
    for (int i = 0; i < int(termites.size()); i++) {
        termites[i].vieTermite(g);
    }
}


int main(int argc, const char **argv) {
	// Lancer les tests doctest
    doctest::Context context(argc, argv);
    int test_result = context.run();
    if (context.shouldExit()) return test_result;
	   // Pour la Simulation 
    srand(time(NULL));

/** Ordre pour le main à suivre après le truc du debut : 
1) création grille + termites
2) initialise
3) affichage + verifieCoherence
4) boucle user
5) appel unePasse
6) verifieCoherence
**/
    Grille g0;
    // On ajoute les  brindilles: 
	g0.poseBrindille(Coord(0, 0));
    g0.poseBrindille(Coord(2, 3));
    g0.poseBrindille(Coord(5, 5));

    // Et on ajoute aussi les termites
    g0.poseTermite(Coord(1, 1), 0);
    g0.poseTermite(Coord(3, 4), 1);
    g0.poseTermite(Coord(7, 2), 2);

    // Affichage en écriture ( debut du projet)
    cout << "Voici l'affichage de la grille en version ecriture ( debut) : "<<endl;
    cout << g0 << endl;

	/** SUITE DU PROJET **/
	cout<<"\n"<<endl; // Pour séparer les parties ( lisibilité)
	
	cout<<"*** LE JEU DES TERMITES ***"<<endl;

	cout<<"Affichage de l'initialisation d'une simulation faite par le hasard : "<<endl;
	Grille g;
    vector<Termite> termites;
    initialise(g, termites);
	afficheJeu(g, termites);

	// On verifie initialement la cohérence du Jeu: 
    verifieCoherence(g, termites);
    cout << "Initialisation OK et bien valide !" <<endl<< "Il y a "<< int(termites.size()) << " termites et " << compteBrindilles(g) << " brindilles !\n"<<endl<<"(On pourra eventuellement verifier en comptant les termites et brindilles directement sur l'affichage !)"<<endl;

	int nbPasse = 10; // à chaque fois on fait 10 etapes de simultation
   
		// Commande pour interagir avec l'utilisateur
	cout << "\nVoici les commandes disponibles ( à retenir pour le jeu car on l'affiche une seule fois !) : \n\nEntrée (touche) : " <<"Realiser "<< nbPasse << " passes ( nombre de passes courants)  \n. :quitter  \n* :accelerer ( passes x2 )  \n/ :ralentir (passes divisées par 2)\n" << endl;

// D'après page 7, q5 PDF- projet_termite:

	char c = ' '; // char est un type de donnée qui permet de stocker un caractère à l'aide d'une variable. Ici, c'est très important de mettre ' ' et pas " " car c'est un char et pas un string
	
	while (c != '.') { // tant que l'utilisateur ne rentre pas ".", le programme ne s'arrete pas

	    cout << "*** LE JEU DES TERMITES ***" << endl;
		cout << "\n[passes=" << nbPasse << "  brindilles=" << compteBrindilles(g) << "]\nEntrez votre commande : ";

		c = getchar(); //// Attend la réponse de l'utilisateur getchar() permet de détecter directement la touche Entrée, qui est nécessaire pour controler la simulation. Avec cin, ça ne marcherait pas car il ignore les retours à la ligne.

	    if (c == '.') {
	        break; // on arrete la simulation
	    } else if (c == '*') { // On accelere en *2 
	        nbPasse *= 2;
	    } else if (c == '/') { // on ralentit (/2)
	        if (nbPasse > 1)
	            nbPasse /= 2;
	    }

    // Enter ou autre commande : on exécute nbPasse fois de passes
    while (getchar() != '\n') {} // Nettoyer l'entrée de l'utilisateur

    for (int p = 0; p < nbPasse; p++) {
        unePasse(g, termites); // on fait la simulation réelle en fonction du nombre de passe ( selon la commande de l'utilisateur)
    }

    afficheJeu(g, termites); // on affiche l'evolution et le resultat suite aux changements

 // On verifie toujours la cohérence après chaque série de passes
    verifieCoherence(g, termites);
}

	cout << "\nC'est la fin de simulation ! \nBrindilles restantes : "<<  compteBrindilles(g) << endl;
	return 0;
}