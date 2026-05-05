#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "coord.hpp"   
#include "grille.hpp"  
#include "termite.hpp"

using namespace std;
/** 
 Pour Compiler: 
-make

- Lancer  tests en bien:  
make check     ou ./tests - s (pour voir les tests qui passent)
**/
TEST_CASE("Constructeur Coord ") {
    Coord c(1, 2);
	CHECK(c.getLig() == 1);
    CHECK(c.getCol() == 2);
}

TEST_CASE("Coord en dehors grille") {
    CHECK_THROWS_AS(Coord(-1, 2),runtime_error);
    CHECK_THROWS_AS(Coord(2, -1),runtime_error);
    CHECK_THROWS_AS(Coord(tailleGrille, 0),runtime_error);
	CHECK_THROWS_AS(Coord(0, tailleGrille),runtime_error);
}



TEST_CASE("Coord cas limites ") {
    Coord c1(0, 0);
    CHECK(c1.getLig() == 0);
    CHECK(c1.getCol() == 0);

    Coord c2(tailleGrille - 1, tailleGrille - 1);
    CHECK(c2.getLig() == tailleGrille - 1);
    CHECK(c2.getCol() == tailleGrille - 1);
}

TEST_CASE("Affichage Coord") {
    Coord c(6, 7);
    ostringstream affiche;
    affiche << c;
    CHECK(affiche.str() == "(6,7)");
}


TEST_CASE("Operateur egalite Coord") {
    Coord a(6, 7);
    Coord b(6, 7);
    Coord c(7, 6);

	//test pour voir quand meme
    CHECK(a == a);
    CHECK(a == b);      // MEME coordonnées
    CHECK_FALSE(a == c);
    CHECK_FALSE(b == c);

    // test  entre 2 coordonnées
    CHECK((a == b) == (b == a));

    
}


// PARTIE 2- DIRECTION : 

TEST_CASE("aGauche et aDroite ") {
    Direction d = Direction::N;
	
	CHECK(aGauche(aDroite(d))==d);
	CHECK(aDroite(aGauche(d))==d);    

	    // Tester rotation  gauche 8 fois ( revient à la direction ini)
	for (int i = 0; i < 8; i++) {
        d = aGauche(d);
    }
    CHECK(d == Direction::N);  

    // Tester rotation  droite 8 fois ( revient à la direction ini)
    
    for (int i = 0; i < 8; i++) {
        d = aDroite(d); 
    }
    CHECK(d == Direction::N);  
}


TEST_CASE("devantCoord") {
    Coord c(5, 5);
    CHECK(devantCoord(c, Direction::N)  == Coord(4, 5));
    CHECK(devantCoord(c, Direction::S)  == Coord(6, 5));
    CHECK(devantCoord(c, Direction::E)  == Coord(5, 6));
    CHECK(devantCoord(c, Direction::W)  == Coord(5, 4));
    CHECK(devantCoord(c, Direction::NE) == Coord(4, 6));
    CHECK(devantCoord(c, Direction::NW) == Coord(4, 4));
    CHECK(devantCoord(c, Direction::SE) == Coord(6, 6));
    CHECK(devantCoord(c, Direction::SW) == Coord(6, 4));
	CHECK_THROWS_AS(devantCoord(Coord(0, 0), Direction::NE),  runtime_error);
    CHECK_THROWS_AS(devantCoord(Coord(0, 5), Direction::N),runtime_error);
    CHECK_THROWS_AS(devantCoord(Coord(tailleGrille - 1, 5), Direction::S), runtime_error);
}

TEST_CASE("Test Q21 Partie 2") {
    Coord ini(10, 10);  // 
    Direction d = Direction::N;  

    // Avance une  fois dans la direction nord
    Coord coord1 = devantCoord(ini, d);
    
    // On tourne 4 fois à droite (pour revenir a la direction ini)
    for (int i = 0; i < 4; ++i) {
        d = aDroite(d);
    }

    // On avance une 2 eme fois dans la direction après avoir fait 4 tours à droite
    Coord coord2 = devantCoord(coord1, d);

    // Et on verifie que la coord finale est = a la coord ini
    CHECK(coord2.getLig() == ini.getLig());
    CHECK(coord2.getCol() == ini.getCol());
}


// PARTIE 3- GRILLE : 

TEST_CASE("Methode: estVide") {
    Grille g;
    CHECK(g.estVide(Coord(0, 0)));
    CHECK(g.estVide(Coord(6, 7)));
    CHECK(g.estVide(Coord(tailleGrille - 1, tailleGrille - 1)));
}


