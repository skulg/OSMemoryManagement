#include "physicalmemory.h"

PhysicalMemory::PhysicalMemory(QString str, uint nb_frames):TObject(str),mNbFrames(nb_frames),mNextEmptyFrame(0)
{
    mFrames = new Frame[nb_frames];
    mFirstInIndex = 0 ;
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
    }else{
        //FIFO

        // Let's get the page number to remove from physical memory.
        int pageToRemove    = this->mFrames[this->mFirstInIndex].pageNumber();

        // Let's check if the frame to be removed was modified. If so, let's write it to the disk.
        if(this->mFrames[pageToRemove].isModified())
        {
            this->mHardDrive->write(pageToRemove, this->mFrames[pageToRemove].frameData());
        }

        // Now marking the page be removed as invalid.
        this->mPageTable->setInvalid(pageToRemove);

        // Insert new frame. This removes the previous frame.
        this->insertFrame(this->mFirstInIndex, page_number , frame_bytes);

        // Updating mFirstInIndex for next time FIFO must kick a page out of memory.
        this->mFirstInIndex= this->mFirstInIndex + 1 % this->nbFrames();

        // Returning the frame index.
        return mFirstInIndex;
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
