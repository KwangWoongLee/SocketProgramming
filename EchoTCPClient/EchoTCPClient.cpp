#include <iostream>
#include "SocketShared.h"

int main()
{
    SocketUtil::StaticInit();

    TCPSocketPtr mSocket = SocketUtil::CreateTCPSocket(IPv4);

    SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:9990");


    if (mSocket->Connect(*serverAddress) != NOERROR)
    {
        return 0;
    }

    string msg;
    char recvMsg[256];

    while (true)
    {
        std::cout << "보낼 메세지 : ";
        std::getline(std::cin, msg);

        mSocket->Send(msg.c_str(), msg.length());

        int recvByte = mSocket->Receive(recvMsg, 256);
        recvMsg[recvByte] = '\0';

        std::cout << "서버 : " << recvMsg << std::endl;

    }

}
