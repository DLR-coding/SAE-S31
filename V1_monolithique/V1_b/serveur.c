#include "serveur.h"

Jeu jeux[MAX_JEUX];
pthread_mutex_t mutex_jeux = PTHREAD_MUTEX_INITIALIZER; // Mutex pour protéger les accès concurrents au tableau `jeux`.

int operation_tester(DemandeOperation *demande) {
    printf("Testing if game '%s' exists...\n", demande->NomJeu);
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    for (int i = 0; i < MAX_JEUX; i++) {
        printf("Checking if game '%s' is at index %d...\n", demande->NomJeu, i);
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            printf("Game '%s' found at index %d.\n", demande->NomJeu, i);
            pthread_mutex_unlock(&mutex_jeux);
            printf("Mutex unlocked\n");
            return 0; // Jeu trouvé
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    printf("Game '%s' not found.\n", demande->NomJeu);
    return -1; // Jeu non trouvé
}

int operation_afficher() {
    printf("Listing all games...\n");
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] != '\0') {
            printf("Game %d: %s\n", i, jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    return nombre_de_jeux;
}

int operation_ajouter(DemandeOperation *demande) {
    printf("Adding game '%s'...\n", demande->NomJeu);
    char *code = malloc(1000 * sizeof(char));
    if (code == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int j = 0; j < 1000; j++) {
        code[j] = '*';
    }
    sleep(1);
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] == '\0') {
            strcpy(jeux[i].NomJeu, demande->NomJeu);
            jeux[i].Code = code;
            printf("Game '%s' added at index %d.\n", demande->NomJeu, i);
            pthread_mutex_unlock(&mutex_jeux);
            printf("Mutex unlocked\n");
            return 0;
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    printf("Game '%s' could not be added.\n", demande->NomJeu);
    return -1; // Tableau plein
}

int operation_supprimer(DemandeOperation *demande) {
    printf("Removing game '%s'...\n", demande->NomJeu);
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            printf("Game '%s' removed at index %d.\n", demande->NomJeu, i);
            pthread_mutex_unlock(&mutex_jeux);
            printf("Mutex unlocked\n");
            return 0;
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    printf("Game '%s' not found for removal.\n", demande->NomJeu);
    return -1; // Jeu non trouvé
}

void operation_simuler(DemandeOperation *demande) {
    printf("Simulating game '%s'...\n", demande->NomJeu);
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            int gagnant = rand() % 2;
            sleep(20);
            if (gagnant == 0) {
                printf("Joueur 1 gagne !\n");
            } else {
                printf("Joueur 2 gagne !\n");
            }
            pthread_mutex_unlock(&mutex_jeux);
            printf("Mutex unlocked\n");
            return;
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    printf("Game '%s' not found for simulation.\n", demande->NomJeu);
}

void operation_lancer(DemandeOperation *demande) {
    printf("Launching game '%s'...\n", demande->NomJeu);
    pthread_mutex_lock(&mutex_jeux);
    printf("Mutex locked\n");
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            printf("Press ENTER to play...\n");
            getchar(); // attend le Enter du standardInput
            int gagnant = rand() % 2;
            if (gagnant == 0) {
                printf("Serveur gagne !\n");
            } else {
                printf("Joueur gagne !\n");
            }
            pthread_mutex_unlock(&mutex_jeux);
            printf("Mutex unlocked\n");
            return;
        }
    }
    pthread_mutex_unlock(&mutex_jeux);
    printf("Mutex unlocked\n");
    printf("Game '%s' not found for launch.\n", demande->NomJeu);
}

void* execute_demande_parallel_thread(void *arg) {
    DemandeOperation *OP = (DemandeOperation*)arg;
    printf("Thread (PID %lu) executing request with CodeOp: %d\n", pthread_self(), OP->CodeOp);

    if (OP->CodeOp == 1) {
        printf("Executing operation_tester...\n");
        operation_tester(OP);
    } else if (OP->CodeOp == 2) {
        printf("Executing operation_afficher...\n");
        operation_afficher();
    } else if (OP->CodeOp == 3) {
        printf("Executing operation_ajouter...\n");
        operation_ajouter(OP);
    } else if (OP->CodeOp == 4) {
        printf("Executing operation_supprimer...\n");
        operation_supprimer(OP);
    } else if (OP->CodeOp == 5) {
        printf("Executing operation_simuler...\n");
        operation_simuler(OP);
    } else if (OP->CodeOp == 6) {
        printf("Executing operation_lancer...\n");
        operation_lancer(OP);
    } else {
        printf("Unknown operation\n");
    }
    printf("Thread (PID %lu) completed execution.\n", pthread_self());
    pthread_exit(NULL);
}

void execute_demande_parallel(DemandeOperation OP, pthread_t *PIDf, int *nbfils_non_bloquants) {
    pthread_t thread;
    printf("Creating thread for operation with CodeOp: %d\n", OP.CodeOp);
    pthread_create(&thread, NULL, execute_demande_parallel_thread, &OP);
    if (OP.flag == 1) {
        printf("Blocking operation by thread (PID %lu),  CodeOp: %d.\n", pthread_self() , OP.CodeOp);
        pthread_join(thread, NULL);
        printf("Blocking operation completed by thread (PID %lu) , CodeOp: %d .\n", pthread_self() , OP.CodeOp);
    } else {
        printf("Starting non-blocking operation  CodeOp: %d ,  by thread (PID %lu).\n", OP.CodeOp, pthread_self());
        PIDf[*nbfils_non_bloquants] = thread;
        (*nbfils_non_bloquants)++;
    }
}


void clean_threads(pthread_t *PIDf, int *nbfils_non_bloquants) {
    printf("Cleaning up non-blocking threads...\n");
    for (int i = 0; i < *nbfils_non_bloquants; i++) {
        if (PIDf[i] != 0) {
            pthread_join(PIDf[i], NULL); // Attendre la fin du thread
            printf("Thread (PID %lu) cleaned up.\n", PIDf[i]);
            PIDf[i] = 0; // Réinitialiser l'entrée
        }
    }
    *nbfils_non_bloquants = 0; // Réinitialiser le compteur de threads
    printf("All non-blocking threads cleaned up.\n");
}

