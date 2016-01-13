
// LRU cache implementation

// An entry is a variable length heap-allocated structure.  Entries
// are kept in a circular doubly linked list ordered by access time.
// 双向链表的节点
// 一个 antry 从堆上分配空间出来 使用deleter删除 在访问的时候是一个环形的双向链表 引用计数来判断什么时候删除这块堆上内存
// 一个LRUHandle保存了key的值 从堆空间分配一块内存出来 使用引用计数来判断什么时候 释放这块内存

struct LRUHandle 
{
  void* value; 
  void (*deleter)(const string&, void* value);
  LRUHandle* next_hash; // 用于hash bucket的单向链表
  LRUHandle* next;        
  LRUHandle* prev;
  size_t charge;      // TODO(opt): Only allow uint32_t?
  size_t key_length;   
  uint32_t refs;      
  uint32_t hash;      // Hash of key();  
  char key_data[1];  
  
  string key() const {  
    if (next == this) {
      return *(reinterpret_cast<string*>(value));
    } 
	else {
      return string(key_data, key_length);
    }
  }
};

// 这里的hashtable 不负责释放保存指针指向的内存块
class HandleTable {
private:
 // The table consists of an array of buckets where each bucket is
 // a linked list of cache entries that hash into the bucket.
 uint32_t length_;  
 uint32_t elems_;   
 LRUHandle** list_; 

public:
  HandleTable() : length_(0), elems_(0), list_(NULL) { Resize(); }
  ~HandleTable() { delete[] list_; }

   LRUHandle* Lookup(const string& key, uint32_t hash) {
    return *FindPointer(key, hash);
  }
  LRUHandle* Insert(LRUHandle* h);  
  LRUHandle* Remove(const string& key, uint32_t hash);

private:
  LRUHandle** FindPointer(const string& key, uint32_t hash);
  void Resize();  
}; 

// 哈希表的实现 据说随机读比g++内置的高%5的效率
// 冲突时使用了链表来解决 这个hashtable 并不赋值释放这些节点的空间 仅仅是保存了这些节点的指针
 // 在对应的桶链表中 找到可以用的NULL节点(最后一个)或者重复的节点(替换掉) 这里二级指针的使用
LRUHandle* HandleTable::Insert(LRUHandle* h)
{
    LRUHandle** ptr = FindPointer(h->key(), h->hash); //
    LRUHandle* old = *ptr;
    h->next_hash = (old == NULL ? NULL : old->next_hash);
    *ptr = h;
    if (old == NULL) {
      ++elems_;
      if (elems_ > length_) {
        Resize();
      }
    }
    return old;
}
  // 这里依旧是二级指针来删除 而不是使用prev节点 对于二级指针的理解 可以看成一个*表示类型void* *p实际上就是这个链表对应的某个节点直接改变它的值就行了 而不用prev节点
LRUHandle* HandleTable::Remove(const string& key, uint32_t hash) {
    LRUHandle** ptr = FindPointer(key, hash);
    LRUHandle* result = *ptr;
    if (result != NULL) {
      *ptr = result->next_hash;
      --elems_;
    }
    return result;
}

  // Return a pointer to slot that points to a cache entry that
  // matches key/hash.  If there is no such cache entry, return a
  // pointer to the trailing slot in the corresponding linked list.
  // 返回指向slot的指针 该slot指向对应的entry(key-value)
  // 如果没有匹配的尾指针
LRUHandle** HandleTable::FindPointer(const string& key, uint32_t hash) 
{
    LRUHandle** ptr = &list_[hash & (length_ - 1)]; // 这里桶的计算方法都是 [hash & (length_ - 1)]
    while (*ptr != NULL &&
           ((*ptr)->hash != hash || key != (*ptr)->key())) {
      ptr = &(*ptr)->next_hash;
    }
    return ptr;
}
  
// 2倍大小重新设置大小
void HandleTable::Resize() 
{
    uint32_t new_length = 4; // 也就是默认是4
    while (new_length < elems_) {
      new_length *= 2;
    }
    LRUHandle** new_list = new LRUHandle*[new_length]; // malloc(length*sizeof(LRUHandle*))
    memset(new_list, 0, sizeof(new_list[0]) * new_length);
    uint32_t count = 0;
  
    for (uint32_t i = 0; i < length_; i++) // 重新hash一次执行完毕
  {
      LRUHandle* h = list_[i]; // 一个bucket 实际上就是一个链表
      while (h != NULL)
      {
        LRUHandle* next = h->next_hash;
        uint32_t hash = h->hash; 
        LRUHandle** ptr = &new_list[hash & (new_length - 1)]; // 重新计算hash值
        h->next_hash = *ptr;  // 放到新bucket的开头，*ptr为指针数组的一个指针值
        *ptr = h;
        h = next;
        count++;
      }
    }
    assert(elems_ == count);
    delete[] list_; // 删除旧的hash
    list_ = new_list; // 新的hash
    length_ = new_length;
}
