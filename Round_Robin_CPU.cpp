#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    }
};

int main () {
    int time_quantum, num;

    cout << "Enter time quantum: ";
    cin >> time_quantum;
    cout << "\nEnter number of processes: ";
    cin >> num;

    int current_time = 0;
    vector<Process> p(num);
    queue<int> q;

    for (int i = 0; i < num; i++) {
        p[i].id = i + 1;
        cout << "\nEnter arrival time and needed time for Process " << p[i].id << ": ";
        cin >> p[i].arrival_time >> p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
    }

    sort(p.begin(), p.end(), [](const Process& p1, const Process& p2) { return p1.arrival_time < p2.arrival_time; });

    int next_arrival_time = 0;
    int start_id = 0;

    cout << "\n";
    bool is_queue_updated = true;

    while (true) {

        auto check_queue = [&]() {
            for (int i = start_id; i < num; i++) {

                if (p[i].arrival_time <= current_time) {
                    q.push(i);
                    is_queue_updated = true;
                    start_id++;
                } else {
                    next_arrival_time = p[i].arrival_time;
                    break;
                }
            }
        };

        auto print_queue = [&is_queue_updated](queue<int> tq, bool is_updated) {
            if (is_updated) {
                if (tq.empty()) {
                    cout << "[Empty]\n";
                } else {
                    cout << "[";
                    while (!tq.empty()) {
                        cout << "P" << tq.front() + 1;
                        tq.pop();

                        if (tq.empty()) { cout << "]\n" ; } else { cout << ", "; }
                    }
                }
            }
            is_queue_updated = false;
        };

        check_queue();
        print_queue(q, is_queue_updated);
        is_queue_updated = false;

        if (!(q.empty())) {

            int current_id = q.front();
            q.pop();
            is_queue_updated = true;

            if (p[current_id].remaining_time <= time_quantum) {

                current_time += p[current_id].remaining_time;
                p[current_id].remaining_time = 0;
                p[current_id].completion_time = current_time;
                p[current_id].turnaround_time =  p[current_id].completion_time - p[current_id].arrival_time;
                p[current_id].waiting_time = p[current_id].turnaround_time - p[current_id].burst_time;

            } else {
                p[current_id].remaining_time -= time_quantum;
                current_time += time_quantum;
                check_queue();
                q.push(current_id);
                is_queue_updated = true;
                print_queue(q, is_queue_updated);
            }

        } else { current_time = next_arrival_time; }

        if (start_id >= num && q.empty()) {
            print_queue(q, true);
            int width = 25;
            cout << setw(width) << "\nProcess";
            cout << setw(width) << "Completion Time";
            cout << setw(width) << "Turnaround Time";
            cout << setw(width) << "Waiting Time\n";

            for (int i = 0; i < num; i++) {
                cout << left << "P" << setw(width) << p[i].id;
                cout << left << setw(width) << p[i].completion_time;
                cout << left << setw(width) << p[i].turnaround_time;
                cout << left << setw(width) << p[i].waiting_time;
                cout << "\n\n";
            }

            float avg_w = 0;
            for (int i = 0; i < num; i++) {
                avg_w += p[i].waiting_time;
            }
            avg_w /= num;
            cout << "Average Waiting Time: " << avg_w << "\n";

            break;
        }
    }
    return 0;
}
