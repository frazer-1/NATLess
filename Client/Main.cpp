#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"

#include "../../Common/Command.h"

#include <iostream>


void main(int argc, char** argv)
{
    try
    {
		while (true)
		{
			Poco::Net::SocketAddress serverAddress("localhost", 7896);
			Poco::Net::DatagramSocket datagramSocket;
			datagramSocket.connect(serverAddress);

			std::string clientName = "Agent1";
			Command regestryCommand = CommandFactory::CreateRegistryCommand(clientName);

			auto msg = regestryCommand.ToBytes();
			datagramSocket.sendBytes(msg.data(), static_cast<int>(msg.size()));

			Sleep(1000);
		}
        //datagramSocket.receiveBytes();
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