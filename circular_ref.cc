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
  // method 2: how to break circular reference
  // **we need to call this method explicitly when this object is no longer used
  void Close() {
    // eliminate circular reference
    mgr_ = nullptr;
  }

 private:
  // method 1: how to break circular reference
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

  void CloseConnection() {
    for (auto& conn : connections_) {
      conn->Close();
    }
  }

 private:
  std::vector<ConnectionPtr> connections_;
};

int main() {
  auto mgr = std::make_shared<ConnectionManager>();
  auto conn = std::make_shared<Connection>(mgr);
  conn->Print();
  mgr->AddConnection(conn);
  mgr->CloseConnection();
  return 0;
}