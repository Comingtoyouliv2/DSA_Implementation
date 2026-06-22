#include <iostream>
#include <cassert>
#include <stdexcept>

// Circular 
class CircularQueue {
private:
    int* data;
    int capacity;    // 할당된 배열 칸 수
    int count;       // 현재 들어있는 원소 개수
    int head;        // 맨 앞 원소의 인덱스 (dequeue 위치)
    int rear;        // 다음에 넣을 빈 자리의 인덱스 (enqueue 위치)

    // [요구사항] capacity 를 newCap 으로 키우고, 원소들을 "논리 순서대로" 옮긴다.
    //   - 주의!! circular 라서 단순히 data[i] 를 그대로 복사하면 순서가 깨진다.
    //   - head 부터 count 개를, 새 배열의 0번부터 펴서 복사해야 함.
    //     (예: head=3, capacity=4, 원소 [d, _, b, c] 논리순서 c->d 이런 식으로 꼬여있음)
    //   - 복사 후 head=0, tail=count 로 재설
    //   - 옛 배열 delete[] 잊지 말 것. (복사 끝난 뒤에!)

    // 재복사에 관해 필요한 것이 무엇일까? 
    void resize(int newCap) {
        int* newData;
        newData = new int[newCap]; 

        // 1 2 3 4 5
        // 결국에는 head 부터 rear 까지 원소를 new arr 에 집어넣어야 한다. 
        // head rear / rear head 이렇게 구분할 필요는 없다.
        // (head + 1) % capacity, (rear + 1) % capacity; 여기서 보고 느끼는게 없나? 
        for(int i = 0; i < count; i++) {
            newData[i] = data[(head + i) % capacity];
        }
        
        // After resizing
        head = 0; // 이러면 맨 앞에 배정
        rear = count; // 이러면 맨 뒤에 다시 배정
        capacity = newCap;

        delete[] data;
        data = newData;
    }

public:
    // --------------------------------------------------------
    // 생성자 / 소멸자
    // --------------------------------------------------------

    // [요구사항] 초기 capacity 할당, head/tail/count 초기화. (예: cap=4)
    //   - cap=0 으로 시작하면 2배 함정 + % 0 (나누기 0) 터지니 주의
    CircularQueue() {
        capacity = 1;
        data = new int[capacity]; // 이렇게 배열도 처음 capacity 로 initiation 해줘야 한다. 
        head = 0;
        rear = 0;
        count = 0;
    }

    // [요구사항] 배열 메모리 해제. (delete[])
    ~CircularQueue() {
        delete[] data;
    }

    // --------------------------------------------------------
    // 삽입 / 삭제 (FIFO)
    // --------------------------------------------------------

    // [요구사항] 맨 뒤에 추가한다. (enqueue)
    //   - 꽉 찼으면(count == capacity) resize(capacity * 2) 먼저
    //   - data[tail] 에 넣고, tail = (tail + 1) % capacity 로 전진
    void push(int value) {
        if (count == capacity) resize(2*capacity);
        data[rear] = value;
        rear = (rear+1) % capacity;
        count++;
    }

    // [요구사항] 맨 앞 원소를 제거한다. (dequeue)
    //   - 비어있으면 throw std::out_of_range
    //   - head = (head + 1) % capacity 로 전진
    //   - count 감소
    //   - (값을 지울 필요는 없음. 인덱스만 옮기면 논리적으로 제거됨)
    void pop() {
        if (count == 0){
            throw std::out_of_range("there is no element to delete");
        } else {
            head = (head+1) % capacity;
            count--;
        }
    }

    // --------------------------------------------------------
    // 접근 / 조회
    // --------------------------------------------------------

    // [요구사항] 맨 앞 원소를 반환한다. (제거 안 함)
    //   - 비어있으면 throw
    int front() const {
        if (count == 0) {
            throw std::out_of_range("There is no element inside the circular queue");
        }
        return data[head];
    }

    // [요구사항] 맨 뒤(가장 최근에 넣은) 원소를 반환한다. (제거 안 함)
    //   - 비어있으면 throw
    //   - tail 은 "다음 빈 자리"라서, 마지막 원소는 tail 바로 직전.
    //     (tail - 1) 이 음수로 갈 수 있으니 % 로 안전하게 감싸야 함 → 함정!
    int back() const {
        if (count == 0) {
            throw std::out_of_range("There is no element inside the circular queue");
        }
        return data[(rear - 1 + capacity) % capacity]; // if rear = 0 -> rear - 1 이 음수로 간다 그래서 한 바퀴 더 돌리고 % 로 표준화한다.
    }