TEST_CASE("METHODE:  pose brindille") {
	Grille g;
    Coord c(6, 7);

    g.poseBrindille(c);
    CHECK(g.contientBrindille(c));
    CHECK_FALSE(g.estVide(c));

	// Avec exception : 
	Grille g2;
    Coord c2(3, 3);
    g2.poseBrindille(c2);
    CHECK_THROWS_AS(g2.poseBrindille(c2), invalid_argument);   
}


TEST_CASE("Methode: enlever brindille") {
    Grille g;
    Coord c(6, 7);
	g.poseBrindille(c);
    g.enleveBrindille(c);
    CHECK_FALSE(g.contientBrindille(c));
    CHECK(g.estVide(c));

	// Avec exception: 
	Grille g2;
    Coord c2(3, 3);
	CHECK_THROWS_AS(g2.enleveBrindille(c2), invalid_argument);
}


TEST_CASE("METHODE: contientBrindille") {
    Grille g;
    Coord c(2, 2);

    // etat Ini
    CHECK_FALSE(g.contientBrindille(c));

    //  après pose
    g.poseBrindille(c);
    CHECK(g.contientBrindille(c));

    // après enlever
    g.enleveBrindille(c);
    CHECK_FALSE(g.contientBrindille(c));

    // independance avec termite
    g.poseTermite(c, 3);
    CHECK_FALSE(g.contientBrindille(c));
}

TEST_CASE("Methode: poseTermite") {
    Grille g;
    Coord c(7, 8);
    CHECK(g.numeroTermite(c) == -1);
    CHECK(g.estVide(c));

    g.poseTermite(c, 3);
    CHECK(g.numeroTermite(c) == 3);
    CHECK_FALSE(g.estVide(c));
	
// Avec exception : 
	Grille g2;
    Coord c2(4, 4);
    g2.poseTermite(c2, 0);
    CHECK_THROWS_AS(g2.poseTermite(c2, 1), invalid_argument);
    g2.enleveTermite(c2);

    g2.poseBrindille(c2);
    CHECK_THROWS_AS(g2.poseTermite(c2, 0), invalid_argument);
}

TEST_CASE("Methode: enleveTermite") {
    Grille g;
    Coord c(7, 8);
    CHECK(g.numeroTermite(c) == -1);
    CHECK(g.estVide(c));

    g.poseTermite(c, 3);
    CHECK(g.numeroTermite(c) == 3);
    CHECK_FALSE(g.estVide(c));

    g.enleveTermite(c);
    CHECK(g.numeroTermite(c) == -1);
    CHECK(g.estVide(c));

	// avec exception: 
	Grille g2;
    Coord c2(4, 4);
    CHECK_THROWS_AS(g2.enleveTermite(c2), invalid_argument);
}


TEST_CASE("Methode: numeroTermite") { // En vrai, on l'a déja testé en utilisant cette méthode pour tester les autres fonctions.
    Grille g;
    Coord c(3, 3);

    //  Etat ini
    CHECK(g.numeroTermite(c) == -1);

    // apres pose Termite
    g.poseTermite(c, 7);
    CHECK(g.numeroTermite(c) == 7);

    // Et on enleve 
    g.enleveTermite(c);
    CHECK(g.numeroTermite(c) == -1);

    //  on regarde independance avec brindille
    g.poseBrindille(c);
    CHECK(g.numeroTermite(c) == -1);
}


TEST_CASE("Autre cas de test pour Grille: brindille et termite occupent cases independantes ( sans que l'un influe sur l'autre)") {
    Grille g;
    Coord c1(1, 1);
    Coord c2(2, 2);
    g.poseBrindille(c1);
    g.poseTermite(c2, 0);
    CHECK(g.contientBrindille(c1));
    CHECK_FALSE(g.contientBrindille(c2));
    CHECK(g.numeroTermite(c2) == 0);
    CHECK(g.numeroTermite(c1) == -1);
}


/** PARTIE 4- TERMITE : **/ 


TEST_CASE("Termite: constructeur + lecture ") {
    Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::N);

    CHECK(t.getId() == 0);
    CHECK(t.getPosition() == c);
    CHECK(t.directionTermite() == Direction::N);
    CHECK_FALSE(t.estPorteBrindille());
}

