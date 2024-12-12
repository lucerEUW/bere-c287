#include <stdio.h>
#include <stdlib.h>

// Struktur zur Modellierung eines Prozesses
typedef struct {
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;       // Priorität (kleinerer Wert = höhere Priorität)
    int waitingTime;
    int turnaroundTime;
    int completionTime;
} Process;

// Vergleichsfunktion für Prioritäts- und Ankunftszeitsortierung
int comparePriorityAndArrivalTime(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    if (p1->priority == p2->priority) {
        return p1->arrivalTime - p2->arrivalTime;
    }
    return p1->priority - p2->priority;
}

// Funktion zur Berechnung der Warte- und Turnaround-Zeiten im Round-Robin-Scheduler mit Prioritäten
void calculateTimes(Process processes[], int n, int timeQuantum) {
    int currentTime = 0;
    int completed = 0;
    int queue[n];       // Warteschlange
    int front = 0, rear = 0;
    int inQueue[n];     // Hilfsarray zur Überprüfung, ob ein Prozess in der Warteschlange ist

    // Initialisiere Warteschlange und Restzeiten
    for (int i = 0; i < n; i++) {
        processes[i].remainingTime = processes[i].burstTime;
        inQueue[i] = 0;
    }

    // Sortiere Prozesse nach Priorität und Ankunftszeit
    qsort(processes, n, sizeof(Process), comparePriorityAndArrivalTime);

    // Füge Prozesse mit Priorität, die bei t=0 verfügbar sind, in die Warteschlange ein
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime <= currentTime) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    // Round-Robin-Scheduling
    while (completed < n) {
        if (front == rear) {
            currentTime++; // Leerlaufzeit, wenn keine Prozesse verfügbar sind
            // Neue Prozesse prüfen
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime <= currentTime && !inQueue[i]) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }
            continue;
        }

        // Hole den nächsten Prozess aus der Warteschlange
        int i = queue[front++ % n];

        // Berechne, wie lange der Prozess ausgeführt wird
        int timeSpent = (processes[i].remainingTime > timeQuantum) ? timeQuantum : processes[i].remainingTime;
        processes[i].remainingTime -= timeSpent;
        currentTime += timeSpent;

        // Füge neue Prozesse in die Warteschlange ein, die während der Ausführung ankommen
        for (int j = 0; j < n; j++) {
            if (processes[j].arrivalTime <= currentTime && processes[j].remainingTime > 0 && !inQueue[j]) {
                queue[rear++] = j;
                inQueue[j] = 1;
            }
        }

        // Wenn der Prozess abgeschlossen ist
        if (processes[i].remainingTime == 0) {
            processes[i].completionTime = currentTime;
            processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
            completed++;
            inQueue[i] = 0; // Prozess ist abgeschlossen
        } else {
            // Füge den Prozess wieder ans Ende der Warteschlange ein
            queue[rear++ % n] = i;
        }
    }
}

// Funktion zur Darstellung der Ergebnisse
void displayProcesses(Process processes[], int n) {
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\tCompletion Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].processID,
               processes[i].arrivalTime,
               processes[i].burstTime,
               processes[i].priority,
               processes[i].waitingTime,
               processes[i].turnaroundTime,
               processes[i].completionTime);
    }
}

int main() {
    int n, timeQuantum;

    // Eingabe der Anzahl der Prozesse
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[n];

    // Eingabe der Prozessinformationen
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time, burst time, and priority for process P%d: ", i + 1);
        processes[i].processID = i + 1;
        scanf("%d %d %d", &processes[i].arrivalTime, &processes[i].burstTime, &processes[i].priority);
    }

    // Eingabe des Time Quantums
    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    // Berechnung der Warte- und Turnaround-Zeiten
    calculateTimes(processes, n, timeQuantum);

    // Ergebnisse anzeigen
    displayProcesses(processes, n);

    return 0;
}