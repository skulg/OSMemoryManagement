#include "linkedlisttest.h"

using namespace std;

LinkedListTest::LinkedListTest()
{
    for(int p = 0; p < 16; ++p)
    {
        TLB_entry test(p, p+256);
        qll.prepend(test);
    }
    //TLB_entry test(1,1);
    //qll.prepend(test);
    //TLB_entry test1(2,2);
    //qll.prepend(test1);
    findItem(8);

    printList();
}

void LinkedListTest::printList()
{
    cout << "Printing list" << endl;

    for(qllIterator = qll.begin(); qllIterator != qll.end(); ++qllIterator)
    {
        cout << " " << (*qllIterator).pageNumber << " :: " << (*qllIterator).frameNumber << endl;
    }
}

QString LinkedListTest::findItem(int pageNumber)
{
    cout << "Searching for " << pageNumber << endl;
    bool found  = false;

    qllIterator = qll.begin();

    while(qllIterator != qll.end())
    {
        if((*qllIterator).pageNumber == pageNumber)
        {
            found   = true;

            cout << "Found " << (*qllIterator).pageNumber << " : " << (*qllIterator).frameNumber << endl;

            if(qllIterator != qll.begin())
            {
                cout << "-- not found at beginning" << endl;

                TLB_entry copy  = *qllIterator;

                //QString copy    = *qllIterator;

                qllIterator = qll.erase(qllIterator);

                qll.prepend(copy);

                //return copy;
            }
        }

        ++qllIterator;
    }

    /*for(qllIterator = qll.begin(); qllIterator != qll.end(); ++qllIterator)
    {
        cout << "Scanning items, current = " << (*qllIterator).toStdString() << endl;

        if((*qllIterator) == zeItem)
        {
            found   = true;

            cout << "Found " << zeItem.toStdString() << endl;

            if(qllIterator != qll.begin())
            {
                //QString copyOfItem = *qllIterator;
                qllIterator = qll.erase(qllIterator);

                cout << "-- not found at beginning" << endl;
            }

            return *qllIterator;
        }
    }*/

    /*if(false == found)
    {
        cout << "will add " << zeItem.toStdString() << " to list" << endl;
        qll.prepend(zeItem);
    }*/

    return NULL;
    //return zeItem;
}

