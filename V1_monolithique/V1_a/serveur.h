#pragma once
#define SERVEUR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_JEUX 10
#define PIPE_READ 0
#define PIPE_WRITE 1
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

int operation_tester(DemandeOperation *demande);
int operation_afficher(DemandeOperation *demande);
int operation_ajouter(DemandeOperation *demande, int pipe_fd[2]);
int operation_supprimer(DemandeOperation *demande);
void operation_simuler(DemandeOperation *demande, int pipe_fd[2]);
void operation_lancer(DemandeOperation *demande);
int execute_demande(DemandeOperation OP);
void execute_demande_parallel(DemandeOperation OP, pid_t *PIDf, int *nbfils_non_bloquants);
void wait_for_children(pid_t *PIDf, int nbfils_non_bloquants);

