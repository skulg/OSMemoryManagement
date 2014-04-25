#include "physicalmemory.h"

PhysicalMemory::PhysicalMemory(QString str, uint nb_frames):TObject(str),mNbFrames(nb_frames),mNextEmptyFrame(0)
{
    mFrames = new Frame[nb_frames];
    mFirstInIndex = 0 ;
    fifo=false;
}

PhysicalMemory::~PhysicalMemory()
{
    delete []mFrames;
}

uint PhysicalMemory::nbFrames()const
{
    return mNbFrames;
}

void PhysicalMemory::read(uint frame_number, uint offset, char * data)
{
    //TP2_IFT2245_TO_DO
    this->mFrames[frame_number].read(offset ,data);

    //TP2_IFT2245_END_TO_DO
}

void PhysicalMemory::write(uint frame_number, uint offset, char * data)
{
    //TP2_IFT2245_TO_DO
    this->mFrames[frame_number].write(offset,data);
    this->mFrames[frame_number].setModified();
    //TP2_IFT2245_END_TO_DO
}

uint PhysicalMemory::insertFrameInNextFreeSpace(uint page_number, QByteArray *frame_bytes)
{
    //TP2_IFT2245_TO_DO
    //If there is an empty frame
    if(this->hasEmptyFrame()){
        uint frameToInsert = this->mNextEmptyFrame;
        this->insertFrame(frameToInsert, page_number , frame_bytes);
        this->mNextEmptyFrame++;
        return frameToInsert;
    }else{ if (this->fifo){

            //FIFO

            int pageToRemove    = this->mFrames[this->mFirstInIndex].pageNumber();
            if(this->mFrames[pageToRemove].isModified())
            {
                this->mHardDrive->write(pageToRemove, this->mFrames[pageToRemove].frameData());
            }
            this->mPageTable->setInvalid(pageToRemove);
            this->insertFrame(this->mFirstInIndex, page_number , frame_bytes);
            uint resultIndex =mFirstInIndex;
            this->mFirstInIndex= this->mFirstInIndex + 1 % this->nbFrames();
            return resultIndex;
        }else{
            //Less Frequently used
            int leastFrequentlyAccessNb=mPageTable->getPagenbAccess(this->mFrames[0].mPageNumber);
            uint leastFrequentlyFrameIndex =0 ;
            for (int i =0 ;i<this->nbFrames();i++){
                int currentPageNbAccess =mPageTable->getPagenbAccess(this->mFrames[i].mPageNumber);
                if (currentPageNbAccess<leastFrequentlyAccessNb){
                    leastFrequentlyFrameIndex=i;
                }
            }
            int pageToRemove   = this->mFrames[leastFrequentlyFrameIndex].pageNumber();
            if(this->mFrames[pageToRemove].isModified())
            {
                this->mHardDrive->write(pageToRemove, this->mFrames[pageToRemove].frameData());
            }
            this->mPageTable->setInvalid(pageToRemove);
            this->insertFrame(leastFrequentlyFrameIndex,page_number,frame_bytes);
            return leastFrequentlyFrameIndex;
        }

    }

    //TP2_IFT2245_END_TO_DO
}

void PhysicalMemory::insertFrame(uint frame_number, uint page_number, QByteArray *frame_bytes)
{
    //TP2_IFT2245_TO_DO
    //The frame is not empty but the user should already
    //have saved the data.
    this->mFrames[frame_number].setFrameData(frame_bytes);
    this->mFrames[frame_number].setPageNumber(page_number);

    //TP2_IFT2245_END_TO_DO
}

bool PhysicalMemory::hasEmptyFrame() const
{
    return (mNextEmptyFrame<mNbFrames);
}

QByteArray * PhysicalMemory::frame(uint frame_number)
{
    return mFrames[frame_number].frameData();
}

uint PhysicalMemory::pageNumber(uint frame_number)
{
    return mFrames[frame_number].pageNumber();
}

void PhysicalMemory::setModified(uint frame_number)
{
    mFrames[frame_number].setModified();
}

void PhysicalMemory::setUnModified(uint frame_number)
{
    mFrames[frame_number].setUnModified();
}

bool PhysicalMemory::isFrameModified(uint frame_number)
{
    return mFrames[frame_number].isModified();
}

void PhysicalMemory::setPageTable(PageTable *thePageTable)
{
    this->mPageTable    = thePageTable;
}

void PhysicalMemory::setHardDrive(HardDrive *theHardDrive)
{
    this->mHardDrive    = theHardDrive;
}
