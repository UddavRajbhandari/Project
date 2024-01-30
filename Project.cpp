#include <iostream>
#include <queue>
#include <string>
#include <stack>
using namespace std;
struct Task {
    string name;
    string description;
    int priority;
    int waitingTime;
    int insertionOrder;
    bool completed;

    // Comparison function to create a max heap with priority and insertion order
    bool operator<(const Task& other) const {
        if (priority == other.priority) {
            return insertionOrder > other.insertionOrder;
        }

        int agedPriority = priority - waitingTime / 10;
        int otherAgedPriority = other.priority - other.waitingTime / 10;

        return agedPriority < otherAgedPriority;
    }
};

class TaskScheduler {
private:
    priority_queue<Task> tasks;
    int insertionCounter;
    stack<priority_queue<Task>> undoStack;
    stack<priority_queue<Task>> redoStack;

public:
    TaskScheduler() : insertionCounter(0) {}

    void addTask(const string& name, const string& description, int priority) {
        tasks.emplace(Task{name, description, priority, 0, insertionCounter++, false});
        saveState();
    }

    void addTaskInteractive() {
        string taskName, taskDescription;
        int taskPriority;

        cout << "Enter task name: ";
        cin >> taskName;

        cout << "Enter task description: ";
        cin.ignore();
        getline(cin, taskDescription);

        cout << "Enter task priority: ";
        cin >> taskPriority;

        addTask(taskName, taskDescription, taskPriority);

        cout << "Task added successfully.\n";
    }

    Task getNextTask() {
        if (!tasks.empty()) {
            Task nextTask = tasks.top();
            tasks.pop();
            return nextTask;
        } else {
            return {"", "", 0, 0, 0, false};
        }
    }

    void updateWaitingTime() {
        priority_queue<Task> tempQueue;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();
            ++task.waitingTime;
            tempQueue.push(task);
        }

        tasks = move(tempQueue);
        saveState();
    }

    void adjustPriority(const string& taskName, int newPriority) {
        priority_queue<Task> tempQueue;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();

            if (task.name == taskName) {
                task.priority = newPriority;
            }

            tempQueue.push(task);
        }

        tasks = move(tempQueue);
        saveState();
    }

    void markTaskAsCompleted(const string& taskName) {
        priority_queue<Task> tempQueue;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();

            if (task.name == taskName) {
                task.completed = true;
            }

            tempQueue.push(task);
        }

        tasks = move(tempQueue);
        saveState();
    }

    void undo() {
        if (undoStack.size() > 1) { // Ensure there are at least two states to undo
            redoStack.push(tasks);
            undoStack.pop();
            tasks = undoStack.top();
        } else {
            cout << "Cannot undo further.\n";
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(tasks);
            tasks = redoStack.top();
            redoStack.pop();
        } else {
            cout << "Cannot redo further.\n";
        }
    }

    void saveState() {
        undoStack.push(tasks);
        redoStack = stack<priority_queue<Task>>(); // Clear redo stack
    }

    void displayAllTasks() const {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
        } else {
            priority_queue<Task> tempQueue = tasks;
            cout << "All Tasks in the Queue:" << endl;
            while (!tempQueue.empty()) {
                Task task = tempQueue.top();
                tempQueue.pop();
                cout << "Task Name: " << task.name << "\tPriority: " << task.priority
                          << "\tWaiting Time: " << task.waitingTime << "\tCompleted: "
                          << (task.completed ? "Yes" : "No") << endl;
                cout << "Description: " << task.description << endl;
                cout << "------------------------" << endl;
            }
        }
    }
};

int main() {
    TaskScheduler scheduler;

    cout << "Welcome to the Task Scheduler!" << endl;

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

        scheduler.addTask(taskName, taskDescription, taskPriority);

        cout << "Do you want to add more tasks? (y/n): ";
        cin >> addMore;
    } while (addMore != 'n' && addMore != 'N');

    while (true) {
        cout << "\nChoose an option:\n";
        cout << "1. Get Next Task\n";
        cout << "2. Update Waiting Times\n";
        cout << "3. Adjust Priority\n";
        cout << "4. Mark Task as Completed\n";
        cout << "5. Undo\n";
        cout << "6. Redo\n";
        cout << "7. Display All Tasks\n";
        cout << "8. Add New Task\n";
        cout << "9. Exit\n";

        int option;
        cin >> option;

        switch (option) {
            case 1: {
                Task nextTask = scheduler.getNextTask();
                if (!nextTask.name.empty()) {
                    cout << "Executing '" << nextTask.name << "' with priority " << nextTask.priority << endl;
                } else {
                    cout << "No more tasks in the queue." << endl;
                    return 0;
                }
                break;
            }
            case 2:
                scheduler.updateWaitingTime();
                cout << "Waiting times updated.\n";
                break;
            case 3: {
                string taskName;
                int newPriority;

                cout << "Enter task name: ";
                cin >> taskName;

                cout << "Enter new priority: ";
                cin >> newPriority;

                scheduler.adjustPriority(taskName, newPriority);
                cout << "Priority adjusted.\n";
                break;
            }
            case 4: {
                string taskName;

                cout << "Enter task name: ";
                cin >> taskName;

                scheduler.markTaskAsCompleted(taskName);
                cout << "Task marked as completed.\n";
                break;
            }
            case 5:
                scheduler.undo();
                cout << "Undo completed.\n";
                break;
            case 6:
                scheduler.redo();
                cout << "Redo completed.\n";
                break;
            case 7:
                scheduler.displayAllTasks();
                break;
            case 8:
                scheduler.addTaskInteractive();
                break;
            case 9:
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
