/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef directory_lease_service_H
#define directory_lease_service_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "directory_service_types.h"

namespace elasticmem { namespace directory {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class directory_lease_serviceIf {
 public:
  virtual ~directory_lease_serviceIf() {}
  virtual void update_leases(rpc_lease_ack& _return, const rpc_lease_update& updates) = 0;
};

class directory_lease_serviceIfFactory {
 public:
  typedef directory_lease_serviceIf Handler;

  virtual ~directory_lease_serviceIfFactory() {}

  virtual directory_lease_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(directory_lease_serviceIf* /* handler */) = 0;
};

class directory_lease_serviceIfSingletonFactory : virtual public directory_lease_serviceIfFactory {
 public:
  directory_lease_serviceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf>& iface) : iface_(iface) {}
  virtual ~directory_lease_serviceIfSingletonFactory() {}

  virtual directory_lease_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(directory_lease_serviceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> iface_;
};

class directory_lease_serviceNull : virtual public directory_lease_serviceIf {
 public:
  virtual ~directory_lease_serviceNull() {}
  void update_leases(rpc_lease_ack& /* _return */, const rpc_lease_update& /* updates */) {
    return;
  }
};

typedef struct _directory_lease_service_update_leases_args__isset {
  _directory_lease_service_update_leases_args__isset() : updates(false) {}
  bool updates :1;
} _directory_lease_service_update_leases_args__isset;

class directory_lease_service_update_leases_args {
 public:

  directory_lease_service_update_leases_args(const directory_lease_service_update_leases_args&);
  directory_lease_service_update_leases_args& operator=(const directory_lease_service_update_leases_args&);
  directory_lease_service_update_leases_args() {
  }

  virtual ~directory_lease_service_update_leases_args() throw();
  rpc_lease_update updates;

  _directory_lease_service_update_leases_args__isset __isset;

  void __set_updates(const rpc_lease_update& val);

