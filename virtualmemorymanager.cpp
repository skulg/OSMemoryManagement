#include "virtualmemorymanager.h"

float VirtualMemoryManager::TLBHIT = 0;
float VirtualMemoryManager::TLBMISS = 0;
float VirtualMemoryManager::PAGEFAULT = 0;
float VirtualMemoryManager::PAGEFOUND = 0;

VirtualMemoryManager::VirtualMemoryManager(QString str, uint nb_pages, uint page_size, uint nb_frames):
    TObject(str),mNbPages(nb_pages),mPageSize(page_size),mNbFrames(nb_frames)/*,
            mRdNumberDistribution(uniform_int_distribution<int>(0,nb_frames-1)),
            dice(std::bind ( mRdNumberDistribution, mRdNumberGenerator ))*/
{
    cout<<"Virtual Memory Initialization"<<endl;

    cout<<"Hard Drive Initialization"<<endl;
    mHardDrive = new HardDrive("Hard_Drive",nb_pages,page_size);

    cout<<"Page Table Initialization"<<endl;
    mPageTable = new PageTable("Page table",nb_pages);

    cout<<"Physical Memory Initialization"<<endl;
    mPhysicalMemory = new PhysicalMemory("Physical Memory",nb_frames);

    // We are cheating a bit : we pass the page table and the hard drive to the physical memory to make it
    // easier to update them when a frame (page) must be removed form memory and written on disk.
    mPhysicalMemory->setPageTable(mPageTable);
    mPhysicalMemory->setHardDrive(mHardDrive);

    cout<<"Translation Look-aside Buffer (TLB) Initialization"<<endl;
    mTLB = new TLB("Translation Look-aside Buffer");

    cout <<"Output log Initialisation" << endl;
    QFile::remove("log.txt");
    mLogFile = new QFile("log.txt");
    if(!mLogFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        cerr<<"Could not open "<<mLogFile->fileName().toStdString()<<endl;
        assert(false);
        return;
    }

}


VirtualMemoryManager::~VirtualMemoryManager()
{
    //Before deleting hard drive and physical memory
    //We need to save modified frames to disk
    saveRAMToDisk();

    delete mHardDrive;
    delete mPageTable;
    delete mPhysicalMemory;
    delete mTLB;

    mLogFile->close();
    delete mLogFile;
}

void VirtualMemoryManager::outputToLog( char* data )
{
    /*DO NOT MODIFY THIS FUNCTION*/
    cout << "Read Data: " << *data << endl;
    QByteArray page;
    page.resize(1);
    page[0]=*data;
    mLogFile->write(data, 1);

}

void VirtualMemoryManager::saveRAMToDisk()
{
    //TP2_IFT2245_TO_DO
    for (uint i=0 ; i<this->mPhysicalMemory->nbFrames();i++){
        QByteArray *currentData= this->mPhysicalMemory->frame(i);
        this->mHardDrive->write(this->mPhysicalMemory->pageNumber(i), currentData);
    }
    //TP2_IFT2245_END_TO_DO
}


void VirtualMemoryManager::read(uint page_number, uint offset, char *data)
{
    cout<<"Read Operation : ";
    //TP2_IFT2245_TO_DO

    int frameNumberInMemory  = fetchPage(page_number);

    this->mPhysicalMemory->read(frameNumberInMemory, offset, data);

    cout << "Page number is " << frameNumberInMemory << endl;

    //TP2_IFT2245_END_TO_DO

    //Do not remove this function
    outputToLog(data);
}

void VirtualMemoryManager::write(uint page_number, uint offset, char *data)
{
    cout<<"Write Operation : ";
    //TP2_IFT2245_TO_DO

    int frameNumberInMemory  = fetchPage(page_number);

    cout << "Page number is " << frameNumberInMemory << endl;

    this->mPhysicalMemory->write(frameNumberInMemory, offset, data);

    //TP2_IFT2245_END_TO_DO
}

uint VirtualMemoryManager::fetchPage(uint page_number)
{
    //TP2_IFT2245_TO_DO
    int frameIndexSearchResult;
    if (this->mTLB->findPage(page_number,frameIndexSearchResult)){
        this->TLBHIT++;
        this->PAGEFOUND++;

        return frameIndexSearchResult;
    }else{
        this->TLBMISS++;
        if(this->mPageTable->frameIndex(page_number , frameIndexSearchResult)){
            this->PAGEFOUND++;
            TLB::TLB_entry myEntry(page_number, frameIndexSearchResult);
            this->mTLB->addTLBEntry(myEntry);
            //TLB::TLB_entry myEntry(page_number, frameIndexSearchResult);
            //this->mTLB->addTLBEntry(*myEntry);
            return frameIndexSearchResult;
        }else{
            this->PAGEFAULT++;
            //Fetch from hardrive
            QByteArray *data= this->mHardDrive->read(page_number);

            frameIndexSearchResult =  this->mPhysicalMemory->insertFrameInNextFreeSpace(page_number, data);
            Page* myPage = new Page("page", page_number, this->mPageSize, frameIndexSearchResult, true);
            this->mPageTable->insertPage(page_number, *myPage);
            TLB::TLB_entry myEntry(page_number, frameIndexSearchResult);
            this->mTLB->addTLBEntry(myEntry);
            return frameIndexSearchResult;
        }

    }



    return 0;
    //TP2_IFT2245_END_TO_DO
}


