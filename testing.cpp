#include <iostream>
#include <queue>
#include <string>
using namespace std;
struct Task {
    string name;
    int priority;
    // Comparison function to create a max heap
    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};
class TaskScheduler {
private:

    priority_queue<Task> tasks;

public:
    void addTask(const string& name, int priority) {
        tasks.push({name, priority});
    }
    /*
    
    Problem arose- 

    when there are many more priorities task but the one with low priority will not be completed  .
    for this , if the tasks is more dominated by the tasks with VIP tasks then we can also increase the low priority task by 1. with this we can ensure that the task with low priority will also be meet.
    if the tasks have same priority sort the task with the task which is near to the deadline(completion) of the task
    */
    Task getNextTask() {
        if (!tasks.empty()) {
            Task nextTask = tasks.top();
            tasks.pop();
            return nextTask;
        } else {
            // Returning a default task when the queue is empty
            return {"", 0};
        }
    }
};

int main() {
    TaskScheduler scheduler;

    // Adding tasks with priorities
    scheduler.addTask("Task 1", 3);
    scheduler.addTask("Task 2", 1);
    scheduler.addTask("Task 3", 5);
    scheduler.addTask("Task 4", 2);

    // Getting and executing tasks based on priority
    while (true) {
        Task nextTask;
        nextTask= scheduler.getNextTask();
        if (!nextTask.name.empty()) {
            cout << "Executing '" << nextTask.name << "' with priority " << nextTask.priority << endl;
        } else {
            cout << "No more tasks in the queue." << endl;
            break;
        }
    }

    return 0;
}
