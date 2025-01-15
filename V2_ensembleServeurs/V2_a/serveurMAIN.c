#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_operation(const char *operation, char *args[], int pipe_fd[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Processus enfant : lire le nom du jeu du pipe et exécuter l'opération
        close(pipe_fd[1]);  // Fermer l'écriture dans le pipe pour l'enfant
        char game_name[100];
        read(pipe_fd[0], game_name, sizeof(game_name));  // Lire le nom du jeu
        close(pipe_fd[0]);  // Fermer la lecture dans le pipe pour l'enfant

        // Transmettre à execv le nom du jeu comme argument
        args[1] = game_name;
        execv(operation, args);  // Exécuter l'opération (tester)
        perror("execv failed");
        exit(1);
    } else {
        // Processus parent : écrire dans le pipe
        close(pipe_fd[0]);  // Fermer la lecture du pipe pour le parent
        char *game_name = "Echec";  // Exemple de nom de jeu
        write(pipe_fd[1], game_name, strlen(game_name) + 1);  // Envoyer le nom du jeu
        close(pipe_fd[1]);  // Fermer l'écriture dans le pipe pour le parent

        int status;
        waitpid(pid, &status, 0);  // Attendre que l'enfant se termine
        if (WIFEXITED(status)) {
            printf("Operation %s exited with code %d\n", operation, WEXITSTATUS(status));
        } else {
            printf("Operation %s did not exit successfully\n", operation);
        }
    }
}

int main() {
    int pipe_fd[2];
    pipe(pipe_fd);

    printf("Checking if the game 'Echec' exists in the database...\n");

    // Arguments pour l'exécution de "tester"
    char *args[] = {"./tester", NULL};  // Le jeu sera passé comme argument dans le pipe

    // Exécuter l'opération de vérification
    execute_operation("./tester", args, pipe_fd);

    return 0;
}
