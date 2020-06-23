class Node(object):
    def __init__(self, mark): # Only file cache will use param 'offset' 
        self.mark = mark
        self.next = None
        self.prev = None

class LinkedList(object):
    def __init__(self, max_length):
        self.head = None
        self.tail = None
        self.length = 0
        self.max_length = self.max_length

    def length(self):
        return self.length
    
    def max_length(self):
        return self.max_length

    def insert_head(self, node):
        if self.head != None:
            node.next = self.head
            self.head.prev = node
            self.head = node
        else:
            self.head = node
            self.tail = node
        self.length += 1
    
    def move_to_head(self, mark):
        cur_node = self.head
        temp_node = cur_node
        while cur_node != None:
            if cur_node.mark == mark:
                if cur_node == self.head:
                    break
                elif cur_node == self.tail:
                    temp_node.next = cur_node.next
                    self.tail = temp_node
                    cur_node.next = self.head
                    self.head.prev = cur_node
                    cur_node.prev = None
                    self.head = cur_node
                    break
                else:
                    temp_node.next = cur_node.next
                    cur_node.next.prev = temp_node
                    cur_node.next = self.head
                    self.head.prev = cur_node
                    cur_node.prev = None
                    self.head = cur_node
                    break
            temp_node = cur_node
            cur_node = cur_node.next
    
    def pop(self):
        if self.length == 0:
            return 
        elif self.length == 1:
            self.head = None
            self.tail = None
            self.length = 0
        else:
            self.tail.prev.next = None
            self.tail = self.tail.prev
            self.length -= 1
    
    def remove(self, mark):
        cur_node = self.head
        temp_node = cur_node
        while cur_node != None:
            if cur_node.mark == mark:
                if cur_node == self.head:
                    self.head = cur_node.next
                    self.head.prev = None
                    break
                elif cur_node == self.tail:
                    temp_node.next = None
                    break
                else:
                    temp_node.next = cur_node.next
                    cur_node.next.prev = temp_node
                    break
            temp_node = cur_node
            cur_node = cur_node.next
        self.length -= 1


class DataStructureCache(object):
    def __init__(self, max_length):
        self.list = LinkedList(max_length)
        self.capacity = max_length

    def isEmpty(self):
        return self.size() == 0
    
    def isFull(self):
        return self.size() == self.capacity

    def size(self):
        return self.list.length

    def capacity(self):
        return self.capacity


class HashTableCache(DataStructureCache):
    def __init__(self, max_length, prefetch_size): 
        super(HashTableCache,self).__init__(max_length)
        # Prefetch size means the number of prefetched key-value pairs
        self.prefetch_size = prefetch_size 
        self.table = dict()
    
    def insert(self, data):
        self.table[data[0]] = data[1]
        new_node = Node(data[0])
        self.list.insert_head(new_node)
    
    def delete(self, key):
        value = self.table[key]
        self.list.remove(key)
        del self.table[key]

    def evict(self):
        del self.table[self.list.tail.mark]
        self.list.pop()
    
    def exists(self, key):
        return key in self.table

    def get(self, key):
        return self.table[key]

    def update(self, key, new_value):
        self.table[key] = new_value

    def hit_handling(self, key):
        self.list.move_to_head(key)

    def miss_handling(self, raw_data):
        prefetch_data_length = len(raw_data)
        prefetch_data = raw_data[::-1]
        for i in range(0, min(prefetch_data_length, 2 * self.prefetch_size),2):
            new_value = prefetch_data[i]
            new_key = prefetch_data[i+1]
            if self.exists(new_key):
                if self.get(new_key) == new_value:
                    self.hit_handling(new_key)
                else:
                    self.update(new_key,new_value)
                    self.hit_handling(new_key)
            else:
                if self.isFull():
                    self.evict()
                self.insert([new_key,new_value])
    
    def print_out(self): # for debug use
        cur_node = self.list.head
        while cur_node:
            print(cur_node.mark,self.table[cur_node.mark])
            cur_node = cur_node.next


class FileCache(DataStructureCache):
    def __init__(self, max_length, block_size, prefetch_block_num):
        super(FileCache,self).__init__(max_length)
        self.block_size = block_size
        self.prefetch_block_num = prefetch_block_num
        self.table = dict()
    
    def insert(self, offset, data):
        self.table[offset] = data
        new_node = Node(offset)
        self.list.insert_head(new_node)

    def evict(self):
        del self.table[self.list.tail.mark]
        self.list.pop()
    
    def exists(self, cur_offset):
        start_offset = (cur_offset // self.block_size) * self.block_size
        if start_offset in self.table:
            return True
        return False

    def read_handling(self, cur_offset, data):
        start_offset = (cur_offset // self.block_size) * self.block_size
        end_offset = start_offset + ((len(data) - 1) // self.block_size) * self.block_size
        data_index = 0
        for offset in range(start_offset, end_offset + 1, self.block_size):
            if offset in self.table:
                self.table[start_offset] = data[data_index : (data_index + min(self.block_size, len(data) - data_index))]
                self.list.move_to_head(offset)
            else:
                if self.isFull():
                    self.evict()
                self.insert(offset, data[data_index : (data_index + min(self.block_size, len(data) - data_index))])
            data_index += self.block_size

    def write_handling(self, cur_offset, data):
        start_offset = (cur_offset // self.block_size) * self.block_size
        if start_offset in self.table:
            self.table[start_offset] = data
            self.list.move_to_head(start_offset)
        else:
            if self.isFull():
                self.evict()
            self.insert(start_offset, data)
        
    def hit_handling(self,cur_offset, read_size):
        start_offset = (cur_offset // self.block_size) * self.block_size
        return self.table[start_offset][(cur_offset - start_offset) : min(read_size + cur_offset - start_offset, len(self.table[start_offset]))]
        
    def print_out(self): # for debug use
        cur_node = self.list.head
        while cur_node:
            print(cur_node.mark,self.table[cur_node.mark])
            cur_node = cur_node.next
        
    
    
