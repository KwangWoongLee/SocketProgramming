#include <iostream>
#include "SocketShared.h"

int main()
{
    SocketUtil::StaticInit();

    TCPSocketPtr mSocket = SocketUtil::CreateTCPSocket(IPv4);

    SocketAddress ownAddress(INADDR_ANY, 8000);
    SocketAddress fromAddress;

    mSocket->Bind(ownAddress);

    mSocket->Listen(SOMAXCONN);

    TCPSocketPtr clientSocket = mSocket->Accept(fromAddress);

    char buf[256];


    while (true)
    {
        int recvByte = clientSocket->Receive(buf, 256);
        buf[recvByte] = '\0';
        std::cout << "클라이언트 : " << buf << std::endl;

        clientSocket->Send(buf, recvByte);
    }

    SocketUtil::CleanUp();

}

