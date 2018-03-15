#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include "directory_lease_client.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

namespace elasticmem {
namespace directory {

directory_lease_client::~directory_lease_client() {
  if (transport_ != nullptr)
    disconnect();
}

directory_lease_client::directory_lease_client(const std::string &host, int port) {
  connect(host, port);
}

void directory_lease_client::connect(const std::string &host, int port) {
  socket_ = std::make_shared<TSocket>(host, port);
  transport_ = std::shared_ptr<TTransport>(new TBufferedTransport(socket_));
  protocol_ = std::shared_ptr<TProtocol>(new TBinaryProtocol(transport_));
  client_ = std::make_shared<thrift_client>(protocol_);
  transport_->open();
}

void directory_lease_client::disconnect() {
  if (transport_->isOpen()) {
    transport_->close();
  }
}

rpc_lease_ack directory_lease_client::update_leases(const rpc_lease_update &updates) {
  rpc_lease_ack ack;
  client_->update_leases(ack, updates);
  return ack;
}

}
}