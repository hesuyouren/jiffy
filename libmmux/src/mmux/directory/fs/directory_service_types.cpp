/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "directory_service_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace mmux { namespace directory {

int _krpc_perm_optionsValues[] = {
  rpc_replace,
  rpc_add,
  rpc_remove
};
const char* _krpc_perm_optionsNames[] = {
  "rpc_replace",
  "rpc_add",
  "rpc_remove"
};
const std::map<int, const char*> _rpc_perm_options_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _krpc_perm_optionsValues, _krpc_perm_optionsNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

std::ostream& operator<<(std::ostream& out, const rpc_perm_options val) {
  std::map<int, const char*>::const_iterator it = _rpc_perm_options_VALUES_TO_NAMES.find(val);
  if (it != _rpc_perm_options_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}

int _krpc_file_typeValues[] = {
  rpc_none,
  rpc_regular,
  rpc_directory
};
const char* _krpc_file_typeNames[] = {
  "rpc_none",
  "rpc_regular",
  "rpc_directory"
};
const std::map<int, const char*> _rpc_file_type_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _krpc_file_typeValues, _krpc_file_typeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

std::ostream& operator<<(std::ostream& out, const rpc_file_type val) {
  std::map<int, const char*>::const_iterator it = _rpc_file_type_VALUES_TO_NAMES.find(val);
  if (it != _rpc_file_type_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}

int _krpc_storage_modeValues[] = {
  rpc_in_memory,
  rpc_in_memory_grace,
  rpc_flushing,
  rpc_on_disk
};
const char* _krpc_storage_modeNames[] = {
  "rpc_in_memory",
  "rpc_in_memory_grace",
  "rpc_flushing",
  "rpc_on_disk"
};
const std::map<int, const char*> _rpc_storage_mode_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(4, _krpc_storage_modeValues, _krpc_storage_modeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

std::ostream& operator<<(std::ostream& out, const rpc_storage_mode val) {
  std::map<int, const char*>::const_iterator it = _rpc_storage_mode_VALUES_TO_NAMES.find(val);
  if (it != _rpc_storage_mode_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}


rpc_replica_chain::~rpc_replica_chain() throw() {
}


void rpc_replica_chain::__set_block_names(const std::vector<std::string> & val) {
  this->block_names = val;
}

void rpc_replica_chain::__set_slot_begin(const int32_t val) {
  this->slot_begin = val;
}

void rpc_replica_chain::__set_slot_end(const int32_t val) {
  this->slot_end = val;
}

void rpc_replica_chain::__set_storage_mode(const rpc_storage_mode val) {
  this->storage_mode = val;
}
std::ostream& operator<<(std::ostream& out, const rpc_replica_chain& obj)
{
  obj.printTo(out);
  return out;
}


void swap(rpc_replica_chain &a, rpc_replica_chain &b) {
  using ::std::swap;
  swap(a.block_names, b.block_names);
  swap(a.slot_begin, b.slot_begin);
  swap(a.slot_end, b.slot_end);
  swap(a.storage_mode, b.storage_mode);
}

rpc_replica_chain::rpc_replica_chain(const rpc_replica_chain& other7) {
  block_names = other7.block_names;
  slot_begin = other7.slot_begin;
  slot_end = other7.slot_end;
  storage_mode = other7.storage_mode;
}
rpc_replica_chain& rpc_replica_chain::operator=(const rpc_replica_chain& other8) {
  block_names = other8.block_names;
  slot_begin = other8.slot_begin;
  slot_end = other8.slot_end;
  storage_mode = other8.storage_mode;
  return *this;
}
void rpc_replica_chain::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "rpc_replica_chain(";
  out << "block_names=" << to_string(block_names);
  out << ", " << "slot_begin=" << to_string(slot_begin);
  out << ", " << "slot_end=" << to_string(slot_end);
  out << ", " << "storage_mode=" << to_string(storage_mode);
  out << ")";
}


rpc_file_status::~rpc_file_status() throw() {
}


void rpc_file_status::__set_type(const rpc_file_type val) {
  this->type = val;
}

void rpc_file_status::__set_permissions(const rpc_perms val) {
  this->permissions = val;
}

void rpc_file_status::__set_last_write_time(const int64_t val) {
  this->last_write_time = val;
}
std::ostream& operator<<(std::ostream& out, const rpc_file_status& obj)
{
  obj.printTo(out);
  return out;
}


void swap(rpc_file_status &a, rpc_file_status &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.permissions, b.permissions);
  swap(a.last_write_time, b.last_write_time);
}

rpc_file_status::rpc_file_status(const rpc_file_status& other10) {
  type = other10.type;
  permissions = other10.permissions;
  last_write_time = other10.last_write_time;
}
rpc_file_status& rpc_file_status::operator=(const rpc_file_status& other11) {
  type = other11.type;
  permissions = other11.permissions;
  last_write_time = other11.last_write_time;
  return *this;
}
void rpc_file_status::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "rpc_file_status(";
  out << "type=" << to_string(type);
  out << ", " << "permissions=" << to_string(permissions);
  out << ", " << "last_write_time=" << to_string(last_write_time);
  out << ")";
}


rpc_data_status::~rpc_data_status() throw() {
}


void rpc_data_status::__set_persistent_store_prefix(const std::string& val) {
  this->persistent_store_prefix = val;
}

void rpc_data_status::__set_chain_length(const int32_t val) {
  this->chain_length = val;
}

void rpc_data_status::__set_data_blocks(const std::vector<rpc_replica_chain> & val) {
  this->data_blocks = val;
}
std::ostream& operator<<(std::ostream& out, const rpc_data_status& obj)
{
  obj.printTo(out);
  return out;
}


void swap(rpc_data_status &a, rpc_data_status &b) {
  using ::std::swap;
  swap(a.persistent_store_prefix, b.persistent_store_prefix);
  swap(a.chain_length, b.chain_length);
  swap(a.data_blocks, b.data_blocks);
}

rpc_data_status::rpc_data_status(const rpc_data_status& other18) {
  persistent_store_prefix = other18.persistent_store_prefix;
  chain_length = other18.chain_length;
  data_blocks = other18.data_blocks;
}
rpc_data_status& rpc_data_status::operator=(const rpc_data_status& other19) {
  persistent_store_prefix = other19.persistent_store_prefix;
  chain_length = other19.chain_length;
  data_blocks = other19.data_blocks;
  return *this;
}
void rpc_data_status::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "rpc_data_status(";
  out << "persistent_store_prefix=" << to_string(persistent_store_prefix);
  out << ", " << "chain_length=" << to_string(chain_length);
  out << ", " << "data_blocks=" << to_string(data_blocks);
  out << ")";
}


