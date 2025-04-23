#pragma once

const int MAX = 30; // キュー配列の最大サイズ

class StackAndQue
{
private:
    int st[MAX]; // スタックを表す配列
    int top = 0; // スタックの先頭を表すポインタ
    // スタックを初期化する
    void st_init() {
        top = 0; // スタックポインタを初期位置に
    }

    // スタックが空かどうかを判定する
    bool st_isEmpty() {
        return (top == 0); // スタックサイズが 0 かどうか
    }

    // スタックが満杯かどうかを判定する
    bool st_isFull() {
        return (top == MAX); // スタックサイズが MAX かどうか
    }

    int qu[MAX]; // キューを表す配列
    int tail = 0, head = 0; // キューの要素区間を表す変数

    // キューを初期化する
    void init() {
        head = tail = 0;
    }

    // キューが空かどうかを判定する
    bool isEmpty() {
        return (head == tail);
    }

    // スタックが満杯かどうかを判定する
    bool isFull() {
        return (head == (tail + 1) % MAX);
    }
public:
    StackAndQue();
    // push (top を進めて要素を格納)
    void push(int v) {
        if (st_isFull()) {
            //cout << "error: stack is full." << endl;
            return;
        }
        st[top++] = v; // st[top] = v; と top++; をまとめてこのように表せます
    }

    // pop (top をデクリメントして、top の位置にある要素を返す)
    int pop() {
        if (st_isEmpty()) {
            //cout << "error: stack is empty." << endl;
            return -1;
        }
        return st[--top]; // --top; と return st[top]; をまとめてこのように表せます
    }

    // enqueue (tail に要素を格納してインクリメント)
    void enqueue(int v) {
        if (isFull()) {
            //cout << "error: queue is full." << endl;
            return;
        }
        qu[tail++] = v;
        if (tail == MAX) tail = 0; // リングバッファの終端に来たら 0 に
    }

    // dequeue (head にある要素を返して head をインクリメント)
    int dequeue() {
        if (isEmpty()) {
            //cout << "error: stack is empty." << endl;
            return -1;
        }
        int res = qu[head];
        ++head;
        if (head == MAX) head = 0;
        return res;
    }
};

StackAndQue::StackAndQue()
{
    init();
    st_init();
}
