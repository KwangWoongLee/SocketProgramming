#include "SocketShared.h"

#define GQCS_TIMEOUT	INFINITE //20

Iocp* GIocpManager = nullptr;

class Session;

Iocp::~Iocp()
{
}

bool Iocp::Initialize()
{
	SYSTEM_INFO stSysInfo;
	GetSystemInfo(&stSysInfo);
	mIoThreadCount = stSysInfo.dwNumberOfProcessors * 2;


	SocketUtil::StaticInit();

	SocketAddress ownAddress(INADDR_ANY, 9990);


	/// Create I/O Completion Port
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, mIoThreadCount);

	if (mCompletionPort == NULL)
		return false;

	/// create TCP socket
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET)
		return false;

	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, mCompletionPort, 0, 0);
	if (handle != mCompletionPort)
	{
		printf_s("[DEBUG] listen socket IOCP register error: %d\n", GetLastError());
		return false;
	}

	int opt = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9990);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(mSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		return false;

	if (SOCKET_ERROR == listen(mSocket, SOMAXCONN))
	{
		printf_s("[DEBUG] listen error\n");
		return false;
	}



	return true;

}

bool Iocp::StartIoThreads()
{
	for (int i = 0; i < mIoThreadCount; ++i)
	{
		unsigned int dwThreadId;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, (LPVOID)(i + 1), 0, &dwThreadId);
	}

	return true;
}

bool Iocp::StartAcceptLoop()
{
	/// accept loop
	while (true)
	{
		SOCKET acceptedSock = accept(mSocket, NULL, NULL);
		if (acceptedSock == INVALID_SOCKET)
		{
			printf_s("accept: invalid socket\n");
			continue;
		}

		

		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(acceptedSock, (SOCKADDR*)&clientaddr, &addrlen);

		/// 소켓 정보 구조체 할당과 초기화
		Session* client = GSessionManager->CreateClientSession(acceptedSock);

		CreateIoCompletionPort((HANDLE)acceptedSock, mCompletionPort, (ULONG_PTR)client, 0);

		OverlappedIOContext* recvContext = new OverlappedIOContext(client, IO_RECV);

		char buf[] = "hi";
		DWORD recvbytes = 0;
		DWORD flags = 0;
		recvContext->mWsaBuf.len = 3;
		recvContext->mWsaBuf.buf = buf;

		WSARecv(acceptedSock, &recvContext->mWsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL);
	}

	return true;
}

void Iocp::Finalize()
{
	CloseHandle(mCompletionPort);

	SocketUtil::CleanUp();
}


unsigned int WINAPI Iocp::IoWorkerThread(LPVOID lpParam)
{
	HANDLE hComletionPort = GIocpManager->GetComletionPort();

	while (true)
	{
		DWORD dwTransferred = 0;
		OverlappedIOContext* context = nullptr;
		Session* asCompletionKey = nullptr;

		int ret = GetQueuedCompletionStatus(hComletionPort, &dwTransferred, (PULONG_PTR)&asCompletionKey, (LPOVERLAPPED*)&context, GQCS_TIMEOUT);
		printf_s("%s",context->mWsaBuf.buf);
		/// check time out first 
		if (ret == 0 && GetLastError() == WAIT_TIMEOUT)
			continue;

		//if (ret == 0 || dwTransferred == 0)
		//{
		//	/// connection closing
		//	asCompletionKey->Disconnect(DR_RECV_ZERO);
		//	GSessionManager->DeleteClientSession(asCompletionKey);
		//	continue;
		//}

		// if (nullptr == context) 인 경우 처리
		//{
		//}

		bool completionOk = true;
		switch (context->mIoType)
		{
		case IO_SEND:
			completionOk = SendCompletion(asCompletionKey, context, dwTransferred);
			break;

		case IO_RECV:
			completionOk = ReceiveCompletion(asCompletionKey, context, dwTransferred);
			break;

		default:
			printf_s("Unknown I/O Type: %d\n", context->mIoType);
			break;
		}

		//if (!completionOk)
		//{
		//	/// connection closing
		//	asCompletionKey->Disconnect(DR_COMPLETION_ERROR);
		//	GSessionManager->DeleteClientSession(asCompletionKey);
		//}

	}

	return 0;
}


bool Iocp::ReceiveCompletion(const Session* client, OverlappedIOContext* context, DWORD dwTransferred)
{

	/// echo back 처리 client->PostSend()사용.
	printf_s("%s", context->mBuffer);
	
	delete context;

	return client->PostRecv();
}

bool Iocp::SendCompletion(const Session* client, OverlappedIOContext* context, DWORD dwTransferred)
{
	/// 전송 다 되었는지 확인하는 것 처리..
	//if (context->mWsaBuf.len != dwTransferred) {...}
	
	delete context;
	return true;
}