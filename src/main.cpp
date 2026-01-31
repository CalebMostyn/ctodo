#include <iostream>
#include "task.h"

int main() {
    std::cout << "Hello World!" << std::endl;
    Task* new_task = new Task("do the dishes");
    std::cout << "Task is : " << new_task->GetTitle() << std::endl;
}
