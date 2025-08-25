//
// Created by regal on 8/23/25.
//

#ifndef TODOLISTMANAGERGUI_TODOLISTMANAGER_H
#define TODOLISTMANAGERGUI_TODOLISTMANAGER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace tdl
{
    struct Task
    {
        bool m_isCompleted{false};
        std::string m_taskName{};
        std::string m_taskDescription {};
    };

    class TodoListManager
    {
    public:
        [[nodiscard]] std::size_t getTasksSize() const { return m_tasks.size();}

        void addTask(const Task& task){m_tasks.emplace_back(task);}

        void removeTask(const std::size_t taskIndex)
        {
            if (taskIndex >= m_tasks.size()) { return; }
            m_tasks.erase(m_tasks.begin() + static_cast<int>(taskIndex));
        }

        Task& viewTask(const std::size_t taskIndex) {return m_tasks[taskIndex];}

        TodoListManager();
        void markTaskComplete(std::size_t taskIndex);
        void viewAllTasks() const;
        void writeToFile() const;
        void loadTasks();

    private:
        std::vector<Task> m_tasks{};
    };
}


#endif //TODOLISTMANAGERGUI_TODOLISTMANAGER_H