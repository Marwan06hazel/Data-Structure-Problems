#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <queue>

using namespace std;

struct Patient {
    int id;
    string name;
    int severity;
    int arrivalTime;
};

struct ComparePatients {
    bool operator()(const Patient& p1, const Patient& p2) {
        if (p1.severity == p2.severity) {
            return p1.arrivalTime > p2.arrivalTime;
        }
        return p1.severity < p2.severity;
    }
};

vector<Patient> heap;
vector<int> patientIndex;
priority_queue<Patient, vector<Patient>, ComparePatients> pq;

int parent(int i) {
    return (i - 1) / 2;
}

int leftChild(int i) {
    return 2 * i + 1;
}

int rightChild(int i) {
    return 2 * i + 2;
}

bool hasHigherPriority(const Patient& p1, const Patient& p2) {

    if (p1.severity == p2.severity) {
        return p1.arrivalTime < p2.arrivalTime;
    }
    return p1.severity > p2.severity;
}

void heapifyUp(int index) {

    Patient currentPatient = heap[index];
    int parentIndex = parent(index);

    while (index > 0 && hasHigherPriority(currentPatient, heap[parentIndex])) {

        heap[index] = heap[parentIndex];
        patientIndex[heap[index].id] = index;

        index = parentIndex;
        parentIndex = parent(index);
    }

    heap[index] = currentPatient;
    patientIndex[currentPatient.id] = index;
}

void heapifyDown(int index) {
    if (heap.empty()) return;

    Patient currentPatient = heap[index];

    while (leftChild(index) < heap.size()) {
        int highest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() && hasHigherPriority(heap[left], currentPatient)) {
            highest = left;
        }

        if (right < heap.size()) {
            if (hasHigherPriority(heap[right], heap[highest])) {
                highest = right;
            }
        }

        if (highest == index) {
            break;
        }

        heap[index] = heap[highest];
        patientIndex[heap[index].id] = index;

        index = highest;
    }

    heap[index] = currentPatient;
    patientIndex[currentPatient.id] = index;
}

void insertPatient(const Patient& patient) {
    heap.push_back(patient);
    int index = heap.size() - 1;

    if (patient.id >= patientIndex.size()) {
        patientIndex.resize(patient.id + 10, -1);
    }

    patientIndex[patient.id] = index;
    heapifyUp(index);
}

void treatNextPatient() {
    if (heap.empty()) {
        cout << "\nNo patients available\n";
        return;
    }

    Patient treated = heap[0];
    Patient lastPatient = heap.back();

    heap.pop_back();
    patientIndex[treated.id] = -1;

    if (!heap.empty()) {
        heap[0] = lastPatient;
        patientIndex[lastPatient.id] = 0;
        heapifyDown(0);
    }

    cout << "\nTreated Patient: " << treated.name << "\n";
}

void viewNextPatient() {
    if (heap.empty()) {
        cout << "\nThere are no patients\n";
        return;
    }

    const Patient &p = heap[0];
    cout << "\nPatient: " << p.name;
    cout << "\nId: " << p.id;
    cout << "\nSeverity: " << p.severity;
    cout << "\nArrival Time: " << p.arrivalTime << "\n";
}

void updateSeverity(int id, int newSeverity) {
    if (id < 0 || id >= patientIndex.size() || patientIndex[id] == -1) {
        cout << "\nPatient not found\n";
        return;
    }

    int index = patientIndex[id];
    int oldSeverity = heap[index].severity;

    heap[index].severity = newSeverity;

    if (newSeverity > oldSeverity) {
        heapifyUp(index);
    }
    else if (newSeverity < oldSeverity) {
        heapifyDown(index);
    }

    cout << "\nSeverity Updated to " << newSeverity << " for Patient " << heap[patientIndex[id]].name << "\n";
}

void displayAllPatients() {
    if (heap.empty()) {
        cout << "\nNo patients available\n";
        return;
    }

    vector<Patient> tempHeap = heap;
    sort(tempHeap.begin(), tempHeap.end(), [](const Patient& p1, const Patient& p2) {
        if (p1.severity == p2.severity) {
            return p1.arrivalTime < p2.arrivalTime;
        }
        return p1.severity > p2.severity;
    });

    int width = 20;
    cout << "\n";
    cout << left << setw(width) << "Patient";
    cout << setw(width) << "Id";
    cout << setw(width) << "Severity";
    cout << setw(width) << "Arrival Time";
    cout << "\n";
    cout << string(80, '-') << "\n";

    for (Patient& p : tempHeap) {
        cout << left << setw(width) << p.name;
        cout << setw(width) << p.id;
        cout << setw(width) << p.severity;
        cout << setw(width) << p.arrivalTime;
        cout << "\n";
    }
}

