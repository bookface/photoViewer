//-*-c++-*-
//-*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#pragma once

#include <QList>
#include <QDebug>

template <typename T>
class RoundRobinList {

  public:
    explicit RoundRobinList(int maxSize=20) : maxSize(maxSize), index(0) {}

    void addItem(const T& item) {
        if (list.size() >= maxSize) {
            list.removeFirst();  // Remove oldest item
        }
        list.append(item);  // Add new item
        if (index >= list.size()) index = list.size() - 1; // Adjust index if needed
    }

    T getNext() {
        if (list.isEmpty()) return T(); // Return default if empty

        index = (index + 1) % list.size();
        return list[index];
    }

    T getPrevious() {
        if (list.isEmpty()) return T(); // Return default if empty

        index = (index - 1 + list.size()) % list.size();
        return list[index];
    }

    int size() const {
        return list.size();
    }

private:
    QList<T> list;
    int maxSize;
    int index;
};

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
