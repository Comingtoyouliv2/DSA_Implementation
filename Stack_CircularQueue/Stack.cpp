#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

class Stack {
private:
    std::vector<int> st;   // 내부 저장은 vector 에 위임

public:
    Stack() {}

    // [요구사항] 맨 위에 추가. (vector 의 뒤쪽에 push)
    // stack 도 예외처리를 하는가?
    void push(int value) {
        // 이미 vector 가 init 되어 있기 때문에, 그냥 넣어주기만 하면 될듯
        st.push_back(value);
    }

    // [요구사항] 맨 위 제거.
    // 
    void pop() {
        if (st.size() == 0) {
            throw std::out_of_range("There is no element to delete");
        } else {
            st.pop_back();
        }
    }

    // --------------------------------------------------------
    // 접근 / 조회
    // --------------------------------------------------------

    // [요구사항] 맨 위 원소 반환 (제거 안 함).
    //   
    int top() const {
        if (st.size() == 0) {
            throw std::out_of_range("There is no element to delete");
        } else {
            return st[st.size()-1];
        }
    }

    // [요구사항] 현재 원소 개수.
    int size() const {
        return st.size(); // 이 size 는 vector 의 size function 이다. 
    }

    // [요구사항] 비어있으면 true.
    bool empty() const {
        return st.size() == 0;
    }

    // --------------------------------------------------------
    // 유틸리티
    // --------------------------------------------------------

    // [요구사항] 바닥 → 위 순서로 출력. 디버깅용.
    void print() const {
        for(size_t i = 0; i < st.size(); i++) {
            std::cout << st[i] << ' ';
        }
    }
};


int main() {
    Stack s;

    // --- 1. 초기 상태 ---
    assert(s.empty() == true);
    assert(s.size() == 0);
    std::cout << "[1] empty/size OK\n";

    // --- 2. push / top / size ---
    s.push(10);
    s.push(20);
    s.push(30);
    assert(s.empty() == false);
    assert(s.size() == 3);
    assert(s.top() == 30);       // LIFO: 마지막에 넣은 게 top
    std::cout << "[2] push/top/size OK\n";

    // --- 3. pop (LIFO 순서 확인) ---
    s.pop();                     // 30 제거
    assert(s.top() == 20);
    assert(s.size() == 2);
    s.pop();                     // 20 제거
    assert(s.top() == 10);
    assert(s.size() == 1);
    std::cout << "[3] pop OK\n";

    // --- 4. 전부 비우기 (경계) ---
    s.pop();                     // 10 제거
    assert(s.empty() == true);
    assert(s.size() == 0);
    std::cout << "[4] empty-edge OK\n";

    // --- 5. 빈 상태에서 예외 ---
    bool threw = false;
    try {
        s.pop();                 // 빈 스택 pop → 예외 기대
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw == true);

    threw = false;
    try {
        s.top();                 // 빈 스택 top → 예외 기대
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw == true);
    std::cout << "[5] empty-exception OK\n";

    // --- 6. 비웠다가 재사용 ---
    s.push(99);
    s.push(88);
    assert(s.size() == 2);
    assert(s.top() == 88);
    std::cout << "[6] reuse OK\n";

    std::cout << "\n모든 Stack 테스트 통과!\n";
    return 0;
}