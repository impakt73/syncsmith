#pragma once

#include <string>
#include <vector>

class TerminalSyncClient
{
public:
    TerminalSyncClient() : mShouldQuit(false) {}
    ~TerminalSyncClient() {}

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void TokenizeInput(const std::string& inLine, std::vector<std::string>& ioLineTokens);
    void ProcessCommand(const std::vector<std::string>& inLineTokens);

    bool mShouldQuit;
};
