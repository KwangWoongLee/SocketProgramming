#pragma once
class UDPSocket
{
public:
	~UDPSocket();
	int Bind(const SocketAddress& inToAddress);
	int SendTo(const void* inToSend, int inLen, const SocketAddress& inToAddress);
	int ReceiveFrom(void* inFromReceive, int inMaxLength, SocketAddress& outFromAddress);

	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

using UDPSocketPtr = shared_ptr< UDPSocket>;