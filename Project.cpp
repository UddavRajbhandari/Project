#include <iostream>
#include <string>
using namespace std;
struct Task {
    string name;
    string description;
    int priority;
    int waitingTime;
    int insertionOrder;
    bool completed;

    Task(const string& n, const string& desc, int prio, int order, bool comp)
        : name(n), description(desc), priority(prio), waitingTime(0), insertionOrder(order), completed(comp) {}

    // Comparison function to create a max heap with priority and insertion order
   bool operator<(const Task& other) const {
    // First compare by priority
    if (priority != other.priority) {
        return priority < other.priority;
    }
    // If priorities are equal, compare by aged priority
    int agedPriority = priority - waitingTime / 10;
    int otherAgedPriority = other.priority - other.waitingTime / 10;
    return agedPriority < otherAgedPriority;
}

};

class PriorityQueue {
private:
    struct Node {
        Task data;
        Node* next;

        Node(const Task& task) : data(task), next(nullptr) {}
    };

    Node* front;

public:
    PriorityQueue() : front(nullptr) {}

    void enqueue(const Task& task) {
        Node* newNode = new Node(task);

        if (!front || task < front->data) {
            newNode->next = front;
            front = newNode;
        } else {
            Node* current = front;
            while (current->next && !(task < current->next->data)) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    Task dequeue() {
        if (isEmpty()) {
            return Task("", "", 0, 0, false);
        }

        Node* temp = front;
        Task frontTask = temp->data;
        front = front->next;
        delete temp;

        return frontTask;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    void adjustPriority(const string& taskName, int newPriority) {
        Node* current = front;
        while (current != nullptr) {
            if (current->data.name == taskName) {
                current->data.priority = newPriority;
                break;
            }
            current = current->next;
        }
    }

    void markTaskAsCompleted(const string& taskName) {
        Node* current = front;
        while (current != nullptr) {
            if (current->data.name == taskName) {
                current->data.completed = true;
                break;
            }
            current = current->next;
        }
    }

    void updateWaitingTime() {
        Node* current = front;
        while (current != nullptr) {
            current->data.waitingTime++;
            current = current->next;
        }
        // After updating waiting times, we may need to reorder the tasks
        reorder();
    }

    // Reorder the tasks based on priority and insertion order
    void reorder() {
        Node* sorted = nullptr;
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;

            if (sorted == nullptr || temp->data < sorted->data) {
                temp->next = sorted;
                sorted = temp;
            } else {
                Node* current = sorted;
                while (current->next && !(temp->data < current->next->data)) {
                    current = current->next;
                }
                temp->next = current->next;
                current->next = temp;
            }
        }
        front = sorted;
    }

    // Other methods...

    void displayAllTasks() const {
        Node* current = front;
        cout << "All Tasks in the Priority Queue:" << endl;
        while (current != nullptr) {
            Task task = current->data;
            cout << "Task Name: " << task.name << "\tPriority: " << task.priority
                      << "\tWaiting Time: " << task.waitingTime << "\tCompleted: "
                      << (task.completed ? "Yes" : "No") << endl;
            cout << "Description: " << task.description << endl;
            cout << "------------------------" << endl;

            current = current->next;
        }
    }
};

int main() {
    PriorityQueue taskQueue;

    cout << "Welcome to the Priority Task Scheduler!" << endl;

    char addMore;
    do {
        string taskName, taskDescription;
        int taskPriority;

        cout << "Enter task name: ";
        cin >> taskName;

        cout << "Enter task description: ";
        cin.ignore();
        getline(cin, taskDescription);

        cout << "Enter task priority: ";
        cin >> taskPriority;

        taskQueue.enqueue(Task{taskName, taskDescription, taskPriority, 0, false});

        cout << "Do you want to add more tasks? (y/n): ";
        cin >> addMore;
    } while (addMore != 'n' && addMore != 'N');

    while (true) {
        cout << "\nChoose an option:\n";
        cout << "1. Get Next Task\n";
        cout << "2. Update Waiting Times\n";
        cout << "3. Adjust Priority\n";
        cout << "4. Mark Task as Completed\n";
        cout << "5. Display All Tasks\n";
        cout << "6. Add New Task\n";
        cout << "7. Exit\n";

        int option;
        cin >> option;

        switch (option) {
            case 1: {
                Task nextTask = taskQueue.dequeue();
                if (!nextTask.name.empty()) {
                    cout << "Executing '" << nextTask.name << "' with priority " << nextTask.priority << endl;
                } else {
                    cout << "No more tasks in the queue." << endl;
                    return 0;
                }
                break;
            }
            case 2:
                taskQueue.updateWaitingTime();
                cout << "Waiting times updated.\n";
                break;
            case 3: {
                string taskName;
                int newPriority;

                cout << "Enter task name: ";
                cin >> taskName;

                cout << "Enter new priority: ";
                cin >> newPriority;

                taskQueue.adjustPriority(taskName, newPriority);
                cout << "Priority adjusted.\n";
                break;
            }
            case 4: {
                string taskName;

                cout << "Enter task name: ";
                cin >> taskName;

                taskQueue.markTaskAsCompleted(taskName);
                cout << "Task marked as completed.\n";
                break;
            }
            case 5:
                taskQueue.displayAllTasks();
                break;
            case 6: {
                string taskName, taskDescription;
                int taskPriority;

                cout << "Enter task name: ";
                cin >> taskName;

                cout << "Enter task description: ";
                cin.ignore();
                getline(cin, taskDescription);

                cout << "Enter task priority: ";
                cin >> taskPriority;

                taskQueue.enqueue(Task{taskName, taskDescription, taskPriority, 0, false});

                cout << "Task added successfully.\n";
                break;
            }
            case 7:
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
