import os
import subprocess
import sys
import time
from Queue import Empty
from unittest import TestCase

from thrift.transport import TTransport, TSocket

from elasticmem import ElasticMemClient, RemoveMode
from elasticmem.subscription.subscriber import Notification


def wait_till_server_ready(host, port):
    check = True
    while check:
        try:
            transport = TTransport.TBufferedTransport(TSocket.TSocket(host, port))
            transport.open()
            transport.close()
            check = False
        except TTransport.TTransportException:
            time.sleep(0.1)


class TestClient(TestCase):
    DIRECTORY_SERVER_EXECUTABLE = os.getenv('DIRECTORY_SERVER_EXEC', 'directoryd')
    STORAGE_SERVER_EXECUTABLE = os.getenv('STORAGE_SERVER_EXEC', 'storaged')
    STORAGE_HOST = '127.0.0.1'
    STORAGE_SERVICE_PORT = 9093
    STORAGE_MANAGEMENT_PORT = 9094
    STORAGE_NOTIFICATION_PORT = 9095
    STORAGE_CHAIN_PORT = 9096
    DIRECTORY_HOST = '127.0.0.1'
    DIRECTORY_SERVICE_PORT = 9090
    DIRECTORY_LEASE_PORT = 9091

    def start_servers(self):
        try:
            self.directoryd = subprocess.Popen([self.DIRECTORY_SERVER_EXECUTABLE])
        except OSError as e:
            print "Error running executable %s: %s" % (self.DIRECTORY_SERVER_EXECUTABLE, e)
            sys.exit()

        wait_till_server_ready(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT)
        wait_till_server_ready(self.DIRECTORY_HOST, self.DIRECTORY_LEASE_PORT)

        try:
            self.storaged = subprocess.Popen([self.STORAGE_SERVER_EXECUTABLE])
        except OSError as e:
            print "Error running executable %s: %s" % (self.STORAGE_SERVER_EXECUTABLE, e)
            sys.exit()

        wait_till_server_ready(self.STORAGE_HOST, self.STORAGE_SERVICE_PORT)
        wait_till_server_ready(self.STORAGE_HOST, self.STORAGE_MANAGEMENT_PORT)
        wait_till_server_ready(self.STORAGE_HOST, self.STORAGE_NOTIFICATION_PORT)
        wait_till_server_ready(self.STORAGE_HOST, self.STORAGE_CHAIN_PORT)

    def stop_servers(self):
        self.directoryd.kill()
        self.directoryd.wait()
        self.storaged.kill()
        self.storaged.wait()

    def kv_ops(self, kv):
        # Test get/put
        for i in range(0, 1000):
            self.assertTrue(kv.put(str(i), str(i)) == "ok")

        for i in range(0, 1000):
            self.assertTrue(kv.get(str(i)) == str(i))

        for i in range(1000, 2000):
            self.assertTrue(kv.get(str(i)) == "key_not_found")

        # Test update
        for i in range(0, 1000):
            self.assertTrue(kv.update(str(i), str(i + 1000)) == "ok")

        for i in range(1000, 2000):
            self.assertTrue(kv.update(str(i), str(i + 1000)) == "key_not_found")

        for i in range(0, 1000):
            self.assertTrue(kv.get(str(i)) == str(i + 1000))

        # Test remove
        for i in range(0, 1000):
            self.assertTrue(kv.remove(str(i)) == "ok")

        for i in range(1000, 2000):
            self.assertTrue(kv.remove(str(i)) == "key_not_found")

        for i in range(0, 1000):
            self.assertTrue(kv.get(str(i)) == "key_not_found")

    def test_lease_worker(self):
        self.start_servers()
        client = ElasticMemClient(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT, self.DIRECTORY_LEASE_PORT)
        try:
            client.create_scope("/a/file.txt", "/tmp")
            self.assertTrue(client.fs.exists("/a/file.txt"))
            time.sleep(client.lease_worker.renewal_duration_s)
            self.assertTrue(client.fs.exists("/a/file.txt"))
            time.sleep(client.lease_worker.renewal_duration_s)
            self.assertTrue(client.fs.exists("/a/file.txt"))
        except:
            self.stop_servers()
            client.close()
            raise

        client.close()
        self.stop_servers()

    def test_create_scope(self):
        self.start_servers()
        client = ElasticMemClient(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT, self.DIRECTORY_LEASE_PORT)
        try:
            self.kv_ops(client.create_scope("/a/file.txt", "/tmp"))
            self.assertTrue(client.fs.exists('/a/file.txt'))
        except:
            self.stop_servers()
            client.close()
            raise

        client.close()
        self.stop_servers()

    def test_get_scope(self):
        self.start_servers()
        client = ElasticMemClient(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT, self.DIRECTORY_LEASE_PORT)
        try:
            client.create_scope("/a/file.txt", "/tmp")
            self.assertTrue(client.fs.exists('/a/file.txt'))
            self.kv_ops(client.get_scope('/a/file.txt'))
        except:
            self.stop_servers()
            client.close()
            raise

        client.close()
        self.stop_servers()

    def test_destroy_scope(self):
        self.start_servers()
        client = ElasticMemClient(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT, self.DIRECTORY_LEASE_PORT)
        try:
            client.create_scope("/a/file.txt", "/tmp")
            self.assertTrue('/a/file.txt' in client.to_renew)
            client.destroy_scope('/a/file.txt', RemoveMode.flush)
            self.assertFalse('/a/file.txt' in client.to_renew)
            self.assertTrue('/a/file.txt' in client.to_flush)
            client.destroy_scope('/a/file.txt', RemoveMode.delete)
            self.assertFalse('/a/file.txt' in client.to_renew)
            self.assertTrue('/a/file.txt' in client.to_remove)
        except:
            self.stop_servers()
            client.close()
            raise

        client.close()
        self.stop_servers()

    def test_notifications(self):
        self.start_servers()
        client = ElasticMemClient(self.DIRECTORY_HOST, self.DIRECTORY_SERVICE_PORT, self.DIRECTORY_LEASE_PORT)
        try:
            client.fs.create("/a/file.txt", "/tmp")

            n1 = client.notifications("/a/file.txt")
            n2 = client.notifications("/a/file.txt")
            n3 = client.notifications("/a/file.txt")

            n1.subscribe(['put'])
            n2.subscribe(['put', 'remove'])
            n3.subscribe(['remove'])

            kv = client.get_scope("/a/file.txt")
            kv.put('key1', 'value1')
            kv.remove('key1')

            self.assertTrue(n1.get_notification() == Notification('put', 'key1'))
            self.assertTrue(n2.get_notification() == Notification('put', 'key1'))
            self.assertTrue(n2.get_notification() == Notification('remove', 'key1'))
            self.assertTrue(n3.get_notification() == Notification('remove', 'key1'))

            with self.assertRaises(Empty):
                n1.get_notification(block=False)
            with self.assertRaises(Empty):
                n2.get_notification(block=False)
            with self.assertRaises(Empty):
                n3.get_notification(block=False)

            n1.unsubscribe(['put'])
            n2.unsubscribe(['remove'])

            kv.put('key1', 'value1')
            kv.remove('key1')

            self.assertTrue(n2.get_notification() == Notification('put', 'key1'))
            self.assertTrue(n3.get_notification() == Notification('remove', 'key1'))

            with self.assertRaises(Empty):
                n1.get_notification(block=False)
            with self.assertRaises(Empty):
                n2.get_notification(block=False)
            with self.assertRaises(Empty):
                n3.get_notification(block=False)

            n1.close()
            n2.close()
            n3.close()
        except:
            self.stop_servers()
            client.close()
            raise

        client.close()
        self.stop_servers()
