
#include "UDPServer.h"

#include "../../Common/Command.h"

#include <iostream>

UDPServer::UDPServer()
    : serverAddress(Poco::Net::IPAddress(), 7896)
    , datagramSocket(serverAddress)
{
}

UDPServer::~UDPServer()
{
}

void UDPServer::Run()
{
    for (;;)
    {
        try
        {
            std::vector<char> data(1024 * 1024);
            Poco::Net::SocketAddress sender;
            size_t readDataSize = static_cast<size_t>(datagramSocket.receiveFrom(&data[0], static_cast<int>(data.size()), sender));
            data.resize(readDataSize);

            auto command =  CommandFactory::CreateFromBytes(data);

            std::string commandDataStr;
            for (auto i = 0; i < command.GetDataSize(); ++i)
            {
                if (!commandDataStr.empty())
                    commandDataStr += ", ";
                commandDataStr += command.GetDataByIndex(i);
            }
            std::cout << "Command received: " << command.GetCommandName() << ": " << commandDataStr << std::endl;

        }
        catch (Poco::Exception& exc)
        {
            std::cout << "ERROR: " << exc.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "ERROR: Unexpected error" << std::endl;
        }
    }
}
