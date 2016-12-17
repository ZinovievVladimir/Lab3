#ifndef INCLUDE_STACK_H_
#define INCLUDE_STACK_H_
#include <stdexcept>

template <typename T>

struct TNode{
  T data;
  TNode* next;
};


template <typename T>
class Stack{
  TNode<T>* top;
 public:
  Stack() {
    top = 0;
  }
  ~Stack();
  void push(T);
  T& gettop();
  bool pop();
  bool empty();
};

#endif  // INCLUDE_STACK_H_

template <typename T>
Stack<T>::~Stack() {
  while (top != 0) {
    TNode<T>* buf = top->next;
    delete top;
    top = buf;
  }
}

template <typename T>
void Stack<T>::push(T x) {
  TNode<T>* node = new TNode<T>;
  node->data = x;
  node->next = top;
  top = node;
}

template <typename T>
T& Stack<T>::gettop() {
  if (top != 0)
    return top->data;
  else
    throw std::logic_error("Stack is empty!");
}

template <typename T>
bool Stack<T>::pop() {
  if (top != 0) {
    TNode<T>* s = top;
    top = top->next;
    delete s;
    return true;
  }
  else {
    return false;
  }
}

template <typename T>
bool Stack<T>::empty() {
  return !top;
}