rpc_dir_entry::~rpc_dir_entry() throw() {
}


void rpc_dir_entry::__set_name(const std::string& val) {
  this->name = val;
}

void rpc_dir_entry::__set_status(const rpc_file_status& val) {
  this->status = val;
}
std::ostream& operator<<(std::ostream& out, const rpc_dir_entry& obj)
{
  obj.printTo(out);
  return out;
}


void swap(rpc_dir_entry &a, rpc_dir_entry &b) {
  using ::std::swap;
  swap(a.name, b.name);
  swap(a.status, b.status);
}

rpc_dir_entry::rpc_dir_entry(const rpc_dir_entry& other20) {
  name = other20.name;
  status = other20.status;
}
rpc_dir_entry& rpc_dir_entry::operator=(const rpc_dir_entry& other21) {
  name = other21.name;
  status = other21.status;
  return *this;
}
void rpc_dir_entry::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "rpc_dir_entry(";
  out << "name=" << to_string(name);
  out << ", " << "status=" << to_string(status);
  out << ")";
}


directory_service_exception::~directory_service_exception() throw() {
}


void directory_service_exception::__set_msg(const std::string& val) {
  this->msg = val;
}
std::ostream& operator<<(std::ostream& out, const directory_service_exception& obj)
{
  obj.printTo(out);
  return out;
}


void swap(directory_service_exception &a, directory_service_exception &b) {
  using ::std::swap;
  swap(a.msg, b.msg);
}

directory_service_exception::directory_service_exception(const directory_service_exception& other22) : TException() {
  msg = other22.msg;
}
directory_service_exception& directory_service_exception::operator=(const directory_service_exception& other23) {
  msg = other23.msg;
  return *this;
}
void directory_service_exception::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "directory_service_exception(";
  out << "msg=" << to_string(msg);
  out << ")";
}

const char* directory_service_exception::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: directory_service_exception";
  }
}

}} // namespace
