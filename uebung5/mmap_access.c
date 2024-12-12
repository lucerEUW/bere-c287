#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
int main() {
  // Größe des Speicherbereichs (eine Seite)
  size_t groesse = 4096; // 4 KB
  // Zuordnung eines Speicherbereichs mit nur Lesezugriff
  char *readonly_mem = mmap(NULL, groesse, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (readonly_mem == MAP_FAILED) {
    perror("Fehler bei mmap");
    exit(EXIT_FAILURE);
  }
  // Versuch, in den nur lesbaren Speicherbereich zu schreiben
  printf("Versuche, in einen nur lesbaren Speicherbereich zu schreiben...\n");
  readonly_mem[0] = 'A'; // Dies sollte einen Segmentation Fault verursachen
  // Wenn das Programm bis hierhin kommt, hat der Schreibschutz nicht
  // funktioniert
  printf("Schreibzugriff erfolgreich (unerwartet).\n");
  // Aufräumen
  if (munmap(readonly_mem, groesse) == -1) {
    perror("Fehler bei munmap");
  }
  return 0;
}
