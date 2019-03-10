//#include <iostream>
class Node {
    public:
        int* data;
        Node* next = nullptr;
        Node* prev = nullptr;
        Node():data(new int[2]){data[0]=data[1]=0;}
        Node(int*_data):data(_data){}
        ~Node(){if (data!=nullptr) delete[] data;}
};

class LinkedList {
        Node sntl;
        Node* head;

    public:
        LinkedList() : sntl(nullptr), head(&sntl) {
            sntl.next = &sntl;
            sntl.prev = &sntl;
        }

        virtual ~LinkedList() {}

        virtual void Insert(Node* nu) {
            nu->next = head;
            nu->prev = head->prev;
            head->prev = nu;
            sntl.next = head = nu;
        }

        int* Extract() {
            int* temp = head->data;
            head = head->next;
            head->prev = &sntl;
            return temp;
        }
        
        int* Read() {
            return head->data;
        }

        void Next() {
            head = head->next;
        }  

        // remove node from linked list. assumes ~Node()
        //  will delete the removed node when it goes out
        //  of scope.
        void Delete(Node* nu) {
            nu->prev->next = nu->next;
            nu->next->prev = nu->prev;
            //delete nu;
        }

        // Remove sentinel from linked list, connected the 
        //  end to the beginning.
        void Ring() {
            sntl.prev->next = sntl.next;
            sntl.next->prev = sntl.prev;
        }

};

int Max(Node* nu) {
    return (nu->data[0] > nu->data[1])? nu->data[0]: nu->data[1];
}

class TrimmableLinkedList: public LinkedList {
    public:
        Node*** lists;
        int N;

        TrimmableLinkedList(int _N)
            : LinkedList()
              , lists(new Node**[_N])
              , N(_N) {

                for (int i=0; i<N; ++i)
                    lists[i] = new Node*[2*i+1];
        }

        virtual ~TrimmableLinkedList() {
            for (int i=0; i<N; ++i)
                delete[] lists[i];
            delete[] lists;
        }
    
        TrimmableLinkedList(TrimmableLinkedList& other)
            : lists (new Node**[other.N])
            , N (other.N) {
            for (int i=0; i<N; ++i) {
                lists[i] = new Node*[2*i+1];
                for (int j=0; j<2*i+1; ++j)
                    lists[i][j] = other.lists[i][j];
            }
        }

        virtual void Insert(Node* nu) {
            LinkedList::Insert(nu);
            const int& i = nu->data[0];
            const int& j = nu->data[1];
            if (i > j) {
                lists[i][j] = nu;
            }
            else {
                lists[j][j+i] = nu;
            }
        }

        void Trim(int new_N) {
            while (N>new_N) {
                for (int i=0; i<2*N-1; ++i)
                    Delete(lists[N-1][i]);
                --N;
            }
        }
};
