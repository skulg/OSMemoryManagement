#include <iostream>
#include <commandmanager.h>
#include <QTime>
#include <qglobal.h>

typedef VirtualMemoryManager VMM;

int main(/*int argc, char *argv[]*/)
{

    TLB* myTLB = new TLB("Translation Look-aside Buffer");

    for(int p = 0; p < 16; ++p)
    {
        TLB::TLB_entry myEntry(p, p+256);
        myTLB->addTLBEntry(myEntry);
    }

    myTLB->printList();

    cout << "Let's add element '16'. Element '0' will go away." << endl;

    TLB::TLB_entry myEntry(16, 16+256);
    myTLB->addTLBEntry(myEntry);

    myTLB->printList();

    cout << "Now, let's search for element '11'. It will go to the top of the list because we use LRU." << endl;

    int frame_number = -1;
    bool found = myTLB->findPage(11, frame_number);

    myTLB->printList();

    if(found)
    {
        cout << "Element '11' was found, its frame is " << frame_number << endl;
    }
    else
    {
        cout << "Element '11' not found" << endl;
    }

    myTLB->findPage(11, frame_number);

    myTLB->printList();

    cout << "Let's find an inexistant element '20'." << endl;

    found = myTLB->findPage(20, frame_number);

    if(found)
    {
        cout << "Element '20' was found, its frame is " << frame_number << endl;
    }
    else
    {
        cout << "Element '20' not found" << endl;
    }

    myTLB->printList();

    exit(0);
    srand(2245);
    cout<<"\n##############################\n\nStarting Virtual Memory Manager ..."<<endl;

    CommandManager cmd_manager;

    cout<<"Creating Commands"<<endl;
    int nb_commands = 3;
    char random_chars;
    for(int i=0; i < nb_commands; ++i)
    {
        Command::ActionType action_type = Command::READ;
        if((rand()%100)>50)
        {
            action_type = Command::WRITE;
            random_chars =  (char)(rand() % ('z' - 'a' + 1) + 'a');
        }
        Command c("Command",action_type,( (rand()%32))*(rand()%16),rand()%256, random_chars );
        cmd_manager.insertCommand(c);
    }
    cmd_manager.applyCommands();


    /*Printing static TBLHIT and PAGEFAULT counter from VirtualMemoryManager*/
    cout << "TLB hit ratio: " << VMM::TLBHIT / (VMM::TLBHIT+VMM::TLBMISS)
         << "; Page fault ratio: " << VMM::PAGEFAULT / (VMM::PAGEFAULT + VMM::PAGEFOUND) <<endl;

    cout<<"Exiting Virtual Memory Manager"<<endl;
    return 0;
}
