#include <algorithm>
#include <atomic>
#include <cstddef>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using FuncQueue = std::deque<std::function<void()>>;

class TaskPool {
 public:
  virtual ~TaskPool() {
    std::cout << "TaskPool::~TaskPool()" << std::endl;
    stop_ = true;
    for (auto& worker : workers_) {
      worker.join();
    }
    delete &Instance();
  }

  static TaskPool& Instance() {
    static TaskPool* ins = nullptr;
    if (!ins) {
      static std::once_flag flag;
      std::call_once(flag, [&] { ins = ins = new (std::nothrow) TaskPool(); });
    }
    return *ins;
  }

  // disable copy constructor
  TaskPool(const TaskPool&) = delete;
  // disable copy assignment operator
  TaskPool& operator=(const TaskPool&) = delete;
  // enable move constructor
  TaskPool(TaskPool&&) = delete;
  // enable move assignment operator
  TaskPool& operator=(TaskPool&&) = delete;

  template <typename F, typename... Args>
  void Execute(F&& f, Args&&... args) {
    std::function<void()> task =
        std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    task_queue_.push_back(task);
  }

 private:
  TaskPool() {
    workers_.emplace_back([this]() {
      std::cout << "TaskPool::TaskPool() workers_ thread id: "
                << std::this_thread::get_id() << std::endl;
      while (!stop_) {
        if (task_queue_.empty()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          continue;
        }
        std::function<void()> task = task_queue_.front();
        task_queue_.pop_front();
        task();
      }
    });
  }
  bool stop_ = false;
  FuncQueue task_queue_;
  std::vector<std::thread> workers_;
};

class Task {
 public:
  Task() {}
  virtual ~Task() {}
  // disable copy constructor
  Task(const Task&) = delete;
  // disable copy assignment operator
  Task& operator=(const Task&) = delete;
  // enable move constructor
  Task(Task&&) = delete;
  // enable move assignment operator
  Task& operator=(Task&&) = delete;

  //############################################
  void AddData(const std::string& data) {
    TaskPool::Instance().Execute(
        [this](const std::string& data) { this->Computing(data); }, data);
  }

  void Computing(const std::string& data) const {
    std::cout << "Task::Computing()" << data << std::endl;
  }
};

int main() {
  Task task;
  std::string data = "hello world";
  task.AddData(data);
  int cnt = 0;
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (++cnt > 3) {
      break;
    }
  }
  std::cout << "main thread exit" << std::endl;
}