#include <iostream>
#include "SocketShared.h"

int main()
{
    SocketUtil::StaticInit();

    UDPSocketPtr mSocket = SocketUtil::CreateUDPSocket(IPv4);

    SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:8000");
    SocketAddress sentAddress;
    string msg;
    char recvMsg[256];

    while (true)
    {
        std::cout << "보낼 메세지 : ";
        std::getline(std::cin, msg);

        mSocket->SendTo(msg.c_str(), msg.length(),*serverAddress);

        int recvByte = mSocket->ReceiveFrom(recvMsg, 256, sentAddress);
        recvMsg[recvByte] = '\0';

        std::cout << "서버 : " << recvMsg << std::endl;

    }

}
