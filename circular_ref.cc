#include <iostream>
#include <memory>
#include <vector>

/// @brief an example to show circular reference

class ConnectionManager;
using ConnectionManagerPtr = std::shared_ptr<ConnectionManager>;
class Connection {
 public:
  explicit Connection(const ConnectionManagerPtr& mgr) { mgr_ = mgr; }
  virtual ~Connection() {
    std::cout << "Connection::~Connection()" << std::endl;
    mgr_ = nullptr;
  }
  void Print() const { std::cout << "Connection::Print()" << std::endl; }

 private:
  // std::weak_ptr<ConnectionManager> mgr_;
  ConnectionManagerPtr mgr_;
};

using ConnectionPtr = std::shared_ptr<Connection>;
class ConnectionManager {
 public:
  ConnectionManager() = default;
  virtual ~ConnectionManager() {
    std::cout << "ConnectionManager::~ConnectionManager()" << std::endl;
    connections_.clear();
  }
  void Print() const { std::cout << "ConnectionManager::Print()" << std::endl; }
  void AddConnection(const ConnectionPtr& conn) {
    connections_.push_back(conn);
  }

 private:
  std::vector<ConnectionPtr> connections_;
};

int main() {
  auto mgr = std::make_shared<ConnectionManager>();
  auto conn = std::make_shared<Connection>(mgr);
  conn->Print();
  mgr->AddConnection(conn);
  return 0;
}