#include <iostream>
#include <memory>
/// @brief interface class
class IMessage {
 public:
  IMessage() = default;
  virtual ~IMessage() = default;
  virtual void print() const = 0;
};

/// @brief concrete class
class Message : public IMessage {
 public:
  Message() = default;
  ~Message() override = default;
  void print() const override { std::cout << "Message::print()" << std::endl; }
};
/// @brief concrete class
class NetworkMessage : public IMessage {
 public:
  NetworkMessage() = default;
  ~NetworkMessage() override = default;
  // copy constructor
  NetworkMessage(const NetworkMessage& rhs) = default;
  // copy assignment operator
  NetworkMessage& operator=(const NetworkMessage& rhs) = default;
  // move constructor
  NetworkMessage(NetworkMessage&& rhs) = default;
  // move assignment operator
  NetworkMessage& operator=(NetworkMessage&& rhs) = default;
  void print() const override {
    std::cout << "NetworkMessage::print()" << std::endl;
  }
};

using IMessagePtr = std::shared_ptr<IMessage>;

void PrintMessage(const IMessage& msg) { msg.print(); }
void PrintMessage(const IMessagePtr& msg) { msg->print(); }

class MessageWrapper {
 public:
  explicit MessageWrapper(IMessage& msg) : msg_(msg) {}
  ~MessageWrapper() = default;
  void ShowMsg() const { msg_.print(); }
  const IMessage& GetMsg() const { return msg_; }

 private:
  IMessage& msg_;
};
using MessageWrapperPtr = std::shared_ptr<MessageWrapper>;

int main() {
  auto my_msg = std::make_shared<Message>();
  PrintMessage(*my_msg);

  auto net_msg = std::make_shared<NetworkMessage>();
  PrintMessage(net_msg);

  MessageWrapperPtr wrapper_msg = nullptr;
  {
    NetworkMessage net_msg2;
    wrapper_msg = std::make_shared<MessageWrapper>(net_msg2);
    wrapper_msg->ShowMsg();

    auto& msg = wrapper_msg->GetMsg();
    msg.print();
  }
  // crash
  // wrapper_msg->ShowMsg();
  return 0;
}