void compareHeap_Pq() {
    int TEST_SIZE;

    cout << "\nEnter Test Patients Number:";
    cin >> TEST_SIZE;

    cout << "\nStarting Performance Comparison (" << TEST_SIZE << " Patients)...\n";
    cout << "Please wait, CPU is calculating...\n";

    vector<Patient> backupHeap = heap;
    vector<int> backupMap = patientIndex;
    priority_queue<Patient, vector<Patient>, ComparePatients> backupPq = pq;

    heap.clear();
    patientIndex.clear();
    pq = {};

    auto startHeap = chrono::high_resolution_clock::now();

    heap.reserve(TEST_SIZE);
    patientIndex.assign(TEST_SIZE + 100, -1);

    for (int i = 0; i < TEST_SIZE; i++) {
        Patient p = {i, "P", rand() % 10 + 1, i};
        insertPatient(p);
    }

    while (!heap.empty()) {
        Patient treated = heap[0];
        Patient lastPatient = heap.back();
        heap.pop_back();
        patientIndex[treated.id] = -1;
        if (!heap.empty()) {
            heap[0] = lastPatient;
            heapifyDown(0);
        }
    }

    auto endHeap = chrono::high_resolution_clock::now();
    auto heapDuration = chrono::duration_cast<chrono::milliseconds>(endHeap - startHeap).count();

    auto startPq = chrono::high_resolution_clock::now();

    for (int i = 0; i < TEST_SIZE; i++) {
        pq.push({i, "P", rand() % 10 + 1, i});
    }

    while (!pq.empty()) {
        pq.pop();
    }

    auto endPq = chrono::high_resolution_clock::now();
    auto pqDuration = chrono::duration_cast<chrono::milliseconds>(endPq - startPq).count();

    cout << "\n" << string(50, '=') << "\n";
    cout << "   PERFORMANCE RESULTS (" << TEST_SIZE << " Insert & Treat)\n";
    cout << string(50, '=') << "\n";
    cout << "Manual Heap Time: " << heapDuration << " ms\n";
    cout << "Priority Queue Time: " << pqDuration << " ms\n";
    cout << string(50, '=') << "\n";

    if (heapDuration <= pqDuration) {
        cout << "Heap is faster\n";
    } else {
        cout << "Priority Queue is Faster\n";
    }

    heap = backupHeap;
    patientIndex = backupMap;
    pq = backupPq;
}

