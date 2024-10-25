#ifndef JOUEUR_H
#define JOUEUR_H

struct j {
    int cheval1;
    int cheval2;
};
typedef struct j Joueur;


// Joueur tab_joueur[4];

// Trouve le cheval le plus proche de l'arrivée
int trouve_plus_proche(int cheval1, int cheval2);

//fait avancer le cheval de nombre de dé case
void avance_normal(int des, int chevalNum, Joueur *j);

//fait avancer le cheval mais verifie si les regles du jeu sont bien respectés
void avance(int des, int chevalNum, Joueur *j);

/*la fonction fait monter le cheval dans l'escalier  en route,vers l'arrivé ou alors fait avancer l'autre cheval plus loin*/
void escalier(int des, int chevalNum, Joueur *j);

// Faire un tour
void tour(Joueur* tab_j, Joueur *j);

// renvoie 1 si 1 des tous cheveaux a attendu la case 63
int gagne(Joueur *tab_j);

#endif
