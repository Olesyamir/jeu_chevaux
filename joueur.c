#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "joueur.h"

// Trouve le cheval le plus proche de l'arrivée
int trouve_plus_proche(int cheval1, int cheval2) {
    if (cheval1 > cheval2) {
        return cheval1;
    } else {
        return cheval2;
    }
}

//fait avancer le joueur de nombre de dé en avant
void avance_normal(int des, int chevalNum, Joueur *j) {
    if (chevalNum == 1) { // Si c'est le cheval 1
        j->cheval1 += des;
        printf("Le cheval 1 s'est bien déplacé de %d cases, et se trouve maintenant dans la case %d\n", des, j->cheval1);
    } else if (chevalNum == 2) { // Si c'est le cheval 2
        j->cheval2 += des;
        printf("Le cheval 2 s'est bien déplacé de %d cases, et se trouve maintenant dans la case %d\n", des, j->cheval2);
    } else {
        printf("Erreur : numéro de cheval invalide\n");
    }
}


//fait avancer le cheval mais verifie si les regles du jeu sont bien respectés
void avance(int des, int chevalNum, Joueur *j) {
    int cheval;
    int autrecheval;
    int num_autre_cheval;

    // Déterminer la position actuelle du cheval et de l'autre cheval
    if (chevalNum == 1) {
        cheval = j->cheval1;
        autrecheval = j->cheval2;
        num_autre_cheval = 2;
    } else if (chevalNum == 2) {
        cheval = j->cheval2;
        autrecheval = j->cheval1;
        num_autre_cheval = 1;
    } else {
        printf("Erreur : numéro de cheval invalide\n");
        return; // Quitter la fonction si le numéro de cheval est invalide
    }

    
    //le cheval arrive a laporte de l'escalier et n'a pas le bon nombre de dé necessaire, donc on fait deplacer l'autre cheval tant qu' il peut 
    if (cheval + des > 56 &&  autrecheval + des < 56) {
        printf("Le cheval %d ne peut pas depasser la case 56, on va faire avancer l'autre cheval ", chevalNum);
        avance_normal(des, num_autre_cheval, j);

    //verifie si les 2 chevaux vont tenter de s'arreter a la porte de l'escalier , alors celui qui se deplace doit faire le bon  nombre sinon recule
    } else if(cheval + des > 56 &&  autrecheval + des > 56){
        // avancer et reculer de cases en trop
            
            if (chevalNum == 1) {
                int cases_a_avancer = 56 - cheval;// avancer jusqu'a 56
                int case_a_reculer = des - cases_a_avancer;// reculer de case en trop
                j->cheval1 += cases_a_avancer;
                j->cheval1 -= case_a_reculer;
                printf("Le cheval 1 s'est bien avancé de %d et a reculer de %d cases, et se trouve maintenant dans la case %d\n", cases_a_avancer,case_a_reculer, j->cheval1);
            } else {
                int cases_a_avancer = 56 - autrecheval;// avancer jusqu'a 56
                int case_a_reculer=des-cases_a_avancer;// reculer de case en trop
                j->cheval2 += cases_a_avancer;
                j->cheval2 -= case_a_reculer;
                printf("Le cheval 1 s'est bien avancé de %d et a reculer de %d cases, et se trouve maintenant dans la case %d\n", cases_a_avancer,case_a_reculer, j->cheval2);
            }

    }else{
        // Avance normalement si c'est dans les limites
        avance_normal(des, chevalNum, j);
    }
   
}



