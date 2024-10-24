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

//fait avancer le joueur de nombre de des (surtout utile pour la fonction escalier)
void avance_normal(int des, int chevalNum, Joueur *j);

//fait avancer mais verifie si respecte les regle du jeu avant d'acceder a l'escalier
void avance(int des, int chevalNum, Joueur *j);

/*la fonction fait monter le cheval pres de l'arrivé ou alors fait avancer l'autre cheval plus loin*/
void escalier(int des, int chevalNum, Joueur *j);

// Faire un tour
void tour(Joueur *j);

// renvoie 1 si 1 des tous cheveaux a attendu la case 65
int gagne(Joueur *tab_j);

#endif