    // [요구사항] 현재 원소 개수.
    int size() const {
        return count;
    }

    // [요구사항] 비어있으면 true.
    bool empty() const {
        return count == 0;
    }

    // --------------------------------------------------------
    // 유틸리티
    // --------------------------------------------------------

    // [요구사항] front -> back 논리 순서로 출력. 디버깅용.
    //   - head 부터 count 개를, i 를 (head + k) % capacity 로 순회하며 찍기
    //   - head, tail, count, capacity 도 같이 찍으면 wrap-around 추적에 매우 좋음
    void print() const {
        for (int i = 0; i < count;  i++) {
            std::cout << data[(head + i) % capacity];
        }
        std::cout << "Front index is : " << head << '\n';
        std::cout << "Rear index is : " << rear << '\n';
        std::cout << "Element number is : " << count << '\n';
        std::cout << "Current capacity is : " << capacity << '\n';
        std::cout << '\n';
    }
};


int main() {
    CircularQueue q;

    // --- 1. 초기 상태 ---
    assert(q.empty() == true);
    assert(q.size() == 0);
    std::cout << "[1] empty/size OK\n";

    // --- 2. push / front / back / size (FIFO) ---
    q.push(10);
    q.push(20);
    q.push(30);
    assert(q.empty() == false);
    assert(q.size() == 3);
    assert(q.front() == 10);   // 처음 넣은 게 front
    assert(q.back() == 30);    // 마지막에 넣은 게 back
    std::cout << "[2] push/front/back/size OK\n";

    // --- 3. pop (FIFO 순서) ---
    q.pop();                   // 10 제거
    assert(q.front() == 20);
    assert(q.size() == 2);
    std::cout << "[3] pop OK\n";

    // --- 4. wrap-around 유발 (핵심!) ---
    //   초기 cap=4 가정. 지금 head=1, tail=3, count=2 인 상태 ([_, 20, 30, _]).
    //   여기서 2개 더 넣으면 tail 이 배열 끝을 넘어 0으로 wrap 돼야 함.
    q.push(40);                // tail=3 자리에 넣고 tail -> 0 으로 wrap
    q.push(50);                // tail=0 자리에 넣고 tail -> 1
    assert(q.size() == 4);
    assert(q.front() == 20);
    assert(q.back() == 50);
    std::cout << "[4] wrap-around OK\n";

    // --- 5. resize 유발 (꽉 찬 상태에서 push) ---
    //   지금 cap=4, count=4 로 꽉 참. 게다가 데이터가 wrap 돼서
    //   물리적으로 [50, 20, 30, 40] 처럼 흩어져 있음 (head=1).
    //   resize 가 이걸 논리순서 [20, 30, 40, 50] 로 펴서 복사해야 통과.
    q.push(60);                // 여기서 resize 발생
    assert(q.size() == 5);
    assert(q.front() == 20);   // resize 후에도 순서 유지돼야 함
    assert(q.back() == 60);
    std::cout << "[5] resize (순서 보존) OK\n";

    // --- 6. FIFO 순서대로 전부 빼기 ---
    int expected[] = {20, 30, 40, 50, 60};
    for (int i = 0; i < 5; i++) {
        assert(q.front() == expected[i]);
        q.pop();
    }
    assert(q.empty() == true);
    assert(q.size() == 0);
    std::cout << "[6] FIFO drain OK\n";

    // --- 7. 빈 상태 예외 (pop / front / back) ---
    bool threw = false;
    try { q.pop(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);

    threw = false;
    try { q.front(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);

    threw = false;
    try { q.back(); } catch (const std::out_of_range&) { threw = true; }
    assert(threw == true);
    std::cout << "[7] empty-exception OK\n";

    // --- 8. 비웠다가 재사용 (head/tail 이 0 아닌 데서 다시 시작) ---
    q.push(99);
    q.push(88);
    assert(q.size() == 2);
    assert(q.front() == 99);
    assert(q.back() == 88);
    std::cout << "[8] reuse OK\n";

    // --- 9. 대량 push/pop 으로 wrap 여러 바퀴 돌리기 ---
    //   인덱스가 배열을 여러 번 돌아도 망가지지 않는지 스트레스 테스트.
    CircularQueue q2;
    for (int round = 0; round < 100; round++) {
        q2.push(round);
        assert(q2.front() == round);   // 넣자마자 바로 빼니 항상 front == round
        q2.pop();
        assert(q2.empty() == true);
    }
    std::cout << "[9] wrap stress OK\n";

    std::cout << "\n모든 Circular Queue 테스트 통과!\n";
    return 0;
}