#include "tlb.h"

TLB::TLB(QString str):TObject(str)
{
    MAXIMUM_NUMBER_OF_ENTRIES=16;
    REPLACEMENT_STRATEGY_FIFO=0;
    REPLACEMENT_STRATEGY_LRU=1;

    REPLACEMENT_STRATEGY    = REPLACEMENT_STRATEGY_FIFO;

    _previousEntry = 0;
}

TLB::~TLB()
{
}

void TLB::addTLBEntry(TLB::TLB_entry new_tlb_entry)
{
    // First, let's look if the list contains the maximum number of entries.
    // If so, let's delete the entry at the end of the list.
    if(MAXIMUM_NUMBER_OF_ENTRIES == _TlbList.size())
    {
        _TlbList.removeLast();
    }

    // Now let's add the entry to the beginning of the list.
    _TlbList.prepend(new_tlb_entry);
}

void TLB::removeEntryFromTLB(int page_number)
{
    _TlbListIterator = _TlbList.begin();

    while(_TlbListIterator != _TlbList.end())
    {
        if((*_TlbListIterator).pageNumber == page_number)
        {
            _TlbListIterator = _TlbList.erase(_TlbListIterator);

            return;
        }

        ++_TlbListIterator;
    }
}

bool TLB::findPage(int page_number, int& frame_index)
{
    _TlbListIterator = _TlbList.begin();

    while(_TlbListIterator != _TlbList.end())
    {
        if((*_TlbListIterator).pageNumber == page_number)
        {
            frame_index = (*_TlbListIterator).frameNumber;

            if(REPLACEMENT_STRATEGY == REPLACEMENT_STRATEGY_LRU)
            {
                TLB_entry copy  = (*_TlbListIterator);
                _TlbListIterator = _TlbList.erase(_TlbListIterator);
                _TlbList.prepend(copy);
            }

            return true;
        }

        ++_TlbListIterator;
    }

    return false;
}

void TLB::printList()
{
    cout << "Printing list..." << endl;

    _TlbListIterator = _TlbList.begin();

    while(_TlbListIterator != _TlbList.end())
    {
        cout << (*_TlbListIterator).pageNumber << endl;

        ++_TlbListIterator;
    }
}


