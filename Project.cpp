#include <iostream>
#include <queue>
#include <string>

using namespace std;
struct Task {
    string name;
    int priority;
    int waitingTime; //New field to track waiting time
    // Comparison function to create a max heap
    bool operator<(const Task& other) const {
        //Introduce aging: Increase priority based on waiting time
        int agedPriority = priority + waitingTime/10;
        int otherAgedPriority = other.priority - other.waitingTime / 10;
        return agedPriority < otherAgedPriority;
    }
};
class TaskScheduler {
private:

    priority_queue<Task> tasks;

public:
    void addTask(const string& name, int priority) {
        tasks.emplace(Task{name, priority, 0});
    }

    Task getNextTask() {
        if (!tasks.empty()) {
            Task nextTask = tasks.top();
            tasks.pop();
            return nextTask;
        } else {
            // Returning a default task when the queue is empty
            return {"", 0,0};
        }
    }
    //Introduce dynamic priority adjustment: Update waiting time
    void updateWaitingTime(){
        priority_queue<Task> tempQueue;

    while (!tasks.empty()) {
        Task task = tasks.top();
        tasks.pop();
        ++task.waitingTime;
        tempQueue.push(task);
    }

    tasks = move(tempQueue);
    }
    
};

int main() {
    TaskScheduler scheduler;
    cout << "Welcome to the Task Scheduler!" << endl;

    char addMore;
    do {
        string taskName;
        int taskPriority;

        cout << "Enter task name: ";
        cin >> taskName;

        cout << "Enter task priority: ";
        cin >> taskPriority;

        scheduler.addTask(taskName, taskPriority);

        cout << "Do you want to add more tasks? (y/n): ";
        cin >> addMore;
    } while (addMore != 'n' && addMore != 'N');


    
    // Getting and executing tasks based on priority
   int executionCount = 0;
    while (executionCount < 10) {
        Task nextTask;
        nextTask= scheduler.getNextTask();
        if (!nextTask.name.empty()) {
            cout << "Executing '" << nextTask.name << "' with priority " << nextTask.priority << endl;
        } else {
            cout << "No more tasks in the queue." << endl;
            break;
        }
        executionCount++;
        scheduler.updateWaitingTime();
    }

    return 0;
}
