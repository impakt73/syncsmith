#include <core/TerminalSyncClient.h>
#include <iostream>
#include <sstream>

bool TerminalSyncClient::Initialize()
{
    std::cout << "Terminal Sync Client Started" << std::endl;
    return true;
}

void TerminalSyncClient::Run()
{
    std::cout << "Terminal Sync Client Ready!" << std::endl;
    while(!mShouldQuit)
    {
        // Read a line of input
        std::string currentLine;
        std::getline(std::cin, currentLine);

        //std::cout << "Input: " << currentLine << std::endl;
        std::vector<std::string> lineTokens;
        TokenizeInput(currentLine, lineTokens);

        /*
        for(unsigned int tokenIndex = 0; tokenIndex < lineTokens.size(); ++tokenIndex)
        {
            std::cout << "Token " << tokenIndex << ": " << lineTokens[tokenIndex] << std::endl;
        }
        */

        ProcessCommand(lineTokens);
    }
}

void TerminalSyncClient::Shutdown()
{
    std::cout << "Terminal Sync Client Stopped" << std::endl;
}

void TerminalSyncClient::TokenizeInput(const std::string& inLine, std::vector<std::string>& ioLineTokens)
{
    std::stringstream lineStream(inLine);
    std::string currentToken;
    while(lineStream.good())
    {
        std::getline(lineStream, currentToken, ' ');
        if(!currentToken.empty())
        {
            ioLineTokens.push_back(currentToken);
        }
    }
}

void TerminalSyncClient::ProcessCommand(const std::vector<std::string>& inLineTokens)
{
    if(inLineTokens.size() > 0)
    {
        if(inLineTokens[0].compare("create") == 0)
        {
            if(inLineTokens.size() > 1)
            {
                if(inLineTokens[1].compare("track") == 0)
                {
                    if(inLineTokens.size() > 2)
                    {
                        std::string trackName = inLineTokens[2];
                        std::cout << "Creating Track With Name: " << trackName << std::endl;
                    }
                    else
                    {
                        std::cout << "Invalid Track Name!" << std::endl;
                    }
                }
                else if(inLineTokens[1].compare("key") == 0)
                {
                    if(inLineTokens.size() > 2)
                    {
                        std::string keyType = inLineTokens[2];
                        std::cout << "Creating Key Of Type Name: " << keyType << std::endl;
                    }
                    else
                    {
                        std::cout << "Invalid Key Type!" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid Argument For Create Command!" << std::endl;
                }
            }
            else
            {
                std::cout << "Not Enough Arguments For Create Command!" << std::endl;
            }
        }
        else if(inLineTokens[0].compare("exit") == 0)
        {
            std::cout << "Quitting!" << std::endl;
            mShouldQuit = true;
        }
        else
        {
            std::cout << "Invalid Command: " << inLineTokens[0] << std::endl;
        }
    }
}
