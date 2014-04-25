#include <iostream>
#include <commandmanager.h>
#include <QTime>
#include <qglobal.h>

typedef VirtualMemoryManager VMM;

int main(/*int argc, char *argv[]*/)
{
    cout<<"\n##############################\n\nVirtual Memory Manager\n\nKevin Forand\nHubert Lemelin"<<endl;
    srand(2245);
    qsrand(QTime::currentTime().msec());
    cout<<"\n##############################\n\nStarting Virtual Memory Manager ..."<<endl;

    CommandManager cmd_manager;

    cout<<"\n##############################\n\nCreating Commands"<<endl;

    int nb_commands = 300;

    char random_chars;

    for(int i=0; i < nb_commands; ++i)
    {
        Command::ActionType action_type = Command::READ;

        if((qrand()%100)>50)
        {
            action_type = Command::WRITE;
            random_chars =  (char)(qrand() % ('z' - 'a' + 1) + 'a');
        }

        Command c("Command",action_type,( (qrand()%32))*(qrand()%16),qrand()%256, random_chars );
        cmd_manager.insertCommand(c);
    }

    cmd_manager.applyCommands();

    cout << "STATISTICS\n" << endl;

    cout << "   TLB_HIT       : " << VMM::TLBHIT << endl;
    cout << "   TLB_MISS      : " << VMM::TLBMISS << endl;
    cout << "   TLB hit ratio : " << VMM::TLBHIT / (VMM::TLBHIT+VMM::TLBMISS) << "\n" << endl;
    cout << "   PAGEFAULT        : " << VMM::PAGEFAULT << endl;
    cout << "   PAGEFOUND        : " << VMM::PAGEFOUND << endl;
    cout << "   Page fault ratio : " << VMM::PAGEFAULT / (VMM::PAGEFAULT + VMM::PAGEFOUND) <<endl;

    cout << "\n##############################\n\nExiting Virtual Memory Manager\n\n##############################" << endl;

    return 0;
}
