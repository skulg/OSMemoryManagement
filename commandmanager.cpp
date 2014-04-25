#include "commandmanager.h"

CommandManager::CommandManager():TObject("Command Manager")
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::insertCommand(Command &cmd)
{
    mCommandList.push_back(cmd);
}

void CommandManager::clearAllCommands()
{
    mCommandList.clear();
}

void CommandManager::applyCommands()
{
    cout<<"\n##############################\n\nApplying commands\n\n##############################\n"<<endl;
    CommandList::iterator cmdit;
    for(cmdit = mCommandList.begin();
        cmdit != mCommandList.end(); ++cmdit)
    {
        cmdit->apply(&mVirtualMemoryManager);
    }
}
