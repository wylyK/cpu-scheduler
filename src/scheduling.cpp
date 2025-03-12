#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <string>
#include <limits>

using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  ifstream file(filename);
  int val;
  while (file >> val) {
    Process p;
    p.arrival = val;
    file >> val;
    p.duration = val;
    workload.push(p);
  }
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
  pqueue_arrival xs = workload;
  Process p;
  int curr_t = 0;
  while (!xs.empty()) {
    p = xs.top(); xs.pop();
    curr_t = max(curr_t, p.arrival);
    p.first_run  = curr_t;
    p.completion = curr_t + p.duration;
    curr_t += p.duration;
    p.duration = 0;
    complete.push_back(p);
  }
  return complete; 
}

list<Process> sjf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival xs = workload;
  pqueue_duration arrived;
  Process p;
  int arrival;
  int curr_t = 0;
  while (!xs.empty()) {
    arrival = xs.top().arrival;
    do {
      p = xs.top(); xs.pop();
      arrived.push(p);
    } while (!xs.empty() && xs.top().arrival == arrival);
    curr_t = max(curr_t, arrival);
    while (!arrived.empty()) {
      p = arrived.top(); arrived.pop();
      p.first_run  = curr_t;
      p.completion = curr_t + p.duration;
      curr_t += p.duration;
      p.duration = 0;
      complete.push_back(p);
    }
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival xs = workload;
  pqueue_duration arrived;
  Process p;
  int arrival, next_arrival;
  int curr_t = 0;
  while (!xs.empty()) {
    arrival = xs.top().arrival;
    do {
      p = xs.top(); xs.pop();
      p.first_run = -1;
      arrived.push(p);
    } while (!xs.empty() && xs.top().arrival == arrival);
    curr_t = max(curr_t, arrival);
    next_arrival = xs.empty() ? numeric_limits<int>::max() : xs.top().arrival;
    while (!arrived.empty() && curr_t < next_arrival) {
      p = arrived.top(); arrived.pop();
      if (p.first_run < 0) p.first_run = curr_t;
      int delta_t = next_arrival - curr_t;
      p.completion = curr_t + min(p.duration, delta_t);
      curr_t += min(p.duration, delta_t);
      p.duration -= min(p.duration, delta_t);
      if (p.duration != 0) arrived.push(p);
      else complete.push_back(p);
    }
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  list<Process> arrived;
  pqueue_arrival xs = workload;
  Process p;
  int arrival, next_arrival;
  int curr_t = 0;
  while (!xs.empty()) {
    arrival = xs.top().arrival;
    do {
      p = xs.top(); xs.pop();
      p.first_run = -1;
      arrived.push_back(p);
    } while (!xs.empty() && xs.top().arrival == arrival);
    curr_t = max(curr_t, arrival);
    next_arrival = xs.empty() ? numeric_limits<int>::max() : xs.top().arrival;
    while (!arrived.empty() && curr_t < next_arrival) {
      p = arrived.front(); arrived.pop_front();
      if (p.first_run < 0) p.first_run = curr_t;
      if (--p.duration != 0) arrived.push_back(p);
      else { 
        p.completion = curr_t + 1;
        complete.push_back(p);
      }
      curr_t++;
    }
  }
  return complete;
}

float avg_turnaround(list<Process> processes) {
  float sum_turnaround = 0;
  for (Process p : processes) sum_turnaround += p.completion - p.arrival;
  return sum_turnaround / processes.size();
}

float avg_response(list<Process> processes) {
  float sum_response;
  for (Process p : processes) sum_response += p.first_run - p.arrival;
  return sum_response / processes.size();
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}
