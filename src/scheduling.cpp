#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>

using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload) {
  list<Process> complete;
  return complete;
}

list<Process> sjf(pqueue_arrival workload) {
  list<Process> complete;
  return complete;
}

list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  return complete;
}

float avg_turnaround(list<Process> processes) {
  return 0;
}

float avg_response(list<Process> processes) {
  return 0;
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}
