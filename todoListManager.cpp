//
// Created by regal on 8/23/25.
//

#include "todoListManager.h"

tdl::TodoListManager::TodoListManager()
{
    loadTasks();
}

void tdl::TodoListManager::loadTasks()
{
    m_tasks.clear();
    std::ifstream inFile{"tasks.txt"};
    if (!inFile.is_open())
    {
        std::cerr << "Could not read the file! \n";
        return;
    }
    std::string status, tName, tDescriptionline;

    // 1. read status till the end
    while (std::getline(inFile, status))
    {
        if (status.empty()) {
            continue;
        }
        // 2. read task name after status
        if (!std::getline(inFile, tName))
        {
            std::cerr << "Malformed file!";
            break;
        }

        // 3. read description by looping till End-of-record delimiter '~~~'
        std::string tDescription{};
        while (std::getline(inFile, tDescriptionline))
        {
            if (tDescriptionline == "~~~")
            {
                break;
            }
            tDescription += tDescriptionline + '\n';
        }
        if (!tDescription.empty())
        {
            tDescription.pop_back();
        }
        m_tasks.emplace_back(Task {status == "1", tName, tDescription});
    }
}


void tdl::TodoListManager::writeToFile() const
{
    std::ofstream outFile("tasks.txt");
    if (outFile.is_open())
    {
        for (const auto& [m_isCompleted, m_taskName, m_taskDescription]: m_tasks)
        {
            outFile << m_isCompleted << '\n';
            outFile << m_taskName << '\n';
            outFile << m_taskDescription << '\n';
            outFile << "~~~" << '\n'; // End-of-record delimiter
        }
    }
}

void tdl::TodoListManager::viewAllTasks() const
{
    if (m_tasks.empty())
    {
        std::cout << "No tasks present! Hooray!\n \n";
        return;
    }
    int taskIndex{0};
    std::cout << "i) Status | Task Name \n";
    for (const auto& task: m_tasks)
    {
        std::cout << taskIndex << ") " << (task.m_isCompleted ? "Completed | " : "Incomplete | ") << task.m_taskName;
        std::cout << '\n';
        ++taskIndex;
    }
    std::cout << '\n';
}

void tdl::TodoListManager::markTaskComplete(const std::size_t taskIndex)
{
    if (taskIndex >= m_tasks.size()) { return; }
    if (!m_tasks[taskIndex].m_isCompleted)
    {
        m_tasks[taskIndex].m_isCompleted = true;
        std::cout << "Task completed successfully! \n \n";
    }
    else
    {
        std::cout << "Task already completed! \n \n";
    }
}