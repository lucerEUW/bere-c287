#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur zur Modellierung eines Prozesses
typedef struct {
    int processID;
    int arrivalTime;
    int burstTime;
    int deadline; // Für EDF
    int waitingTime;
    int turnaroundTime;
    int isDeadlineMet; // 1 = Deadline eingehalten, 0 = Deadline verfehlt
} Process;

// Vergleichsfunktionen für Sortierung
int compareArrivalTime(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    return p1->arrivalTime - p2->arrivalTime;
}

int compareDeadline(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    return p1->deadline - p2->deadline;
}

// Funktion zum Berechnen von Warte-, Turnaround-Zeit und Deadline-Überprüfung
void calculateTimes(Process processes[], int n) {
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        // Wartezeit ist die Differenz zwischen der aktuellen Zeit und der Ankunftszeit
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;

        // Turnaround-Zeit ist Wartezeit plus Burst-Zeit
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;

        // Update der aktuellen Zeit
        currentTime += processes[i].burstTime;

        // Überprüfung, ob die Deadline eingehalten wurde
        if (currentTime <= processes[i].deadline) {
            processes[i].isDeadlineMet = 1; // Deadline eingehalten
        } else {
            processes[i].isDeadlineMet = 0; // Deadline verfehlt
        }
    }
}

// Funktion zur Darstellung der Ergebnisse
void displayProcesses(Process processes[], int n) {
    printf("\nProcess\tArrival\tBurst\tDeadline\tWaiting\tTurnaround\tDeadline Met?\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%s\n",
               processes[i].processID,
               processes[i].arrivalTime,
               processes[i].burstTime,
               processes[i].deadline,
               processes[i].waitingTime,
               processes[i].turnaroundTime,
               processes[i].isDeadlineMet ? "Yes" : "No");
    }
}

int main() {
    int n;
    char schedulerType[10];

    // Eingabe der Anzahl der Prozesse
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[n];

    // Eingabe der Prozessinformationen
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time, burst time, and deadline for process P%d: ", i + 1);
        processes[i].processID = i + 1;
        scanf("%d %d %d", &processes[i].arrivalTime, &processes[i].burstTime, &processes[i].deadline);
    }

    // Auswahl der Scheduling-Strategie
    printf("Choose scheduler (FCFS/EDF): ");
    scanf("%s", schedulerType);

    if (strcmp(schedulerType, "FCFS") == 0) {
        // Prozesse nach Ankunftszeit sortieren
        qsort(processes, n, sizeof(Process), compareArrivalTime);
    } else if (strcmp(schedulerType, "EDF") == 0) {
        // Prozesse nach Deadline sortieren
        qsort(processes, n, sizeof(Process), compareDeadline);
    } else {
        printf("Invalid scheduler type. Exiting...\n");
        return 1;
    }

    // Berechnung der Warte-, Turnaround-Zeiten und Deadline-Überprüfung
    calculateTimes(processes, n);

    // Ergebnisse anzeigen
    displayProcesses(processes, n);

    return 0;
}