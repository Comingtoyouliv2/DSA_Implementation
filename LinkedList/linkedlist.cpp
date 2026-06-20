#include <iostream>
#include <cassert>
#include <stdexcept> 

// using namespace std;

class LinkedList {
private:    
    struct Node {
        Node* next;
        int data;

        Node(int val) : next{nullptr}, data{val} {}
    };

    Node* head;
    Node* tail;
    int count; // 노드의 현재 갯수 

public:
    // --------------------------------------------------------
    // 생성자 / 소멸자
    // --------------------------------------------------------

    LinkedList() : head{nullptr}, tail{nullptr}, count{0} {}

    // [요구사항] 모든 노드의 메모리를 해제한다. (clear() 재사용 가능)
    ~LinkedList() {
        clear(); // ******** 내부에 있는 function 은 어떻게 사용하는걸까... 
    }

    // --------------------------------------------------------
    // 삽입 (Insertion)
    // --------------------------------------------------------

    // [요구사항] 맨 앞에 새 노드를 추가한다.
    void push_front(int value) {
        Node* newNode = new Node(value);

        if(count == 0) {
            tail = newNode;
            head = newNode;
            count++;
        } else {
            newNode->next = head;
            head = newNode;
            count++;
        }
    }

    // [요구사항] 맨 뒤에 새 노드를 추가한다.
    void push_back(int value) {
        // head 를 갱신 
        Node* newNode = new Node(value);

        // Edge case 를 어떻게 잡을 것인가?
        if (count == 0) {
            tail = newNode;
            head = newNode;
            count++;
        } else { 
            tail->next = newNode;
            tail = newNode;
            count++;
        }
    
    }

    // [요구사항] index 위치에 새 노드를 삽입한다. (0-based)
    void insert(int index, int value) {
        // 맨 마지막에는 넣을 수 있게 해줘야 할듯? 
        if (index < 0 || index > count) {
            std::out_of_range("Index is out of the range of the linked list");
        }
        
        Node* newNode = new Node(value);
        Node* curr = head;
        Node* dummy = nullptr;
        int tempIndex = index;
        // prev->next = curr; 이런식으로 하면 nullptr 은 아무것도 없는데 연결한거라서 터진다.
    

        if (index == 0) {
            push_front(value);
        } else if (index == count) {
            push_back(value);
        } else {
            // 6번째에 넣고 싶다? 5 뒤에 연결
            //       curr    target
            //         1 ->   2 -> 3 -> 4 -> 5
            // 이러면 target index 에 도착 
            while (tempIndex != 1) {
                curr = curr->next;
                tempIndex--;
            }
            dummy = curr->next; 
            curr->next = newNode;
            newNode->next = dummy;
            count++;
        }
    }

    // --------------------------------------------------------
    // 삭제 (Deletion)
    // --------------------------------------------------------

    // [요구사항] 맨 앞 노드를 제거한다.
    void pop_front() {
        // TODO
        Node* curr = head->next;
        delete head;
        head = curr;
        count--;
    }

    // [요구사항] 맨 뒤 노드를 제거한다.
    void pop_back() {
        Node* curr = head;
         
        // 아무것도 없으면, 아무것도 하면 안된다.
        if (curr == nullptr) {
            return;
        } else if (count == 1) {
            delete head; 
            count--;
        } else {
            //1개만 있을때, 여러개 있을떄 ... 
            //  (head, tail)
            //        1 
            while(curr->next != tail) {
                curr = curr->next;
            }
            delete tail;
            tail = curr;
            tail->next = nullptr;
        
            count--;
        }
    }

    // [요구사항] index 위치의 노드를 제거한다. (0-based)
    void erase(int index) {
        // 딱 한군데만 제거한다. 
        if (index < 0 || index > count) {
            std::out_of_range("Index is out of range");
        }

        if (index == 0) {
            pop_front();
        } else if (index == count) {
            pop_back();
        } else {
            Node* curr = head;
            Node* target = nullptr;
            int tempIndex = index;

            while (tempIndex != 1) {
                curr = curr->next;
                tempIndex--;
            }
            // curr target        
            // 1->   2    -> 3

            target = curr->next;
            curr->next = target->next;
            delete target;
            count--;
        }
    }

    // [요구사항] 모든 노드를 제거하고 빈 리스트로 만든다.
    void clear() {
        Node* curr = head;
        Node* dummy = curr;
  
        // dummy curr
        //  1    2       3      4.     5

        while (curr != nullptr) {
            curr = curr->next;
            delete dummy;
            count--;
            dummy = curr;
        }
    }

    // --------------------------------------------------------
    // 접근 / 조회 (Access / Query)
    // --------------------------------------------------------

    // [요구사항] index 위치의 값을 반환한다. (0-based)
    int get(int index) const {
        if (index >= count || index < 0) {
            throw std::out_of_range("index must be fall inside the linked list index");
        } 

        Node* curr = head; 
        
        while (index != 0) {
            curr = curr->next;
            index--;
        }

        return curr->data;
    }

