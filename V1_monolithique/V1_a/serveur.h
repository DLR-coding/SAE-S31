#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_JEUX 10

typedef struct {
    char NomJeu[25];
    char *Code; // NULL si le jeu n'est pas encore téléchargé
} Jeu;

typedef struct {
    int CodeOp; // Code de l’opération : 1 pour tester, 2 pour ajouter, etc.
    char NomJeu[25]; // Nom du jeu sur lequel l'opération est effectuée
    char Param[200]; // Paramètre supplémentaire comme l'adresse de téléchargement
    int flag; // 0 par défaut, utilisé pour des informations supplémentaires
} DemandeOperation;

int operation_tester(DemandeOperation *demande);
int operation_afficher(DemandeOperation *demande);
int operation_ajouter(DemandeOperation *demande);
int operation_supprimer(DemandeOperation *demande);
void operation_simuler(DemandeOperation *demande);
void operation_lancer(DemandeOperation *demande);
int execute_demande(DemandeOperation OP);

#endif // SERVEUR_H