TEST_CASE("Termite:Affichage avec  toString") {
    Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);

    Termite tN(0, c, Direction::N);
    CHECK(tN.toString() == "|");

    Termite tE(0, c, Direction::E);
    CHECK(tE.toString() == "-");

    Termite tNE(0, c, Direction::NE);
    CHECK(tNE.toString() == "/");

    Termite tNW(0, c, Direction::NW);
    CHECK(tNW.toString() == "\\");

	// Test toString quand le termite porte une brindille ==> doit afficher "X"
    Grille g2;
    Coord cT(8, 8);
    Coord cB(8, 9);
    g2.poseTermite(cT, 0);
    g2.poseBrindille(cB);
    Termite tPorte(0, cT, Direction::E);
    tPorte.chargeBrindille(g2);
    CHECK(tPorte.toString() == "X");
}



TEST_CASE("Termite: tourneADroite ") {
    Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::N);

    t.tourneADroite();
    CHECK(t.directionTermite() == Direction::NE);
	t.tourneADroite();
    CHECK(t.directionTermite() == Direction::E);
}

TEST_CASE("Termite: tourneAGauche") {
	Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::N);
	
	t.tourneAGauche();
    CHECK(t.directionTermite() == Direction::NW);
    t.tourneAGauche();
    CHECK(t.directionTermite() == Direction::W);
}

// C'est un peu plus compliqué de tester TourneAleat avec tous les cas possibles. En particulier, si on met le or dans le doctest, cela marche pas et renvoie une erreur quand on compile donc on a decidé de pas le mettre  

TEST_CASE("Termite: laVoieEstLibre") {
    Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::S);

    CHECK(t.laVoieEstLibre(g));

    g.poseBrindille(Coord(6, 5));
    CHECK_FALSE(t.laVoieEstLibre(g)); // renvoie false car il y a une brindille devant
    g.enleveBrindille(Coord(6, 5));

    g.poseTermite(Coord(6, 5), 1);
    CHECK_FALSE(t.laVoieEstLibre(g)); //  // renvoie false car il y a un termite devant
    g.enleveTermite(Coord(6, 5));

    //  au bord de grille : voie libre impossible
    g.enleveTermite(c);
    Coord bord(0, 5);
    g.poseTermite(bord, 0);
    Termite t2(0, bord, Direction::N); // cela fait 0-1 = -1 donc impossible
    CHECK_FALSE(t2.laVoieEstLibre(g));
}


TEST_CASE("Termite: brindilleEnFace") {
    Grille g;
    Coord c(1, 1);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::E);

    CHECK_FALSE(t.brindilleEnFace(g));
    g.poseBrindille(Coord(1, 2));
    CHECK(t.brindilleEnFace(g));
}


TEST_CASE("Termite: voisinsLibres") {
    Grille g;
    Coord c(5, 5); // On prend une case qui est pas trop dans une extremité de la grille .
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::N);

    CHECK(t.voisinsLibres(g) == 8);

    g.poseBrindille(Coord(4, 5));
    CHECK(t.voisinsLibres(g) == 7);

    g.poseBrindille(Coord(5, 6));
    CHECK(t.voisinsLibres(g) == 6);
}


TEST_CASE("Termite: avanceTermite") {
    Grille g;
    Coord c(5, 5);
    g.poseTermite(c, 0);
    Termite t(0, c, Direction::E);

    t.avanceTermite(g); // on regarde ensuite si cela a bien avancé
    CHECK(t.getPosition() == Coord(5, 6));
    CHECK(g.numeroTermite(Coord(5, 6)) == 0);
    CHECK(g.estVide(Coord(5, 5)));
}


// on fait le test charge et decharge brindille ensemble car c'est plus simple et c'est lié.
TEST_CASE("Termite: chargeBrindille et dechargeBrindille") {
    Grille g;
    Coord c(5, 5);
    Coord devantC(5, 6);
    g.poseTermite(c, 0);
    g.poseBrindille(devantC);// on met une brindille devant le termite pour que le termite puisse le prendre.

    Termite t(0, c, Direction::E);
    CHECK_FALSE(t.estPorteBrindille());
    CHECK(g.contientBrindille(devantC));

    t.chargeBrindille(g); // Termite prend la brindille qui est devant lui
    CHECK(t.estPorteBrindille());
    CHECK_FALSE(g.contientBrindille(devantC));

    t.dechargeBrindille(g); // et il le pose devant lui.
    CHECK_FALSE(t.estPorteBrindille());
    CHECK(g.contientBrindille(devantC));
}


// Et pareil avec la methode marche aleatoire qui est compliquée à tester donc on decide de ne pas le tester.