    // [요구사항] value 가 처음 나오는 위치(index)를 반환한다.
    //   - 없으면 -1 반환
    int indexOf(int value) const {
        int index = 0;
        Node* curr = head;
        
        while (curr != nullptr) {
            if(curr->data == value) {
                return index;
            }
            curr = curr->next;
            index++;
        }

        return -1;
    }

    // [요구사항] value 가 리스트에 존재하는지 여부를 반환한다.
    //   - indexOf 를 재사용해도 됨
    bool contains(int value) const {
        if (indexOf(value) == -1) {
            return false;
        } 
        return true;
    }

    // [요구사항] 현재 원소 개수를 반환한다. (count 그대로 반환, O(1))
    int size() const {
        return count;
    }

    // [요구사항] 리스트가 비어있으면 true 를 반환한다.
    bool empty() const {
        return count== 0;
    }

    // --------------------------------------------------------
    // 유틸리티 (Utility)
    // --------------------------------------------------------

    // [요구사항] 리스트 전체를 출력한다. 예) [10 -> 20 -> 30]
    //   - 디버깅용. 형식은 자유롭게.
    void print() const {
        Node* curr = head;

        while (curr != nullptr) {
            std::cout << curr->data << ' ';
            std::cout << '\n';
            curr = curr->next;
        }
    }
}; 



int main() {
    LinkedList list;

    // --- 1. 초기 상태 ---
    assert(list.empty() == true);
    assert(list.size() == 0);
    std::cout << "[1] empty/size OK\n";

    // --- 2. push_back / push_front + get ---
    list.push_back(10);     // [10]
    list.push_back(20);     // [10 -> 20]
    list.push_back(30);     // [10 -> 20 -> 30]
    list.push_front(5);     // [5 -> 10 -> 20 -> 30]
    assert(list.size() == 4);
    assert(list.empty() == false);
    assert(list.get(0) == 5);
    assert(list.get(1) == 10);
    assert(list.get(2) == 20);
    assert(list.get(3) == 30);
    std::cout << "[2] push_back/push_front/get OK\n";

    // --- 3. insert (앞 / 중간 / 맨 뒤) ---
    list.insert(0, 1);      // 맨 앞:   [1 -> 5 -> 10 -> 20 -> 30]
    list.insert(2, 7);      // 중간:    [1 -> 5 -> 7 -> 10 -> 20 -> 30]
    list.insert(6, 99);     // 맨 뒤:   [1 -> 5 -> 7 -> 10 -> 20 -> 30 -> 99]
    assert(list.size() == 7);
    assert(list.get(0) == 1);
    assert(list.get(2) == 7);
    assert(list.get(6) == 99);

    std::cout << "[3] insert OK\n";

    // --- 4. indexOf / contains ---
    assert(list.indexOf(1)  == 0);       // 맨 앞
    assert(list.indexOf(7)  == 2);       // 중간
    assert(list.indexOf(99) == 6);       // 맨 뒤
    assert(list.indexOf(404) == -1);     // 없는 값
    assert(list.contains(10)   == true);
    assert(list.contains(1000) == false);
    std::cout << "[4] indexOf/contains OK\n";

    // --- 5. pop_front / pop_back ---
    list.pop_front();       // [5 -> 7 -> 10 -> 20 -> 30 -> 99]
    list.pop_back();        // [5 -> 7 -> 10 -> 20 -> 30]
    assert(list.size() == 5);
    assert(list.get(0) == 5);
    assert(list.get(4) == 30);
    std::cout << "[5] pop_front/pop_back OK\n";

    // --- 6. erase (앞 / 중간 / 맨 뒤) ---
    list.erase(0);          // 앞:     [7 -> 10 -> 20 -> 30]
    list.erase(2);          // 중간:   [7 -> 10 -> 30]
    list.erase(2);          // 맨 뒤:  [7 -> 10]
    assert(list.size() == 2);
    assert(list.get(0) == 7);
    assert(list.get(1) == 10);
    std::cout << "[6] erase OK\n";



    // --- 7. 경계 케이스: 원소 1개까지 줄였다가 비우기 ---
    list.pop_front();       // [10]
    assert(list.size() == 1);
    assert(list.get(0) == 10);
    // -------------
    list.pop_back();        // []
    assert(list.size() == 0);
    assert(list.empty() == true);
    std::cout << "[7] single-element edge OK\n";

    // --- 8. clear 후 재사용 ---
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.print();
    list.clear();           // []
    assert(list.empty() == true);
    assert(list.size() == 0);
    list.push_back(42);     // clear 이후에도 정상 동작하는지
    assert(list.size() == 1);
    assert(list.get(0) == 42);

    std::cout << "[8] clear/reuse OK\n";
    std::cout << "\n모든 테스트 통과!\n";
    return 0;
}