#include <iostream>
#include <queue>
#include <string>
#include <ctime> // For time tracking

using namespace std;

const int HIGH_PRIORITY_THRESHOLD = 50; // Threshold for high priority tasks
const int HIGH_PRIORITY_TASK_COUNT_THRESHOLD = 10; // Threshold for number of high priority tasks
const double WAITING_TIME_PRIORITY_FACTOR = 10.0; // Factor to adjust priority based on waiting time
struct Task {
    string name;
    string description;
    int priority;
    double waitingTime; 
    int insertionOrder;
    bool completed;

    Task(const string& n, const string& desc, int prio, int order, bool comp)
        : name(n), description(desc), priority(prio), waitingTime(0), insertionOrder(order), completed(comp) {}

    bool operator<(const Task& other) const {
        if (priority == other.priority) {
            return insertionOrder > other.insertionOrder;
        }

        int agedPriority = priority - static_cast<int>(waitingTime / 10); // Adjusted based on waiting time
        int otherAgedPriority = other.priority - static_cast<int>(other.waitingTime / 10); // Adjusted based on waiting time

        return agedPriority < otherAgedPriority;
    }
};
class TaskComparator {
public:
    bool operator()(const Task& t1, const Task& t2) const {
        return t1 < t2; // Use the overloaded less-than operator
    }
};
class TaskScheduler {
private:
    priority_queue<Task, vector<Task>, TaskComparator> tasks; // Use custom comparator
    queue<Task> completedTasks;
    int insertionCounter;
    time_t startTime; // To track start time for waiting tasks

public:
    TaskScheduler() : insertionCounter(0), startTime(time(NULL)) {}
    void addTask(const string& name, const string& description, int priority) {
        if (name.empty() || description.empty()) {
            cout << "Task name and description cannot be empty." << endl;
            return;
        }
        if (priority < 0) {
            cout << "Priority cannot be negative." << endl;
            return;
        }
        Task newTask(name, description, priority, insertionCounter++, false);
        tasks.push(newTask);
    }
    void addTaskInteractive() {
        string taskName, taskDescription;
        int taskPriority;
        cin.ignore();
        cout << "Enter task name: ";
        getline(cin, taskName);
        if (taskName.empty()) {
            cout << "Task name cannot be empty." << endl;
            return;
        }
        cout << "Enter task description: ";
        getline(cin, taskDescription);
        if (taskDescription.empty()) {
            cout << "Task description cannot be empty." << endl;
            return;
        }
        cout << "Enter task priority: ";
        while (!(cin >> taskPriority) || taskPriority < 0) {
            cout << "Invalid input for priority, please enter a non-negative integer: ";
            cin.clear();
            while (cin.get() != '\n') {} // Clear the input buffer
        }
        addTask(taskName, taskDescription, taskPriority);
        cout << "Task added successfully.\n";
    }
    Task getNextTask() {
        if (!tasks.empty()) {
            Task nextTask = tasks.top();
            tasks.pop();
            return nextTask;
        } else {
            return Task("", "", 0, 0, false);
        }
    }
void updateWaitingTime() {
    if (tasks.empty()) {
        cout << "No tasks in the queue." << endl;
        return;
    }  
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, startTime);
    int timeIncrement = 5;
    // Initialize a temporary queue to hold updated tasks
    priority_queue<Task, vector<Task>, TaskComparator> tempQueue; 
    // Get the count of high priority tasks
    int highPriorityTaskCount = getHighPriorityTaskCount(); 
    while (!tasks.empty()) {
        Task task = tasks.top();
        tasks.pop();
        double waitingTime;
        if (task.priority < HIGH_PRIORITY_THRESHOLD && highPriorityTaskCount >= HIGH_PRIORITY_TASK_COUNT_THRESHOLD) {
            waitingTime = task.waitingTime + elapsedTime; // Increase waiting time for low priority tasks
        } else {
            waitingTime = task.waitingTime + timeIncrement; // Update waiting time for other tasks
        }    
        // Adjust priority based on waiting time
        // Increase priority for tasks based on their waiting time
        int priorityIncrement = static_cast<int>(waitingTime / WAITING_TIME_PRIORITY_FACTOR);
        task.priority += priorityIncrement;     
        // Update the task with the new waiting time
        task.waitingTime = waitingTime;      
        // Push the updated task into the temporary queue
        tempQueue.push(task);
    }
    // Replace the original queue with the updated one
    tasks.swap(tempQueue);
    cout << "Waiting times updated.\n";
}
    void adjustPriority(const string& taskName, int newPriority) {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }
        priority_queue<Task, vector<Task>, TaskComparator> tempQueue; // Use custom comparator
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
    void markTaskAsCompleted(const string& taskName) {
        if (tasks.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }
        priority_queue<Task, vector<Task>, TaskComparator> tempQueue; // Use custom comparator
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
    void clearCompletedTasks() {
        while (!completedTasks.empty()) {
            completedTasks.pop();
        }
    }
    void displayAllTasks() const {
        if (tasks.empty() && completedTasks.empty()) {
            cout << "No tasks in the queue." << endl;
        } else {
            priority_queue<Task, vector<Task>, TaskComparator> tempQueue = tasks; // Use custom comparator
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

            queue<Task> tempCompletedQueue = completedTasks;
            cout << endl;
            cout << "Completed Tasks:" << endl;
            cout << endl;
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

private:
    int getHighPriorityTaskCount() const {
        int count = 0;
        priority_queue<Task, vector<Task>, TaskComparator> tempQueue = tasks; // Use custom comparator
        while (!tempQueue.empty() && tempQueue.top().priority >= HIGH_PRIORITY_THRESHOLD) {
            tempQueue.pop();
            count++;
        }
        return count;
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
        cin.ignore();
        getline(cin, taskName);
        if (taskName.empty()) {
            cout << "Task name cannot be empty." << endl;
            continue;
        }
        cout << "Enter task description: ";
        getline(cin, taskDescription);
        if (taskDescription.empty()) {
            cout << "Task description cannot be empty." << endl;
            continue;
        }
        cout << "Enter task priority: ";
        while (!(cin >> taskPriority) || taskPriority < 0) {
            cout << "Invalid input for priority, please enter a non-negative integer: ";
            cin.clear();
            while (cin.get() != '\n') {} // Clear the input buffer
        }
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
                cin.ignore();
                cout << "Enter task name: ";
                getline(cin , taskName);

                cout << "Enter new priority: ";
                cin >> newPriority;

                scheduler.adjustPriority(taskName, newPriority);
                break;
            }
            case 4: {
                string taskName;
                cin.ignore();
                cout << "Enter task name: ";
                getline(cin , taskName);

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
                scheduler.clearCompletedTasks();
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