/*la fonction fait monter le cheval dans l'escalier  en route,vers l'arrivé ou alors fait avancer l'autre cheval plus loin*/
void escalier(int des, int chevalNum, Joueur *j) {
    printf("ESCALIER\n");
    int cheval;
    int autrecheval;
    int num_autre_cheval;
    // Déterminer la position actuelle du cheval le plus proche de l'arrivé (cheval) et de l'autre plus loin (autrecheval)
    if (chevalNum == 1) {
        cheval = j->cheval1;
        autrecheval=j->cheval2; 
        num_autre_cheval=2;
    } else if (chevalNum == 2) {
        cheval = j->cheval2;
        autrecheval=j->cheval1; num_autre_cheval=1;
    } else {
        printf("Erreur : numéro de cheval invalide\n");
        return; // Quitter la fonction si le numéro de cheval est invalide
    }


    switch (cheval) { 
        case 56: if (des == 1){avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){ avance(des, num_autre_cheval, j);}}break;

        case 57: if (des == 2) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){ avance(des, num_autre_cheval, j);}}break;

        case 58: if (des == 3) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){avance(des, num_autre_cheval, j);}}break;

        case 59: if (des == 4) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){avance(des, num_autre_cheval, j);}}break;

        case 60: if (des == 5) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){avance(des, num_autre_cheval, j);}}break;

        case 61: if (des == 6) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){avance(des, num_autre_cheval, j);}}break;

        case 62: if (des == 6) {avance(1, chevalNum, j);}  
                    else{if (autrecheval<56){avance(des, num_autre_cheval, j);}}break; 
                    // Après il arrive sur la dernière case 63
        default: printf("Ce n'est pas le bon nombre, tu ne peux pas avancer\n"); break;
    }
}




void tour(Joueur* tab_j, Joueur *j) {
    int cheval1 = j->cheval1;
    int cheval2 = j->cheval2;


    int des = (rand() % 6) + 1; // Générer un nombre aléatoire entre 1 et 6
    printf("\nLe joueur doit déplacer un cheval de %d cases\n", des);
    // j->des = des; // Met à jour le dé

    int cheval;
    printf("chaque cheval se trouve a la position: cheval1: %d, cheval2:%d\n",j->cheval1,j->cheval2);
    printf("j'entre dans les cas \n");
    // Début
    if (cheval1 == 0 && cheval2 == 0) {
        if (des == 6) {
            avance(1, 1, j); // Sortir le cheval 1 de l'écurie
            cheval1 = j->cheval1; // Met à jour la position du cheval 1
        }
    }
    // Escalier
    else if (cheval1 >= 56 || cheval2 >= 56) {
        cheval = trouve_plus_proche(cheval1, cheval2);
        escalier(des, cheval == cheval1 ? 1 : 2, j);
    }
    // Jeu où le cheval est entre la case 1 et 56
    else {
        cheval = trouve_plus_proche(cheval1, cheval2);
        if (des != 6) {
            avance(des, cheval == cheval1 ? 1 : 2, j);
        } else { // des = 6, le joueur peut sortir un cheval ou avancer de 6
            if (cheval1 == 0) {
                avance(1, 1, j); // Sortir le cheval 1
            } else if (cheval2 == 0) {
                avance(1, 2, j); // Sortir le cheval 2
            } else {
                avance(6, cheval == cheval1 ? 1 : 2, j);
            }
        }
    }

    // Vérifie si un cheval a atteint la case 63
    if (j->cheval1 >= 63 || j->cheval2 >= 63) {
        printf("Le joueur a gagné avec un cheval à la case %d !\n", j->cheval1 >= 56 ? j->cheval1 : j->cheval2);

    }

    // Mets à jour les positions
    cheval1 = j->cheval1;
    cheval2 = j->cheval2;

    if (des == 6 && gagne(tab_j)!=1){
        printf("Depuis que j'ai obtenu un 6, j'ai droit au deuxième tour\n");
        tour(tab_j,j);
    }
}

int gagne(Joueur *tab_j){
    for (int i = 0; i < 4; i++) {
        if (tab_j[i].cheval1 == 63 || tab_j[i].cheval2 == 63) {
            printf("LE JOUEUR NUMERO %d A GAGNE LA PARTIE!!!!!!!!!",i);
            return 1; 
        }
    }
    return 0;
}
