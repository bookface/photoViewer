//-*-c++-*-
//-*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#pragma once

#include <QList>
#include <QDebug>

template <typename T>
class RoundRobinList {

  public:
    RoundRobinList(int maxSize = 50) : _maxSize(maxSize), _index(0) {}
    ~RoundRobinList() { _list.clear(); }

    void addItem(const T& item) {
        if (_list.size() >= _maxSize) {
            _list.removeFirst();  // Remove oldest item
        }
        _list.append(item);             // Add new item to end
        _index = _list.size() - 1;      // move to end of list
    }

    T getNext() {
        if (_list.isEmpty()) {
            return T();                 // Return default if empty
        }
        _index = (_index + 1) % _list.size();
        return _list[_index];
    }

    T getPrevious() {
        if (_list.isEmpty()) {
            return T();                 // Return default if empty
        }

        _index = (_index - 1 + _list.size()) % _list.size();
        return _list[_index];
    }

    int size() const {
        return _list.size();
    }

private:
    QList<T> _list;
    int _maxSize;
    int _index;
};

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#ifdef TESTING_ROUND_ROBIN
int main() {
    RoundRobinList<QString> rrList(3);  // Fixed size of 3

    rrList.addItem("A");
    rrList.addItem("B");
    rrList.addItem("C");

    qDebug() << "Next:" << rrList.getNext();  // Output: B
    qDebug() << "Next:" << rrList.getNext();  // Output: C
    qDebug() << "Previous:" << rrList.getPrevious();  // Output: B
    qDebug() << "Previous:" << rrList.getPrevious();  // Output: A

    rrList.addItem("D");  // A is removed, list = [B, C, D]
    rrList.addItem("E");  // B is removed, list = [C, D, E]

    qDebug() << "Next:" << rrList.getNext();  // Output: D
    qDebug() << "Next:" << rrList.getNext();  // Output: E
    qDebug() << "Previous:" << rrList.getPrevious();  // Output: D

    return 0;
}
#endif
