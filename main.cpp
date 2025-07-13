/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

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
	void deallocate(void* ptr){
	    	    cout << "deallocation starts!" << endl;

	    MempoolNode<T>* part = freeNodes;
	    ((MempoolNode<T>*)ptr)->bFree = part;
	    freeNodes=((MempoolNode<T>*)ptr);
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
		status_ = status_;
	}
	void print(){
	    cout << " Timestamp " << ts << " sucess_status " << sucess_status << endl;
	}
};

int main()
{
	Mempool<OrderResponse> temp = Mempool<OrderResponse>(2);
	temp.Check_For_Size();
	OrderResponse* OR1 = (OrderResponse*)temp.allocate();
	OR1->ts = 1;
	OR1->sucess_status = 1;
	OrderResponse* OR2 = (OrderResponse*)temp.allocate();
	OR2->ts = 2;
	OR2->sucess_status = 2;
	temp.deallocate(OR1);
	OrderResponse* OR3 = (OrderResponse*)temp.allocate();
	OR3->ts = 3;
	OR3->sucess_status = 3;
// 	OrderResponse* OR4 = (OrderResponse*)temp.allocate();
// 	OR4->ts = 3;
// 	OR4->sucess_status = 3;

// 	OR1->print();
	OR2->print();
	OR3->print();
// 	OR4->print();
	return 0;
}