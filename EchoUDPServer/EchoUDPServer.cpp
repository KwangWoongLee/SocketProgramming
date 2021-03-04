#include <iostream>
#include "SocketShared.h"

int main()
{
    SocketUtil::StaticInit();

    UDPSocketPtr mSocket = SocketUtil::CreateUDPSocket(IPv4);

    SocketAddress ownAddress(INADDR_ANY, 8000);
    SocketAddress fromAddress;

    mSocket->Bind(ownAddress);

    char buf[256];


    while (true)
    {
        int recvByte = mSocket->ReceiveFrom(buf, 256,fromAddress);
        buf[recvByte] = '\0';
        std::cout << "클라이언트 : " << buf << std::endl;

        mSocket->SendTo(buf, recvByte,fromAddress);
    }

    SocketUtil::CleanUp();

}

