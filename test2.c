#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include "joueur.h"


/*  */
void tache_pere(int tp[4][2], int t_spe[2]) {
    // Close reading ends in tp and special pipe writing end
    for (int j = 0; j < 4; j++){
        close(tp[j][0]); // close reading ends for pipes
    }
    close(t_spe[1]); // close writing end of special pipe

    int i = 0;
    int rep;

    Joueur tab_joueur[4];
    // initialiser tous chevaux a 0
    memset(tab_joueur, 0, sizeof(tab_joueur));

    while (i < 4 && gagne(tab_joueur) != 1) { 
        // Inform all children about the current player

        printf("P envoie le numero de joueur : %d\n", i);
        for (int j = 0; j < 4; j++){
            write(tp[j][1], &i, sizeof(i));  // Send the player index to all children
        }

        // Read result from the special pipe
        read(t_spe[0], &tab_joueur, sizeof(tab_joueur));
        printf("P a recu l'etat du jeu (par t_spe)\n");
        printf("\n-------------------------- L'ÉTAT DU JEU -------------------------- \n");
        printf("JOUEUR 0 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[0].cheval1, tab_joueur[0].cheval2);
        printf("JOUEUR 1 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[1].cheval1, tab_joueur[1].cheval2);
        printf("JOUEUR 2 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[2].cheval1, tab_joueur[2].cheval2);
        printf("JOUEUR 3 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[3].cheval1, tab_joueur[3].cheval2);
        printf("----------------------------------------------------------------------\n");

        i++;  // Move to the next round
        // revenir au debut vers le tour de 1er joueur si le jeu n'est pas fini
        if (i == 4 && gagne(tab_joueur) != 1) i = 0;
    }

    printf("\n**************************** FIN DU JEU ****************************\n");
    printf("JOUEUR 0 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[0].cheval1, tab_joueur[0].cheval2);
    printf("JOUEUR 1 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[1].cheval1, tab_joueur[1].cheval2);
    printf("JOUEUR 2 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[2].cheval1, tab_joueur[2].cheval2);
    printf("JOUEUR 3 : \n\t cheval 1 : %d,  cheval 2 : %d\n", tab_joueur[3].cheval1, tab_joueur[3].cheval2);
    printf("*********************************************************************\n");

    i = -1;
    for (int j = 0; j < 4; j++){
            write(tp[j][1], &i, sizeof(i));  // -1 pour les joueurs pour signaler la fin du jeu
        }

    // Close pipes after finishing the communication
    close(t_spe[0]);
    for (int j = 0; j < 4; j++) {
        close(tp[j][1]);
    }

    // Wait for children to finish
    for (int j = 0; j < 4; j++) {
        wait(NULL);
    }
}


int getIDVoisinDroit(int id){
    if (id !=3 ) return id+1;
    else return 0;
}

int getIDVoisinGauche(int id){
    if (id !=0 ) return id-1;
    else return 3;
}

/* 
    tab_pid - tableau de tous les pids 
    tpi - tubes de pere a fils
    tt - tubes pour l'anneau
    t_spe - tube de fils a pere
*/
void tache_joueur(int tpi[4][2], int tt[4][2], int t_spe[2], int id_actuel) {
    srand(time(NULL) + id_actuel);
    
    // int x;
    int id_joueur;
    int id_voisin_droit = getIDVoisinDroit(id_actuel);
    int id_voisin_gauche = getIDVoisinGauche(id_actuel);

    Joueur tab_joueur[4];
    // initialiser tous chevaux a 0
    memset(tab_joueur, 0, sizeof(tab_joueur));

    while(1) {
        close(tpi[id_actuel][1]);  // Close writing end to parent
        close(t_spe[0]);  // Close reading end of special pipe

        // Read the index of the player from the parent
        read(tpi[id_actuel][0], &id_joueur, sizeof(id_joueur));
        // printf("(id_actuel = %d) Le joueur %d joue (d'apres la lecture par pipe de pere)\n", id_actuel, id_joueur);

        if (id_joueur == -1) {
            // End of game, close pipes and break
            break;
        }

        if (id_actuel == id_joueur) {
            // If this process is the player, it writes the game state to its right neighbor

            // fair un tour seulement si personne a gagné (aucun cheval n'est pas à la case 65)
            if (gagne(tab_joueur) != 1) {
                printf("\n ******************** TOUR de J%d******************** \n", id_actuel);
                tour(&tab_joueur[id_actuel]); //& ???
                printf("\n******************************************************\n");
            }
            
            printf("F (joueur) envoie l'etat du jeu a son voisin droit [%d]: cheval 1 : %d, cheval 2 : %d\n", 
                    id_voisin_droit, tab_joueur[id_actuel].cheval1,tab_joueur[id_actuel].cheval2 );
            write(tt[id_actuel][1], &tab_joueur, sizeof(tab_joueur));

        } else {
            // Read the state from the left neighbor and forward it

            read(tt[id_voisin_gauche][0], &tab_joueur, sizeof(tab_joueur));
            printf("F [%d] lit l'etat du jeu de son voisin gauche : %d, %d\n", 
                    id_actuel, tab_joueur[id_joueur].cheval1,tab_joueur[id_joueur].cheval2);
            
            if (id_voisin_droit == id_joueur) {
                // If the right neighbor is the player, send the state to the parent
                printf("F (dernier) envoie l'etat du jeu a pere : %d, %d\n", 
                        tab_joueur[id_joueur].cheval1,tab_joueur[id_joueur].cheval2);
                write(t_spe[1], &tab_joueur, sizeof(tab_joueur));
            } else {
                // Otherwise, pass the state to the right neighbor
                printf("F envoie l'etat du jeu a son voisin droit [%d] : %d, %d\n", 
                        id_voisin_droit, tab_joueur[id_joueur].cheval1,tab_joueur[id_joueur].cheval2);
                write(tt[id_actuel][1], &tab_joueur, sizeof(tab_joueur));
            }
        }
    }

    // Close all pipes after the game ends
    close(tt[id_voisin_gauche][0]);
    close(tt[id_actuel][1]);
    close(tpi[id_actuel][0]);
    close(t_spe[1]);
}


int main() {

    

    int tt[4][2];
    int tpi[4][2];
    int t_spe[2];

    // Create pipes for communication
    for (int i = 0; i < 4; i++) {
        if (pipe(tt[i]) == -1 || pipe(tpi[i]) == -1) {
            perror("pipe creation failed");
            exit(-1);
        }
    }
    if (pipe(t_spe) == -1) {
        perror("special pipe creation failed");
        exit(-1);
    }

    int tab_pid[4];
    for (int i = 0; i < 4; i++) {
        tab_pid[i] = fork();
        // initialiser tab_joueurs
        if (tab_pid[i] == -1) {
            perror("fork failed");
            exit(-1);
        } else if (tab_pid[i] == 0) {
            // Child process - handle player's task
            tache_joueur(tpi, tt, t_spe, i);
            exit(0);
        }
    }

    // Parent process - manage the game
    tache_pere(tpi, t_spe);

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}
