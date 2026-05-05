# Simulation de colonies de termites

## Description

Ce projet simule un comportement émergent de stigmergie : des termites se déplacent aléatoirement sur une grille et rassemblent progressivement des brindilles en un seul tas, sans aucune coordination explicite entre eux. Chaque termite suit des règles très simples, et la complexité naît de l'interaction du grand nombre.

## Fichiers

```
coord.hpp / coord.cpp     — Type Coord (coordonnées) et enum Direction (8 points cardinaux)
grille.hpp / grille.cpp   — Type Grille (tableau 2D de cases)
termite.hpp / termite.cpp — Type Termite et algorithme de comportement
projet.cpp                — Simulation principale et main
test.cpp                  — Point d'entrée pour les tests doctest
Makefile                  — Règles de compilation
doctest.h                 — Infrastructure de tests (à copier dans le répertoire)
```

## Compilation et exécution

### Prérequis

`clang++` doit être installé, et `doctest.h` doit être présent dans le répertoire du projet.

### Commandes

```bash
# Compiler tout (simulation + tests)
make

# Lancer les tests
make check

# Lancer les tests avec affichage des tests réussis
./tests -s

# Lancer la simulation
./projet

# Nettoyer les fichiers compilés
make clean
```

## Utilisation de la simulation

Au lancement, le programme affiche une grille 20×20 initialisée aléatoirement avec des brindilles (`*`) et des termites représentés par leur direction (`|`, `-`, `/`, `\`). Un termite portant une brindille s'affiche `X`.

### Commandes interactives

| Touche | Effet |
|--------|-------|
| `Entrée` | Effectue N passes (N = 10 par défaut) |
| `*` | Double le nombre de passes |
| `/` | Divise le nombre de passes par 2 |
| `.` | Quitte la simulation |

## Comportement des termites

À chaque étape, chaque termite suit une machine à états pilotée par `vieTermite`. Le comportement se résume ainsi. Quand un termite ne porte pas de brindille, il marche aléatoirement jusqu'à trouver une brindille en face de lui, il la ramasse, puis le sablier est initialisé à `dureeSablier = 6` déplacements. Pendant ces 6 déplacements il continue de marcher sans pouvoir déposer. Une fois le sablier écoulé, dès qu'il voit une brindille en face (un tas), il bascule en mode "tourne sur place" et pivote d'un huitième de tour à la fois jusqu'à trouver une case libre non enclavée (au moins 2 voisins libres) où déposer sa brindille à côté du tas. Après avoir déposé, le sablier repart pour éviter qu'il reprenne immédiatement la brindille qu'il vient de poser.

### Paramètres (dans `termite.hpp`)

```cpp
const float probaTourner     = 0.1;   // probabilité de tourner quand la voie est libre
const int   dureeSablier     = 6;     // délai minimum entre ramasser et poser (et vice-versa)
const float densiteBrindille = 0.05;  // densité initiale de brindilles sur la grille
const int   nbTermites       = 20;    // nombre de termites
```

## Architecture du code

### `Coord`

Paire (ligne, colonne) validée à la construction : une `runtime_error` est levée si les coordonnées sont hors de la grille (`tailleGrille = 20`). La classe expose `getLig()`, `getCol()`, ainsi que les opérateurs `==` et `!=`.

### `Direction`

Énumération des 8 points cardinaux (N, NE, E, SE, S, SW, W, NW). Les fonctions `aGauche` et `aDroite` permettent de tourner d'un huitième de tour. La fonction `devantCoord` calcule la case située dans la direction donnée depuis une coordonnée et propage la `runtime_error` du constructeur de `Coord` lorsque la case calculée sort des bornes de la grille. La fonction `dirAleatoire` renvoie une direction tirée au hasard parmi les huit.

### `Grille`

Tableau 2D de cases représenté par la structure interne `Case` (attributs `bool brindille` et `int termite`, initialisés à `false` et `-1`). Chaque méthode de mutation vérifie sa précondition et lève une `invalid_argument` en cas de violation, par exemple poser une brindille sur une case déjà occupée ou enlever un termite d'une case qui n'en contient pas.

### `Termite`

La classe encapsule les attributs `id`, `position`, `dir`, `porteBrindille`, `sablier` et `tourneSurPlace`. La méthode principale est `vieTermite(Grille&)`, qui orchestre l'ensemble du comportement décrit ci-dessus. Les méthodes auxiliaires `laVoieEstLibre`, `brindilleEnFace` et `voisinsLibres` interrogent la grille sans la modifier et gèrent silencieusement les débordements de bords en attrapant les `runtime_error` de `Coord`.

### Cohérence grille et termites

L'information sur la position des termites est stockée en double : dans le vecteur de `Termite` et dans la `Grille`. La fonction `verifieCoherence` parcourt les deux structures après chaque série de passes et lève une `runtime_error` si une incohérence est détectée, ce qui permet de détecter toute désynchronisation entre les deux représentations.

## Tests

Les tests sont écrits avec [doctest](https://github.com/doctest/doctest) et couvrent les modules suivants.

Pour `Coord`, on teste le constructeur, les cas limites, le rejet des coordonnées hors grille, l'égalité et l'affichage. Pour `Direction`, on vérifie que `aGauche` et `aDroite` sont bien inverses l'une de l'autre, que 8 rotations successives dans chaque sens reviennent à la direction initiale, et que `devantCoord` produit les bons résultats pour les 8 directions ainsi qu'aux bords. Pour `Grille`, toutes les méthodes sont testées, y compris les préconditions qui lèvent des exceptions. Pour `Termite`, on couvre le constructeur, `toString` (y compris le cas porteur de brindille qui affiche `X`), les rotations `tourneADroite` et `tourneAGauche`, `laVoieEstLibre`, `brindilleEnFace`, `voisinsLibres`, `avanceTermite`, ainsi que le couple `chargeBrindille` et `dechargeBrindille`.
