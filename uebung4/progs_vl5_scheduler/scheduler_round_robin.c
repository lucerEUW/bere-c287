#include <stdio.h>
#include <stdlib.h>

// Struktur zur Modellierung eines Prozesses
typedef struct {
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingTime; // Für RR: Restliche Burst-Zeit
    int waitingTime;
    int turnaroundTime;
    int completionTime;
} Process;

// Funktion zur Berechnung von Warte- und Turnaround-Zeiten im Round-Robin-Scheduler
void calculateTimes(Process processes[], int n, int timeQuantum) {
    int currentTime = 0;    // Aktuelle Zeit
    int completed = 0;      // Anzahl der abgeschlossenen Prozesse
    int queue[n];           // Warteschlange für Round Robin
    int front = 0, rear = 0;
    int inQueue[n];         // Hilfsarray, um zu prüfen, ob ein Prozess bereits in der Warteschlange ist

    // Initialisiere Restzeit und Warteschlange
    for (int i = 0; i < n; i++) {
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
        inQueue[i] = 0;
    }

    // Füge Prozesse zur Warteschlange hinzu, die bei t=0 verfügbar sind
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime <= currentTime && !inQueue[i]) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    // Round-Robin-Scheduler
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
        int timeSpent = processes[i].remainingTime > timeQuantum ? timeQuantum : processes[i].remainingTime;
        processes[i].remainingTime -= timeSpent;
        currentTime += timeSpent;

        // Füge neue Prozesse in die Warteschlange ein, die während der Ausführung von i ankommen
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
        } else {
            // Füge den Prozess wieder ans Ende der Warteschlange hinzu
            queue[rear++ % n] = i;
        }
    }
}

// Funktion zur Darstellung der Ergebnisse
void displayProcesses(Process processes[], int n) {
    printf("\nProcess\tArrival\tBurst\tWaiting\tTurnaround\tCompletion\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].processID,
               processes[i].arrivalTime,
               processes[i].burstTime,
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
        printf("Enter arrival time and burst time for process P%d: ", i + 1);
        processes[i].processID = i + 1;
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
    }

    // Eingabe des Zeitquants
    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    // Berechnung der Warte- und Turnaround-Zeiten
    calculateTimes(processes, n, timeQuantum);

    // Ergebnisse anzeigen
    displayProcesses(processes, n);

    return 0;
}