#ifndef LINKEDLISTTEST_H
#define LINKEDLISTTEST_H

#include <qlinkedlist.h>
#include <iostream>
#include <qstring.h>

class LinkedListTest
{
public:
    typedef struct TLB_entry{
        int pageNumber;
        int frameNumber;
        TLB_entry(int pNum = -1, int fNum = -1)
            :pageNumber(pNum), frameNumber(fNum){}
                            } TLB_entry;
public:
    LinkedListTest();


private:
    QLinkedList<TLB_entry> qll;
    QLinkedList<TLB_entry>::iterator qllIterator;
    void printList();
    QString findItem(int pageNumber);
};

#endif // LINKEDLISTTEST_H