  bool operator == (const directory_lease_service_update_leases_args & rhs) const
  {
    if (!(updates == rhs.updates))
      return false;
    return true;
  }
  bool operator != (const directory_lease_service_update_leases_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const directory_lease_service_update_leases_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class directory_lease_service_update_leases_pargs {
 public:


  virtual ~directory_lease_service_update_leases_pargs() throw();
  const rpc_lease_update* updates;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _directory_lease_service_update_leases_result__isset {
  _directory_lease_service_update_leases_result__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _directory_lease_service_update_leases_result__isset;

class directory_lease_service_update_leases_result {
 public:

  directory_lease_service_update_leases_result(const directory_lease_service_update_leases_result&);
  directory_lease_service_update_leases_result& operator=(const directory_lease_service_update_leases_result&);
  directory_lease_service_update_leases_result() {
  }

  virtual ~directory_lease_service_update_leases_result() throw();
  rpc_lease_ack success;
  directory_lease_service_exception ex;

  _directory_lease_service_update_leases_result__isset __isset;

  void __set_success(const rpc_lease_ack& val);

  void __set_ex(const directory_lease_service_exception& val);

  bool operator == (const directory_lease_service_update_leases_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(ex == rhs.ex))
      return false;
    return true;
  }
  bool operator != (const directory_lease_service_update_leases_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const directory_lease_service_update_leases_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _directory_lease_service_update_leases_presult__isset {
  _directory_lease_service_update_leases_presult__isset() : success(false), ex(false) {}
  bool success :1;
  bool ex :1;
} _directory_lease_service_update_leases_presult__isset;

class directory_lease_service_update_leases_presult {
 public:


  virtual ~directory_lease_service_update_leases_presult() throw();
  rpc_lease_ack* success;
  directory_lease_service_exception ex;

  _directory_lease_service_update_leases_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

template <class Protocol_>
class directory_lease_serviceClientT : virtual public directory_lease_serviceIf {
 public:
  directory_lease_serviceClientT(apache::thrift::stdcxx::shared_ptr< Protocol_> prot) {
    setProtocolT(prot);
  }
  directory_lease_serviceClientT(apache::thrift::stdcxx::shared_ptr< Protocol_> iprot, apache::thrift::stdcxx::shared_ptr< Protocol_> oprot) {
    setProtocolT(iprot,oprot);
  }
 private:
  void setProtocolT(apache::thrift::stdcxx::shared_ptr< Protocol_> prot) {
  setProtocolT(prot,prot);
  }
  void setProtocolT(apache::thrift::stdcxx::shared_ptr< Protocol_> iprot, apache::thrift::stdcxx::shared_ptr< Protocol_> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return this->piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return this->poprot_;
  }
  void update_leases(rpc_lease_ack& _return, const rpc_lease_update& updates);
  void send_update_leases(const rpc_lease_update& updates);
  void recv_update_leases(rpc_lease_ack& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< Protocol_> piprot_;
  apache::thrift::stdcxx::shared_ptr< Protocol_> poprot_;
  Protocol_* iprot_;
  Protocol_* oprot_;
};

typedef directory_lease_serviceClientT< ::apache::thrift::protocol::TProtocol> directory_lease_serviceClient;

template <class Protocol_>
class directory_lease_serviceProcessorT : public ::apache::thrift::TDispatchProcessorT<Protocol_> {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
  virtual bool dispatchCallTemplated(Protocol_* iprot, Protocol_* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (directory_lease_serviceProcessorT::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef void (directory_lease_serviceProcessorT::*SpecializedProcessFunction)(int32_t, Protocol_*, Protocol_*, void*);
  struct ProcessFunctions {
    ProcessFunction generic;
    SpecializedProcessFunction specialized;
    ProcessFunctions(ProcessFunction g, SpecializedProcessFunction s) :
      generic(g),
      specialized(s) {}
    ProcessFunctions() : generic(NULL), specialized(NULL) {}
  };
  typedef std::map<std::string, ProcessFunctions> ProcessMap;
  ProcessMap processMap_;
  void process_update_leases(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_update_leases(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
 public:
  directory_lease_serviceProcessorT(::apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> iface) :
    iface_(iface) {
    processMap_["update_leases"] = ProcessFunctions(
      &directory_lease_serviceProcessorT::process_update_leases,
      &directory_lease_serviceProcessorT::process_update_leases);
  }

  virtual ~directory_lease_serviceProcessorT() {}
};

typedef directory_lease_serviceProcessorT< ::apache::thrift::protocol::TDummyProtocol > directory_lease_serviceProcessor;

template <class Protocol_>
class directory_lease_serviceProcessorFactoryT : public ::apache::thrift::TProcessorFactory {
 public:
  directory_lease_serviceProcessorFactoryT(const ::apache::thrift::stdcxx::shared_ptr< directory_lease_serviceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< directory_lease_serviceIfFactory > handlerFactory_;
};

typedef directory_lease_serviceProcessorFactoryT< ::apache::thrift::protocol::TDummyProtocol > directory_lease_serviceProcessorFactory;

class directory_lease_serviceMultiface : virtual public directory_lease_serviceIf {
 public:
  directory_lease_serviceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~directory_lease_serviceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> > ifaces_;
  directory_lease_serviceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<directory_lease_serviceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void update_leases(rpc_lease_ack& _return, const rpc_lease_update& updates) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->update_leases(_return, updates);
    }
    ifaces_[i]->update_leases(_return, updates);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
template <class Protocol_>
class directory_lease_serviceConcurrentClientT : virtual public directory_lease_serviceIf {
 public:
  directory_lease_serviceConcurrentClientT(apache::thrift::stdcxx::shared_ptr< Protocol_> prot) {
    setProtocolT(prot);
  }
  directory_lease_serviceConcurrentClientT(apache::thrift::stdcxx::shared_ptr< Protocol_> iprot, apache::thrift::stdcxx::shared_ptr< Protocol_> oprot) {
    setProtocolT(iprot,oprot);
  }
 private:
  void setProtocolT(apache::thrift::stdcxx::shared_ptr< Protocol_> prot) {
  setProtocolT(prot,prot);
  }
  void setProtocolT(apache::thrift::stdcxx::shared_ptr< Protocol_> iprot, apache::thrift::stdcxx::shared_ptr< Protocol_> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return this->piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return this->poprot_;
  }
  void update_leases(rpc_lease_ack& _return, const rpc_lease_update& updates);
  int32_t send_update_leases(const rpc_lease_update& updates);
  void recv_update_leases(rpc_lease_ack& _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< Protocol_> piprot_;
  apache::thrift::stdcxx::shared_ptr< Protocol_> poprot_;
  Protocol_* iprot_;
  Protocol_* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

typedef directory_lease_serviceConcurrentClientT< ::apache::thrift::protocol::TProtocol> directory_lease_serviceConcurrentClient;

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

}} // namespace

#include "directory_lease_service.tcc"
#include "directory_service_types.tcc"

#endif
