#include <iostream>
#include "task.h"

Task::Task(string title) {
    SetTitle(title);
    SetIsCompleted(false);
}

void Task::SetTitle(string title) {
    m_title = title;
}

string Task::GetTitle() {
    return m_title;
}

void Task::SetIsCompleted(bool is_completed) {
    m_is_completed = is_completed;
}
bool Task::GetIsCompleted() {
    return m_is_completed;
}
