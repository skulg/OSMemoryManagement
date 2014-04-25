#include "pagetable.h"

PageTable::PageTable(QString str,uint nb_pages):TObject(str),mNbPages(nb_pages)
{
    mPages = new Page[nb_pages];
}

PageTable::~PageTable()
{
    delete []mPages;
}

uint PageTable::nbPages()const
{
    return mNbPages;
}

//NOTE ADDED page_number to signature to match comments descriptions of @params.
void PageTable::insertPage(uint page_number,const Page &page)
{
    //TP2_IFT2245_TO_DO
    //BEGAN NOT TESTED
    this->mPages[page_number]=page;
    //TP2_IFT2245_END_TO_DO
}

bool PageTable::frameIndex(uint page_number, int& frame_index)
{
    //TP2_IFT2245_TO_DO
    //BEGAN NOT TESTED
    if (this->mPages[page_number].isValid()){
        this->mPages[page_number].nbAccess++;
        frame_index=this->mPages[page_number].mFrameIndex;
        return true;
    }
    return false;
    //TP2_IFT2245_END_TO_DO
}

void PageTable::setInvalid(uint page_number)
{
    mPages[page_number].setInvalid();
}

int PageTable::getPagenbAccess(uint page_number){
    return mPages[page_number].nbAccess;
}
