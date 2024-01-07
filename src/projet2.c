// Author: Jonny SAYKOSY
// CNAM 2023-2024

#include "biblio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MESSAGE_LENGTH 1024

int child_process()
{
    pid_t pid = getpid();
    char message[MESSAGE_LENGTH];
    memset(&message, 0, sizeof(message));

    printf("%d : %s\n", pid, "Je suis le fils");
    printf("%d : Veuillez saisir un message: ", pid);
    fgets(message, sizeof(message), stdin);

    // Removes trailing newline
    if ((strlen(message) > 0) && (message[strlen(message) - 1] == '\n'))
        message[strlen(message) - 1] = '\0';

    emission("127.0.0.1", 8000, message);
    printf("%d : %s\n", pid, "Fin du processus fils...");
    return 0;
}

int main()
{
    pid_t child_pid = fork();
    pid_t pid = getpid();

    if (child_pid < 0) {
        perror("Failed to create process");
        exit(1);
    }

    if (child_pid == 0) {
        child_process();
    } else {
        printf("%d : %s\n", pid, "Je suis le père");

        char buffer[MESSAGE_LENGTH];
        memset(&buffer, 0, sizeof(buffer));
        reception(8000, buffer);

        // Wait for child process to finish
        wait(NULL);

        printf("%d : Message reçu : %s\n", pid, buffer);
        printf("%d : %s\n", pid, "Fin du processus père...");
        return 0;
    }
}
