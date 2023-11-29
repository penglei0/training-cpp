#include <iostream>
#include <memory>
#include <vector>

using octetVec = std::vector<uint8_t>;

/// layer view of receiving:
/// application layer(upper layer) <-- the TCP layer entity <--- IP layer(lower
/// layer)

/***************************************
 * Interfaces/notifiers for upper layers
 ***************************************/
/// This interface represents the data entry point of the receiving side of a
/// TCP layer entity. The lower-layers will use this class to pass PDUs into the
/// TCP.
class tcp_rx_lower_layer_interface {
 public:
  tcp_rx_lower_layer_interface() = default;
  virtual ~tcp_rx_lower_layer_interface() = default;
  tcp_rx_lower_layer_interface(const tcp_rx_lower_layer_interface&) = delete;
  tcp_rx_lower_layer_interface& operator=(const tcp_rx_lower_layer_interface&) =
      delete;
  tcp_rx_lower_layer_interface(const tcp_rx_lower_layer_interface&&) = delete;
  tcp_rx_lower_layer_interface& operator=(
      const tcp_rx_lower_layer_interface&&) = delete;

  /// \brief Interface for the lower layer to pass PDUs into the TCP layer
  /// \param pdu PDU to be handled
  virtual void handle_pdu(octetVec pdu) = 0;
};

/// This interface represents the data exit point of the receiving side of the
/// TCP layer entity. The TCP layer will use this call to pass SDUs into the
/// upper layer entity.
class tcp_rx_upper_layer_notifier {
 public:
  tcp_rx_upper_layer_notifier() = default;
  virtual ~tcp_rx_upper_layer_notifier() = default;
  tcp_rx_upper_layer_notifier(const tcp_rx_upper_layer_notifier&) = delete;
  tcp_rx_upper_layer_notifier& operator=(const tcp_rx_upper_layer_notifier&) =
      delete;
  tcp_rx_upper_layer_notifier(const tcp_rx_upper_layer_notifier&&) = delete;
  tcp_rx_upper_layer_notifier& operator=(const tcp_rx_upper_layer_notifier&&) =
      delete;

  /// \brief Interface for the TCP layer entity to pass SDUs into the upper
  /// layer.
  /// \param sdu SDU to be passed to upper layer.
  virtual void on_new_sdu(octetVec sdu) = 0;
};

/***************************************
 * Interfaces/notifiers of IP layer
 ***************************************/
/// \brief Interface to inform upper layers about reception of new PDUs.
class ip_rx_pdu_interface {
 public:
  virtual ~ip_rx_pdu_interface() = default;
  virtual void on_new_pdu(octetVec pdu) = 0;
};

/// \brief Interface to inform upper layers about reception of new PDUs.
class ip_rx_pdu_notifier {
 public:
  virtual ~ip_rx_pdu_notifier() = default;
  /// SDU to be passed to upper layer.
  virtual void handle_sdu(octetVec sdu) = 0;
};

/// Adapter between TCP and IP layer: ip -> tcp
class ip_tcp_adapter : public ip_rx_pdu_notifier {
 public:
  explicit ip_tcp_adapter(tcp_rx_lower_layer_interface& tcp_handler_)
      : tcp_handler(&tcp_handler_) {}
  ~ip_tcp_adapter() = default;

  void handle_sdu(octetVec sdu) override {
    std::cout << "ip handle the PDUs" << std::endl;
    tcp_handler->handle_pdu(std::move(sdu));
  }

 private:
  tcp_rx_lower_layer_interface* tcp_handler = nullptr;
};

class ip_rx_entity : public ip_rx_pdu_interface {
 public:
  ip_rx_entity() = default;
  ~ip_rx_entity() = default;
  void connect_tcp(ip_tcp_adapter& tcp_dn_) { tcp_dn = &tcp_dn_; }
  void on_new_pdu(octetVec pdu) override { tcp_dn->handle_sdu(std::move(pdu)); }

 private:
  ip_tcp_adapter* tcp_dn = nullptr;
};

/// @brief Adapter between TCP and app layer: tcp -> app.
class app_tcp_adapter : public tcp_rx_upper_layer_notifier {
 public:
  app_tcp_adapter() = default;
  ~app_tcp_adapter() = default;
  void on_new_sdu(octetVec sdu) override {
    std::cout << "application handle the PDUs" << std::endl;
  }
};

class tcp_rx_entity : public tcp_rx_lower_layer_interface {
 public:
  explicit tcp_rx_entity(tcp_rx_upper_layer_notifier& upper_dn_)
      : upper_dn(upper_dn_) {}
  ~tcp_rx_entity() = default;
  void handle_pdu(octetVec pdu) override {
    // receive pdu from lower layer
    std::cout << "tcp handle the PDUs" << std::endl;
    upper_dn.on_new_sdu(std::move(pdu));
  }

 private:
  tcp_rx_upper_layer_notifier& upper_dn;
};

int main() {
  // tcp/ip app layers
  // inside of a app entity may have a `app_tcp_adapter`
  app_tcp_adapter app_adapter;

  tcp_rx_entity tcp_layer(app_adapter);

  ip_tcp_adapter tcp_adapter(tcp_layer);

  // inside of a ip entity may have a `ip_tcp_adapter`
  ip_rx_entity ip_rx;

  ip_rx.connect_tcp(tcp_adapter);

  // simulate the receiving of a PDU from lower layer
  octetVec data;
  ip_rx.on_new_pdu(data);  // ip -> tcp -> app

}
