#include <iostream>
#include <queue>
#include <string>
#include <ctime> // For time tracking

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
    queue<Task> completedTasks;
    int insertionCounter;
    time_t startTime; // To track start time for waiting tasks

public:
    TaskScheduler() : insertionCounter(0), startTime(time(NULL)) {}

    // Method to add a task with specified name, description, and priority
    void addTask(const string& name, const string& description, int priority) {
        Task newTask(name, description, priority, insertionCounter++, false);
        tasks.push(newTask);
    }

    // Method to interactively add a task by taking user input
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

    // Method to get the next task from the priority queue
    Task getNextTask() {
        if (!tasks.empty()) {
            Task nextTask = tasks.top();
            tasks.pop();
            return nextTask;
        } else {
            // Returning a default task with empty name and priority 0 to indicate no tasks available
            return Task("", "", 0, 0, false);
        }
    }

    // Method to update waiting times for all tasks in the queue
    void updateWaitingTime() {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }

        // Calculate current time
        time_t currentTime = time(NULL);
        double elapsedTime = difftime(currentTime, startTime);

        // Add 5 to waiting time for all tasks
        int timeIncrement = 5;

        priority_queue<Task> tempQueue;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();

            // Update waiting time
            task.waitingTime += timeIncrement;

            // Increase priority by 5 if waiting for more than 5 units
            if (task.waitingTime >= 5) {
                task.priority += 5;
            }

            tempQueue.push(task);
        }

        tasks.swap(tempQueue);
        cout << "Waiting times updated.\n";
    }

    // Method to adjust the priority of a specific task
    void adjustPriority(const string& taskName, int newPriority) {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }

        priority_queue<Task> tempQueue;
        bool taskFound = false;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();

            if (task.name == taskName) {
                task.priority = newPriority;
                taskFound = true;
            }

            tempQueue.push(task);
        }

        tasks.swap(tempQueue);

        if (taskFound) {
            cout << "Priority adjusted.\n";
        } else {
            cout << "Task not found.\n";
        }
    }

    // Method to mark a task as completed
    void markTaskAsCompleted(const string& taskName) {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }

        priority_queue<Task> tempQueue;
        bool taskFound = false;

        while (!tasks.empty()) {
            Task task = tasks.top();
            tasks.pop();

            if (task.name == taskName) {
                task.completed = true;
                taskFound = true;
                completedTasks.push(task); // Add completed task to the completed tasks queue
            } else {
                tempQueue.push(task); // Add non-completed tasks to the temporary queue
            }
        }

        tasks.swap(tempQueue);

        if (taskFound) {
            cout << "Task marked as completed.\n";
        } else {
            cout << "Task not found.\n";
        }
    }

    // Method to clear completed tasks from the queue
    void clearCompletedTasks() {
        while (!completedTasks.empty()) {
            completedTasks.pop();
        }
    }

    // Method to display information about all tasks in the queue
    void displayAllTasks() const {
        if (tasks.empty() && completedTasks.empty()) {
            cout << "No tasks in the queue." << endl;
        } else {
            // Display tasks in the queue
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

            // Display completed tasks without removing them from the queue
            queue<Task> tempCompletedQueue = completedTasks;
            cout<<endl;
            cout << "Completed Tasks:" << endl;
            cout<<endl;
            while (!tempCompletedQueue.empty()) {
                Task task = tempCompletedQueue.front();
                tempCompletedQueue.pop();
                cout << "Task Name: " << task.name << "\tPriority: " << task.priority
                          << "\tWaiting Time: " << task.waitingTime << "\tCompleted: Yes" << endl;
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
        cout << "5. Display All Tasks\n";
        cout << "6. Add New Task\n";
        cout << "7. Exit\n";

        int option;
        cin >> option;

        switch (option) {
            case 1: {
                Task nextTask = scheduler.getNextTask();
                if (nextTask.name.empty()) {
                    cout << "No more tasks in the queue." << endl;
                    return 0;
                } else {
                    cout << "Executing '" << nextTask.name << "' with priority " << nextTask.priority << endl;
                }
                break;
            }
            case 2:
                scheduler.updateWaitingTime();
                break;
            case 3: {
                string taskName;
                int newPriority;

                cout << "Enter task name: ";
                cin >> taskName;

                cout << "Enter new priority: ";
                cin >> newPriority;

                scheduler.adjustPriority(taskName, newPriority);
                break;
            }
            case 4: {
                string taskName;

                cout << "Enter task name: ";
                cin >> taskName;

                scheduler.markTaskAsCompleted(taskName);
                break;
            }
            case 5:
                scheduler.displayAllTasks();
                break;
            case 6:
                scheduler.addTaskInteractive();
                break;
            case 7:
                // Clear completed tasks from the queue before exiting
                scheduler.clearCompletedTasks();
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
