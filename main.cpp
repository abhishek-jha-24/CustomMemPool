#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

template<typename T>
class MempoolNode {
public:
    T data;
    MempoolNode* next;
    MempoolNode* bFree;
    std::atomic<int> smartCount;

    MempoolNode() : next(nullptr), bFree(nullptr), smartCount(1) {
        cout << "Creating node..." << endl;
    }

    ~MempoolNode() {
        cout << "Deallocating node..." << endl;
        next = nullptr;
        bFree = nullptr;
    }
};

class SpinLock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

template <typename T>
class Mempool {
private:
    int totalNodes;
    MempoolNode<T>* head = nullptr;
    MempoolNode<T>* freeNodes = nullptr;
    SpinLock spinLock;

public:
    Mempool(int count) : totalNodes(count) {
        for (int i = 0; i < totalNodes; i++) {
            auto* newNode = new MempoolNode<T>();
            newNode->next = head;
            newNode->bFree = head;
            head = newNode;
        }
        freeNodes = head;
    }

    ~Mempool() {
        MempoolNode<T>* current = head;
        while (current) {
            current->data.~T();
            MempoolNode<T>* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void checkSize() {
        cout << "Checking linked list size..." << endl;
        MempoolNode<T>* temp = head;
        while (temp) {
            cout << " Node active..." << endl;
            temp = temp->next;
        }
    }

    T* allocate() {
        spinLock.lock();
        if (!freeNodes) {
            spinLock.unlock();
            cout << "No more nodes available." << endl;
            return nullptr;
        }

        MempoolNode<T>* node = freeNodes;
        freeNodes = freeNodes->bFree;
        node->smartCount.store(1);
        spinLock.unlock();

        cout << "Allocation successful!" << endl;
        return &(node->data);
    }

    void share(T* ptr) {
        auto* node = reinterpret_cast<MempoolNode<T>*>(ptr);
        node->smartCount.fetch_add(1);
        cout << "Sharing node. Smart count: " << node->smartCount.load() << endl;
    }

    void deallocate(T* ptr) {
        auto* node = reinterpret_cast<MempoolNode<T>*>(ptr);
        int count = node->smartCount.fetch_sub(1) - 1;

        if (count == 0) {
            spinLock.lock();
            node->bFree = freeNodes;
            freeNodes = node;
            spinLock.unlock();
            cout << "Deallocation successful." << endl;
        } else {
            cout << "Node not deallocated. Smart count: " << count << endl;
        }
    }
};

class OrderResponse {
public:
    int ts;
    int successStatus;

    OrderResponse() = default;

    OrderResponse(int ts_, int status_) : ts(ts_), successStatus(status_) {}

    void print() const {
        cout << "Timestamp: " << ts << ", Success Status: " << successStatus << endl;
    }

    ~OrderResponse() {
        cout << "Destructed OrderResponse" << endl;
    }
};

int main() {
    Mempool<OrderResponse> pool(5);
    pool.checkSize();

    OrderResponse* or1 = new (pool.allocate()) OrderResponse(1, 1);
    OrderResponse* or2 = new (pool.allocate()) OrderResponse(2, 2);

    or1->print();
    or2->print();

    pool.share(or1); // simulate sharing with another thread

    pool.deallocate(or1); // ref count goes from 2 to 1, not deallocated
    pool.deallocate(or1); // ref count 0, deallocated

    OrderResponse* or3 = new (pool.allocate()) OrderResponse(3, 3);
    or3->print();

    pool.deallocate(or2);
    pool.deallocate(or3);

    return 0;
}
