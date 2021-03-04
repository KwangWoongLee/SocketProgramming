#include <iostream>
#include "SocketShared.h"

int main()
{
    SocketUtil::StaticInit();

    TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(IPv4);

    SocketAddress ownAddress(INADDR_ANY, 8000);
    

    listenSocket->Bind(ownAddress);

    listenSocket->Listen(SOMAXCONN);
    

    vector<TCPSocketPtr> readSockets;
    readSockets.push_back(listenSocket);

    vector<TCPSocketPtr> writeSockets;
    vector<TCPSocketPtr> readResultSockets;
    vector<TCPSocketPtr> writeResultSockets;

    while (true)
    {
        if (!SocketUtil::Select(&readSockets, &readResultSockets,
            &writeSockets, &writeResultSockets, nullptr, nullptr))
            continue;

        for (const TCPSocketPtr& socket : readResultSockets)
        {
            if (socket == listenSocket)
            {
                SocketAddress fromAddress;
                TCPSocketPtr clientSocket = socket->Accept(fromAddress);
                readSockets.push_back(clientSocket);
            }
            else
            {
                char buf[256];


                int recvByte = socket->Receive(buf, 256);
                buf[recvByte] = '\0';
                std::cout << "클라이언트" << socket << " : "<< buf << std::endl;

                socket->Send(buf, recvByte);
            }
        }
    }

    SocketUtil::CleanUp();

}

