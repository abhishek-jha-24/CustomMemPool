#include <iostream>
using namespace std;

template<typename T>
class MemoryPoolNode {
public:
    T data;
    MemoryPoolNode* next;
    MemoryPoolNode* nextFree;

    MemoryPoolNode() {
        cout << "Creating node..." << endl;
    }

    ~MemoryPoolNode() {
        cout << "Deallocating node..." << endl;
        next = nullptr;
        nextFree = nullptr;
    }
};

template <typename T>
class MemoryPool {
private:
    int totalNodes;
    MemoryPoolNode<T>* poolHead = nullptr;
    MemoryPoolNode<T>* freeListHead = nullptr;

public:
    MemoryPool(int totalNodes_) : totalNodes(totalNodes_) {
        for (int i = 0; i < totalNodes; i++) {
            cout << "Creating node " << i << endl;
            MemoryPoolNode<T>* newNode = new MemoryPoolNode<T>();
            newNode->next = poolHead;
            newNode->nextFree = poolHead;
            poolHead = newNode;
        }
        freeListHead = poolHead;
    }

    void checkSize() {
        cout << "Checking number of linked list elements..." << endl;
        MemoryPoolNode<T>* current = poolHead;
        while (current) {
            cout << "Walking through list..." << endl;
            current = current->next;
        }
    }

    T* allocate() {
        if (!freeListHead) {
            cout << "No free nodes available for allocation." << endl;
            return nullptr;
        }

        MemoryPoolNode<T>* allocatedNode = freeListHead;
        freeListHead = freeListHead->nextFree;
        cout << "Allocation successful!" << endl;
        return &(allocatedNode->data);
    }

    void deallocate(T* ptr) {
        MemoryPoolNode<T>* nodeToFree = (MemoryPoolNode<T>*) ptr;

        cout << "Starting deallocation..." << endl;
        nodeToFree->nextFree = freeListHead;
        freeListHead = nodeToFree;
        cout << "Deallocation successful!" << endl;
    }

    ~MemoryPool() {
        cout << "Destroying memory pool..." << endl;
        MemoryPoolNode<T>* current = poolHead;
        MemoryPoolNode<T>* temp;

        while (current) {
            current->data.~T();  // Manually call destructor for contained objects
            temp = current;
            current = current->next;
            delete temp;
        }
    }
};

class OrderResponse {
public:
    int timestamp;
    int successStatus;

    OrderResponse() = default;

    OrderResponse(int ts, int status) {
        timestamp = ts;
        successStatus = status;
    }

    void print() const {
        cout << "Timestamp: " << timestamp << ", Success Status: " << successStatus << endl;
    }

    ~OrderResponse() {
        cout << "Destroyed OrderResponse" << endl;
    }
};

int main() {
    MemoryPool<OrderResponse> pool(28);
    pool.checkSize();

    OrderResponse* or1 = new (pool.allocate()) OrderResponse(1, 1);
    OrderResponse* or2 = new (pool.allocate()) OrderResponse(2, 2);
    or1->print();
    or2->print();

    pool.deallocate(or1);

    OrderResponse* or3 = new (pool.allocate()) OrderResponse(3, 3);
    or2->print();
    or3->print();

    return 0;
}
