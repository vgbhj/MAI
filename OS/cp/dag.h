#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <mutex>
#include <shared_mutex>

// Job structure
struct Job {
    int id;
    std::vector<int> dependencies;
    bool isStart;
    bool isEnd;
    int componentId;
    std::string command;

    Job() = default;
    ~Job() = default;

    Job(int _id, const std::vector<int>& _dependencies, bool _isStart, bool _isEnd, std::string _command)
        : id(_id), dependencies(_dependencies), isStart(_isStart), isEnd(_isEnd), componentId(-1),  command(_command){}
};


// Функция для присвоения компоненты связанности
int assignComponentId(int jobId, int componentId, std::vector<Job>& jobs) {
    int tmp_dep = componentId;
    if (jobs[jobId].componentId == -1) {
        for (int dependency : jobs[jobId].dependencies) {
            if(jobs[dependency].componentId == -1){
                jobs[dependency].componentId = assignComponentId(dependency, componentId, jobs);
            }
            tmp_dep = std::min(tmp_dep, jobs[dependency].componentId);
        }
        return tmp_dep;
    }

    return jobs[jobId].componentId;
}

// Функция для поиска компонент связанности
void findComponents(std::vector<Job>& jobs) {
    int componentId = 0;
    for (auto& job : jobs) {
        if (job.componentId == -1) {
            job.componentId = assignComponentId(job.id, componentId, jobs);
            componentId++;
        }

        // Debug
        // std::cout << job.componentId << ' ' << job.id << '\n';
    }
}

// Функция для проверки наличия одной компоненты связанности
bool hasSingleComponent(const std::vector<Job>& jobs) {
    int componentId = jobs.begin()->componentId;
    for (const auto& job : jobs) {
        if (job.componentId != componentId) {
            return false;
        }
    }
    return true;
}

// Function to check for cycles
bool hasCycle(int jobId, const std::vector<Job>& jobs, std::unordered_set<int>& visited, std::unordered_set<int>& recStack) {
    if (!visited.count(jobId)) {
        visited.insert(jobId);
        recStack.insert(jobId);

        for (int dependency : jobs[jobId].dependencies) {
            if (!visited.count(dependency) && hasCycle(dependency, jobs, visited, recStack)) {
                return true;  // Cycle detected
            } else if (recStack.count(dependency)) {
                return true;  // Cycle detected
            }
        }
    }

    recStack.erase(jobId);
    return false;
}

// Function to validate DAG
bool validateDAG(const std::vector<Job>& jobs) {
    std::unordered_set<int> visited;
    std::unordered_set<int> recStack;

    bool hasStartJob = false;
    bool hasEndJob = false;

    for (const auto& job : jobs) {
        if (!visited.count(job.id) && hasCycle(job.id, jobs, visited, recStack)) {
            return false;  // Cycle detected
        }

        if (job.isStart) {
            hasStartJob = true;
        }

        if (job.isEnd) {
            if (job.dependencies.size() != 0)
                return false;
            hasEndJob = true;
        }
    }

    return hasStartJob && hasEndJob && hasSingleComponent(jobs);

    return true;
}

// Function to parse YAML configuration
std::vector<Job> parseYAML(const YAML::Node& root) {
    std::vector<Job> jobs;

    for (const auto& entry : root) {
        int jobId = entry.first.as<int>() - 1;
        const auto& jobData = entry.second;

        std::vector<int> dependencies;
        std::string cmd;
        bool isStart = jobData["isStart"].as<bool>();
        bool isEnd = jobData["isEnd"].as<bool>();

        if (jobData["dependencies"]) {
            // std::cout << jobData["dependencies"] << '\n';
            dependencies = jobData["dependencies"].as<std::vector<int>>();
            for(int i = 0; i < dependencies.size(); ++i){
                dependencies[i]--;
            }
            // std::cout << dependencies.size() << '\n';
        }

        if(jobData["command"]){
            cmd = jobData["command"].as<std::string>();
        }

        jobs.push_back(Job(jobId, dependencies, isStart, isEnd, cmd));
    }

    return jobs;
}


class DAG_J
{
private:
    std::vector<Job> jobs;
public:
    DAG_J() = default;
    ~DAG_J() = default;

    void createDAG(std::string filename){

        try {
            YAML::Node root = YAML::LoadFile(filename);
            jobs = parseYAML(root);
        } catch (const std::exception& e) {
            std::cerr << "Error reading configuration file: " << e.what() << std::endl;
        }

        findComponents(jobs);
        if (validateDAG(jobs)) {
            std::cout << "DAG is valid." << std::endl;
        } else {
            std::cout << "DAG is not valid. Cycles detected or invalid start/end jobs." << std::endl;
        }
    }

    void runCompile(){
        std::queue<int> readyJobs;
        std::unordered_set<int> completedJobs;

        // Находим все джобы без зависимостей (готовые к выполнению) и добавляем их в очередь
        for (size_t i = 0; i < jobs.size(); ++i) {
            if (jobs[i].dependencies.empty()) {
                readyJobs.push(i);
            }
        }
        

        // Запускаем выполнение джоб в нескольких процессах
        while (!readyJobs.empty()) {
            std::vector<pid_t> childPids;

            // Ограничим максимальное количество процессов для запуска
            int maxProcesses = 5; // Например, 5 процессов
            int jobIndex;
            while (!readyJobs.empty() && maxProcesses > 0) {
                jobIndex = readyJobs.front();
                readyJobs.pop();

                pid_t pid = fork();

                if (pid < 0) {
                    std::cout << "Ошибка при создании процесса-потомка." << std::endl;
                    exit(EXIT_FAILURE);
                }

                if (pid == 0) {
                    // child process
                    executeJob(jobIndex);
                    exit(EXIT_SUCCESS);
                } else {
                    // parent process
                    childPids.push_back(pid);
                    --maxProcesses;
                }
            }

            // Ожидаем завершение выполнения дочерних процессов
            for (pid_t pid : childPids) {
                int status;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status)) {
                    int exitStatus = WEXITSTATUS(status);
                    if (exitStatus == EXIT_SUCCESS) {
                        completedJobs.insert(jobIndex);
                    }
                }
            }

            // Находим новые готовые джобы для выполнения 
            for (size_t i = 0; i < jobs.size(); ++i) {
                if (completedJobs.count(i) == 0 && std::all_of(jobs[i].dependencies.begin(), jobs[i].dependencies.end(),
                    [&completedJobs](int dep) { return completedJobs.count(dep) > 0; })) {
                    readyJobs.push(i);
                }
            }
        }
    }


    void executeJob(int jobIndex){
        std::string compileCommand = jobs[jobIndex].command;

        if (!compileCommand.empty()) {
            std::cout << "Выполнение команды: " << compileCommand << std::endl;

            // Выполняем команду в процессе-потомке
            FILE* pipe = popen(compileCommand.c_str(), "r");
            if (pipe) {
                pclose(pipe);
            } else {
                std::cout << "Ошибка при выполнении команды компиляции." << std::endl;
                exit(EXIT_FAILURE);
            }
            } else {
                std::cout << "Команда компиляции для джобы не найдена." << std::endl;
                exit(EXIT_FAILURE);
            }
    }
};