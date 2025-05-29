#pragma once

class ICommand
{
public:
    virtual ~ICommand() {}
    virtual void Execute() = 0;
};