#
# Autogenerated by Thrift Compiler (0.11.0)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TFrozenDict, TException, TApplicationException
from thrift.protocol.TProtocol import TProtocolException
from thrift.TRecursive import fix_spec

import sys
import logging
from .ttypes import *
from thrift.Thrift import TProcessor
from thrift.transport import TTransport
all_structs = []


class Iface(object):
    def get_client_id(self):
        pass

    def register_client_id(self, block_id, client_id):
        """
        Parameters:
         - block_id
         - client_id
        """
        pass

    def command_request(self, seq, block_id, cmd_id, arguments):
        """
        Parameters:
         - seq
         - block_id
         - cmd_id
         - arguments
        """
        pass


class Client(Iface):
    def __init__(self, iprot, oprot=None):
        self._iprot = self._oprot = iprot
        if oprot is not None:
            self._oprot = oprot
        self._seqid = 0

    def get_client_id(self):
        self.send_get_client_id()
        return self.recv_get_client_id()

    def send_get_client_id(self):
        self._oprot.writeMessageBegin('get_client_id', TMessageType.CALL, self._seqid)
        args = get_client_id_args()
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_get_client_id(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = get_client_id_result()
        result.read(iprot)
        iprot.readMessageEnd()
        if result.success is not None:
            return result.success
        raise TApplicationException(TApplicationException.MISSING_RESULT, "get_client_id failed: unknown result")

    def register_client_id(self, block_id, client_id):
        """
        Parameters:
         - block_id
         - client_id
        """
        self.send_register_client_id(block_id, client_id)
        self.recv_register_client_id()

    def send_register_client_id(self, block_id, client_id):
        self._oprot.writeMessageBegin('register_client_id', TMessageType.CALL, self._seqid)
        args = register_client_id_args()
        args.block_id = block_id
        args.client_id = client_id
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_register_client_id(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = register_client_id_result()
        result.read(iprot)
        iprot.readMessageEnd()
        return

    def command_request(self, seq, block_id, cmd_id, arguments):
        """
        Parameters:
         - seq
         - block_id
         - cmd_id
         - arguments
        """
        self.send_command_request(seq, block_id, cmd_id, arguments)

    def send_command_request(self, seq, block_id, cmd_id, arguments):
        self._oprot.writeMessageBegin('command_request', TMessageType.ONEWAY, self._seqid)
        args = command_request_args()
        args.seq = seq
        args.block_id = block_id
        args.cmd_id = cmd_id
        args.arguments = arguments
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()


class Processor(Iface, TProcessor):
    def __init__(self, handler):
        self._handler = handler
        self._processMap = {}
        self._processMap["get_client_id"] = Processor.process_get_client_id
        self._processMap["register_client_id"] = Processor.process_register_client_id
        self._processMap["command_request"] = Processor.process_command_request

    def process(self, iprot, oprot):
        (name, type, seqid) = iprot.readMessageBegin()
        if name not in self._processMap:
            iprot.skip(TType.STRUCT)
            iprot.readMessageEnd()
            x = TApplicationException(TApplicationException.UNKNOWN_METHOD, 'Unknown function %s' % (name))
            oprot.writeMessageBegin(name, TMessageType.EXCEPTION, seqid)
            x.write(oprot)
            oprot.writeMessageEnd()
            oprot.trans.flush()
            return
        else:
            self._processMap[name](self, seqid, iprot, oprot)
        return True

    def process_get_client_id(self, seqid, iprot, oprot):
        args = get_client_id_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = get_client_id_result()
        try:
            result.success = self._handler.get_client_id()
            msg_type = TMessageType.REPLY
        except TTransport.TTransportException:
            raise
        except TApplicationException as ex:
            logging.exception('TApplication exception in handler')
            msg_type = TMessageType.EXCEPTION
            result = ex
        except Exception:
            logging.exception('Unexpected exception in handler')
            msg_type = TMessageType.EXCEPTION
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("get_client_id", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_register_client_id(self, seqid, iprot, oprot):
        args = register_client_id_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = register_client_id_result()
        try:
            self._handler.register_client_id(args.block_id, args.client_id)
            msg_type = TMessageType.REPLY
        except TTransport.TTransportException:
            raise
        except TApplicationException as ex:
            logging.exception('TApplication exception in handler')
            msg_type = TMessageType.EXCEPTION
            result = ex
        except Exception:
            logging.exception('Unexpected exception in handler')
            msg_type = TMessageType.EXCEPTION
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("register_client_id", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_command_request(self, seqid, iprot, oprot):
        args = command_request_args()
        args.read(iprot)
        iprot.readMessageEnd()
        try:
            self._handler.command_request(args.seq, args.block_id, args.cmd_id, args.arguments)
        except TTransport.TTransportException:
            raise
        except Exception:
            logging.exception('Exception in oneway handler')

# HELPER FUNCTIONS AND STRUCTURES


class get_client_id_args(object):


    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('get_client_id_args')
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(get_client_id_args)
get_client_id_args.thrift_spec = (
)


class get_client_id_result(object):
    """
    Attributes:
     - success
    """


    def __init__(self, success=None,):
        self.success = success

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 0:
                if ftype == TType.I64:
                    self.success = iprot.readI64()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('get_client_id_result')
        if self.success is not None:
            oprot.writeFieldBegin('success', TType.I64, 0)
            oprot.writeI64(self.success)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(get_client_id_result)
get_client_id_result.thrift_spec = (
    (0, TType.I64, 'success', None, None, ),  # 0
)


class register_client_id_args(object):
    """
    Attributes:
     - block_id
     - client_id
    """


    def __init__(self, block_id=None, client_id=None,):
        self.block_id = block_id
        self.client_id = client_id

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.I32:
                    self.block_id = iprot.readI32()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.I64:
                    self.client_id = iprot.readI64()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('register_client_id_args')
        if self.block_id is not None:
            oprot.writeFieldBegin('block_id', TType.I32, 1)
            oprot.writeI32(self.block_id)
            oprot.writeFieldEnd()
        if self.client_id is not None:
            oprot.writeFieldBegin('client_id', TType.I64, 2)
            oprot.writeI64(self.client_id)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(register_client_id_args)
register_client_id_args.thrift_spec = (
    None,  # 0
    (1, TType.I32, 'block_id', None, None, ),  # 1
    (2, TType.I64, 'client_id', None, None, ),  # 2
)


class register_client_id_result(object):


    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('register_client_id_result')
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(register_client_id_result)
register_client_id_result.thrift_spec = (
)


class command_request_args(object):
    """
    Attributes:
     - seq
     - block_id
     - cmd_id
     - arguments
    """


    def __init__(self, seq=None, block_id=None, cmd_id=None, arguments=None,):
        self.seq = seq
        self.block_id = block_id
        self.cmd_id = cmd_id
        self.arguments = arguments

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, [self.__class__, self.thrift_spec])
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRUCT:
                    self.seq = sequence_id()
                    self.seq.read(iprot)
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.I32:
                    self.block_id = iprot.readI32()
                else:
                    iprot.skip(ftype)
            elif fid == 3:
                if ftype == TType.I32:
                    self.cmd_id = iprot.readI32()
                else:
                    iprot.skip(ftype)
            elif fid == 4:
                if ftype == TType.LIST:
                    self.arguments = []
                    (_etype3, _size0) = iprot.readListBegin()
                    for _i4 in range(_size0):
                        _elem5 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                        self.arguments.append(_elem5)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, [self.__class__, self.thrift_spec]))
            return
        oprot.writeStructBegin('command_request_args')
        if self.seq is not None:
            oprot.writeFieldBegin('seq', TType.STRUCT, 1)
            self.seq.write(oprot)
            oprot.writeFieldEnd()
        if self.block_id is not None:
            oprot.writeFieldBegin('block_id', TType.I32, 2)
            oprot.writeI32(self.block_id)
            oprot.writeFieldEnd()
        if self.cmd_id is not None:
            oprot.writeFieldBegin('cmd_id', TType.I32, 3)
            oprot.writeI32(self.cmd_id)
            oprot.writeFieldEnd()
        if self.arguments is not None:
            oprot.writeFieldBegin('arguments', TType.LIST, 4)
            oprot.writeListBegin(TType.STRING, len(self.arguments))
            for iter6 in self.arguments:
                oprot.writeString(iter6.encode('utf-8') if sys.version_info[0] == 2 else iter6)
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
all_structs.append(command_request_args)
command_request_args.thrift_spec = (
    None,  # 0
    (1, TType.STRUCT, 'seq', [sequence_id, None], None, ),  # 1
    (2, TType.I32, 'block_id', None, None, ),  # 2
    (3, TType.I32, 'cmd_id', None, None, ),  # 3
    (4, TType.LIST, 'arguments', (TType.STRING, 'UTF8', False), None, ),  # 4
)
fix_spec(all_structs)
del all_structs