int main() {
    int nextId = 21;

    Patient patient1{1, "patient 1", 1, 0};
    Patient patient2{2, "patient 2", 5, 1};
    Patient patient3{3, "patient 3", 2, 2};
    Patient patient4{4, "patient 4", 3, 3};
    Patient patient5{5, "patient 5", 9, 4};
    Patient patient6{6, "patient 6", 9, 5};
    Patient patient7{7, "patient 7", 10, 6};
    Patient patient8{8, "patient 8", 10, 7};
    Patient patient9{9, "patient 9", 8, 8};
    Patient patient10{10, "patient 10", 4, 9};
    Patient patient11{11, "patient 11", 6, 10};
    Patient patient12{12, "patient 12", 6, 11};
    Patient patient13{13, "patient 13", 8, 12};
    Patient patient14{14, "patient 14", 7, 13};
    Patient patient15{15, "patient 15", 10, 14};
    Patient patient16{16, "patient 16", 2, 15};
    Patient patient17{17, "patient 17", 7, 16};
    Patient patient18{18, "patient 18", 10, 17};
    Patient patient19{19, "patient 19", 5, 18};
    Patient patient20{20, "patient 20", 4, 19};

    cout << "\nWelcome to your hospital emergency system\n"
                "Choose the way you love to handle your data\n"
                "1. Manual Heap\n"
                "2. STL priority_queue\n";

    int select;
    cin >> select;

    if (select == 1) {

        insertPatient(patient1);
        insertPatient(patient2);
        insertPatient(patient3);
        insertPatient(patient4);
        insertPatient(patient5);
        insertPatient(patient6);
        insertPatient(patient7);
        insertPatient(patient8);
        insertPatient(patient9);
        insertPatient(patient10);
        insertPatient(patient11);
        insertPatient(patient12);
        insertPatient(patient13);
        insertPatient(patient14);
        insertPatient(patient15);
        insertPatient(patient16);
        insertPatient(patient17);
        insertPatient(patient18);
        insertPatient(patient19);
        insertPatient(patient20);

    } else {

        pq.push(patient1);
        pq.push(patient2);
        pq.push(patient3);
        pq.push(patient4);
        pq.push(patient5);
        pq.push(patient6);
        pq.push(patient7);
        pq.push(patient8);
        pq.push(patient9);
        pq.push(patient10);
        pq.push(patient11);
        pq.push(patient12);
        pq.push(patient13);
        pq.push(patient14);
        pq.push(patient15);
        pq.push(patient16);
        pq.push(patient17);
        pq.push(patient18);
        pq.push(patient19);
        pq.push(patient20);

    }

    while (true) {

        cout << "\nChoose one option:\n";
        cout << "1. Insert Patient\n";
        cout << "2. Treat Next Patient\n";
        cout << "3. View Next Patient\n";
        cout << "4. Update Patient Severity\n";
        cout << "5. Display All Patients\n";
        cout << "6. Start Comparison\n";
        cout << "7. Reset All\n";
        cout << "0. Exit\n";

        int choice;
        cin >> choice;

        if (select == 1) {

            switch (choice) {
                case 1: {
                    Patient newPatient;
                    newPatient.id = nextId;
                    newPatient.arrivalTime = nextId;
                    nextId++;

                    cout << "Enter Name:";
                    cin >> newPatient.name;
                    cout << "Enter Severity:";
                    cin >> newPatient.severity;

                    insertPatient(newPatient);
                    cout << "\nPatient Added Successfully\n";
                    break;
                }

                case 2:
                    treatNextPatient();
                    break;

                case 3:
                    viewNextPatient();
                    break;

                case 4: {
                    int id, severity;
                    cout << "\nEnter Patient Id:";
                    cin >> id;
                    cout << "Enter New Severity:";
                    cin >> severity;

                    updateSeverity(id, severity);
                    break;
                }

                case 5:
                    displayAllPatients();
                    break;

                case 6:
                    compareHeap_Pq();
                    break;

                case 7: {
                    heap.clear();
                    patientIndex.clear();
                    nextId = 1;
                    cout << "\nReset Completed\n";
                    break;
                }

                case 0:
                    cout << "\nGood Bye\n";
                    return 0;

                default:
                    cout << "\nInvalid choice\n";
            }
        } else {

            switch (choice) {
                case 1: {
                    Patient newPatient;
                    newPatient.id = nextId;
                    newPatient.arrivalTime = nextId;
                    nextId++;

                    cout << "Enter Name:";
                    cin >> newPatient.name;
                    cout << "Enter Severity:";
                    cin >> newPatient.severity;

                    pq.push(newPatient);
                    cout << "\nPatient Added Successfully\n";
                    break;
                }

                case 2: {
                    if (!pq.empty()) pq.pop();
                    else cout << "\nNo patients available\n";
                    break;
                }

                case 3: {
                    if (pq.empty()) {
                        cout << "\nThere are no patients\n";
                        break;
                    }

                    const Patient& p = pq.top();
                    cout << "\nPatient: " << p.name;
                    cout << "\nId: " << p.id;
                    cout << "\nSeverity: " << p.severity;
                    cout << "\nArrival Time: " << p.arrivalTime << "\n";
                    break;
                }

                case 4: {
                    int id, severity;
                    cout << "\nEnter Patient Id:";
                    cin >> id;
                    cout << "Enter New Severity:";
                    cin >> severity;

                    priority_queue<Patient, vector<Patient>, ComparePatients> temp;
                    bool is_found = false;

                    while (!pq.empty()) {

                        Patient p = pq.top();
                        pq.pop();

                        if (p.id == id) {
                            is_found = true;
                            p.severity = severity;

                            cout << "\nSeverity Updated to " << severity << " for Patient " << p.name << "\n";
                        }

                        temp.push(p);
                    }

                    if (!is_found) {
                        cout << "\nPatient not found\n";
                    }

                    pq = temp;
                    break;
                }

                case 5: {

                    int width = 20;


                    if (!pq.empty()) {
                        cout << "\n";
                        cout << left << setw(width) << "Patient";
                        cout << setw(width) << "Id";
                        cout << setw(width) << "Severity";
                        cout << setw(width) << "Arrival Time";
                        cout << "\n";
                        cout << string(80, '-') << "\n";
                    } else {
                            cout << "\nNo patients available\n";
                            break;
                    }

                    priority_queue<Patient, vector<Patient>, ComparePatients> temp = pq;

                    while (!temp.empty()) {

                        const Patient& p = temp.top();

                        cout << left << setw(width) << p.name;
                        cout << setw(width) << p.id;
                        cout << setw(width) << p.severity;
                        cout << setw(width) << p.arrivalTime;
                        cout << "\n";

                        temp.pop();
                    }

                    break;
                }

                case 6:
                    compareHeap_Pq();
                    break;

                case 7: {
                    pq = {};
                    nextId = 1;
                    cout << "\nReset Completed\n";
                    break;
                }

                case 0:
                    cout << "\nGood Bye\n";
                    return 0;

                default:
                    cout << "\nInvalid choice\n";
            }
        }
    }
}
