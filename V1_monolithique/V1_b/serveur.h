#pragma once
#define SERVEUR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JEUX 10
#define MAX_CHILDREN 10

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

extern Jeu jeux[MAX_JEUX];
extern pthread_mutex_t mutex_jeux;

int operation_tester(DemandeOperation *demande);
int operation_afficher();
int operation_ajouter(DemandeOperation *demande);
int operation_supprimer(DemandeOperation *demande);
void operation_simuler(DemandeOperation *demande);
void operation_lancer(DemandeOperation *demande);
void* execute_demande_parallel_thread(void *arg);
void execute_demande_parallel(DemandeOperation OP, pthread_t *PIDf, int *nbfils_non_bloquants);
void clean_threads(pthread_t *PIDf, int *nbfils_non_bloquants);