#ifndef _TASK__H_
#define _TASK__H_

#include <string>

using std::string;

class Task {
private:
    string m_title;
    bool m_is_completed;
public:
    Task(string title);

    void SetTitle(string title);
    string GetTitle();

    void SetIsCompleted(bool is_completed);
    bool GetIsCompleted();
};

#endif //_TASK__H_
