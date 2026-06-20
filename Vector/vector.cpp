#include <iostream>
#include <cassert>
#include <stdexcept>

// 항상 using namespace std; 사용 안하고 있다는 것 알기 

class Vector {
private:
    int* v;       // 동적 배열 (heap)
    int sz;          // 현재 원소 개수 (size)
    int cap;         // 할당된 칸 수 (capacity)

    // [요구사항] cap 을 newCap 으로 키운다. (재할당의 핵심)
    //   vector reallocate 되는 순서를 알자: - 새 배열 할당 → 기존 sz 개 복사 → 옛 배열 delete[] → 포인터/cap 교체
    void reallocate(int newCap) {
        int* newV = new int[newCap];

        // 1. pointer 자체만으로 순회하려고 하지말고, 주변 변수를 활용해라
        for (int i = 0; i < sz; i++) {
            newV[i] = v[i];
        }

        delete[] v; //node 와 해제하는 구문이 다르다. delete[] 로 arr 해제해야한다. 
        v = newV;
        cap = newCap;
    }

public:
    Vector() {
        sz = 0;
        cap = 1;
        v = new int[cap]; // int(cap) 는 v[0] 에 cap 하나 넣는거고, int[cap] 는 int 에 cap 개 짜리 배열 생성하는것
    }

    // [요구사항] 배열 메모리 해제. (delete[])
    ~Vector() {
        delete[] v;
    }

    // --------------------------------------------------------
    // 삽입 / 삭제
    // --------------------------------------------------------

    // [요구사항] 맨 뒤에 값 추가.
    // 항상 떠올릴 것 1. Edge Case 2. Pointer
    void push_back(int value) {
        if (sz >= cap) {
            reallocate(2*cap); 
            v[sz] = value; // 이런식으로 할당하면 될듯? 
        } else {
            v[sz] = value;
        }
        sz++; // 다 했으면 한 칸 넣어주기
    }

    // [요구사항] 맨 뒤 원소 제거.
    // 무엇을 놓쳤을까
    // vector 자체는 남아있는 상태 
    // size == 0 일떄,
    // size == 1 일떄,
    // size == n 일떄, 
    void pop_back() {
        if (sz == 0) {
            throw std::out_of_range("there is no element");
        } else {
            sz--; // 이건 통과, edge case 를 잘 정리하지 못했다. 
        }
    }

    // --------------------------------------------------------
    // 접근 / 조회
    // --------------------------------------------------------

    // [요구사항] index 위치 값 반환. (0-based)
    int at(int index) const {
        if (index < 0 || index >= sz || sz == 0) {
            throw std::out_of_range("index out of range");
        }
        return v[index];
    }

    // [요구사항] index 위치 값을 newValue 로 수정.
    void set(int index, int newValue) {
        if (index < 0 || index >= sz) {
            throw std::out_of_range("index out of range");
        }
        v[index] = newValue;
    }

    // [요구사항] 맨 뒤 원소 반환 (제거 안 함).
    int back() const {
        if (sz == 0) {
            throw std::out_of_range("there is no element");
        }
        return v[sz-1];
    }

    // [요구사항] 현재 원소 개수.
    int size() const {
        return sz;
    }

    // [요구사항] 현재 할당된 칸 수. (디버깅/검증용으로 중요)
    int capacity() const {
        return cap;
    }

    // [요구사항] 비어있으면 true.
    bool empty() const {
        return sz == 0;
    }

    // [요구사항] [a, b, c] 형태로 출력. sz, cap 도 같이 찍으면 추적에 좋음.
    void print() const {
        for (int i = 0; i < sz; i++){
            std::cout << "[ ";
            std::cout << v[i] << ' ';
            std::cout << " ]";
        }
        std::cout << "size=" << sz << " cap=" << cap << '\n';
    }
};


int main() {
    Vector v;

    // --- 1. 초기 상태 ---
    assert(v.empty() == true);
    assert(v.size() == 0);
    assert(v.capacity() >= 1);     // 0이면 안 됨 (2배 함정)
    std::cout << "[1] init OK\n";

    // --- 2. push_back / size / back ---
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    assert(v.empty() == false);
    assert(v.size() == 3);
    assert(v.back() == 30);
    assert(v.at(0) == 10);
    assert(v.at(1) == 20);
    assert(v.at(2) == 30);
    std::cout << "[2] push_back/at/back OK\n";

    // --- 3. capacity 성장 확인 (amortized 핵심) ---
    //   초기 cap=1 가정하면, 여기까지 오며 1→2→4 로 커졌어야 함.
    //   size 는 정확해야 하고, capacity 는 size 이상이어야 함.
    assert(v.capacity() >= v.size());
    int capBefore = v.capacity();
    while (v.size() < capBefore) v.push_back(0);   // 딱 채우기
    assert(v.capacity() == capBefore);             // 아직 재할당 전
    v.push_back(99);                               // 이 push 가 재할당 유발
    assert(v.capacity() > capBefore);              // 커졌어야 함
    assert(v.capacity() >= v.size());
    std::cout << "[3] capacity growth OK\n";

    // --- 4. set 으로 값 수정 ---
    v.set(0, 111);
    assert(v.at(0) == 111);
    std::cout << "[4] set OK\n";

    // --- 5. pop_back ---
    int sizeBefore = v.size();
    int lastVal = v.back();
    (void)lastVal;
    v.pop_back();
    assert(v.size() == sizeBefore - 1);
    std::cout << "[5] pop_back OK\n";

    // --- 6. 전부 비우기 (경계) ---
    while (!v.empty()) v.pop_back();
    assert(v.size() == 0);
    assert(v.empty() == true);
    std::cout << "[6] drain OK\n";

    // --- 7. 빈 상태 예외 (pop_back / at / back) ---
    // nothing inside here. 
    v.print();
    
    bool threw = false;
    try { v.pop_back(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);

    threw = false;
    try { v.back(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);

    threw = false;
    try { v.at(0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);
    std::cout << "[7] empty-exception OK\n";

    // --- 8. 비웠다가 재사용 ---
    v.push_back(7);
    v.push_back(8);
    assert(v.size() == 2);
    assert(v.at(0) == 7);
    assert(v.back() == 8);
    std::cout << "[8] reuse OK\n";

    std::cout << "\n모든 Vector 테스트 통과!\n";
    return 0;
}