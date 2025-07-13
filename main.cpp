#include <iostream>
using namespace std;

template<typename T>
class MempoolNode {
    public:
        char* data;
        MempoolNode* next;
        MempoolNode* bFree;
        MempoolNode(){
            data = new char[sizeof(T)];
        }
        
};


template <typename T>
class Mempool {
public:
	int nodes;
	int size;
	char* raw_mem;
	int prev;
	MempoolNode<T>* head=nullptr;
	MempoolNode<T>* freeNodes=nullptr;
	Mempool(int nodes_) {
	    nodes = nodes_;
	    for(int i = 0; i< nodes_; i++){
	        cout << " creating node " << i << endl;
	        MempoolNode<T>* new_node = new MempoolNode<T>();
	        new_node->next = head;
	        new_node->bFree = head;
	        head = new_node;
	    }
	    freeNodes=head;
	}
	void Check_For_Size() {
	    cout << "Checking no of linkedlist elements" << endl;
	    MempoolNode<T>* temp = head;
    	while(temp){
    	    temp=temp->next;
    	    cout << " going ... " << endl;
    	}
	}
	void* allocate(){
	    if(!freeNodes){
	        cout << "Cannot allocate aanymore" << endl;
	        return nullptr;
	    }
	    void* part =  freeNodes->data;
	    freeNodes = freeNodes->bFree;
	    cout << "allocation sucess!" << endl;
	    return part;
	}
	void deallocate(T* ptr){
	    MempoolNode<T>* temp = (MempoolNode<T>*)ptr;
	    	    cout << "deallocation starts!" << endl;
	    (temp)->bFree = freeNodes;
	    freeNodes=temp;
	    cout << freeNodes << endl;
	    cout << "deallocation sucess!" << endl;
	   // return freeNodes
	}


};


class OrderResponse {
public:
	int ts;
	int sucess_status;
	OrderResponse(int ts_, int status_) {
		ts = ts_;
		sucess_status = status_;
	}
	void print(){
	    cout << " Timestamp " << ts << " sucess_status " << sucess_status << endl;
	}
};
int main() {
    Mempool<OrderResponse> temp = Mempool<OrderResponse>(2);
    temp.Check_For_Size();

    OrderResponse* OR1 = new (temp.allocate()) OrderResponse(1, 1);
    OrderResponse* OR2 = new (temp.allocate()) OrderResponse(2, 2);
    OR1->print();
    OR2->print();

    temp.deallocate(OR1);

    OrderResponse* OR3 = new (temp.allocate()) OrderResponse(3, 3);
    OR3->print();

    // Optional cleanup
    OR2->~OrderResponse();
    OR3->~OrderResponse();

    return 0;
}
