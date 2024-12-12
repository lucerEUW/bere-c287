#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  // Gemeinsamen Speicher erstellen
  int *shared_mem = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (shared_mem == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  *shared_mem = 0; // Initialisierung
  pid_t pid = fork();
  // Kindprozess
  if (pid == 0) {
    *shared_mem += 5;
    printf("Kindprozess: Wert = %d\n", *shared_mem);
  }
  // Elternprozess
  else if (pid > 0) {
    // wait(NULL); // Auf Kind warten
    *shared_mem += 10;
    printf("Elternprozess: Wert = %d\n", *shared_mem);
  } else {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  // Gemeinsamen Speicher freigeben
  munmap(shared_mem, sizeof(int));
  return 0;
}
