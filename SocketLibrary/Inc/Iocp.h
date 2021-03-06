#define BUFSIZE	4096


class Iocp
{
public:
	Iocp() {};
	~Iocp();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	bool StartAcceptLoop();

	HANDLE GetComletionPort() { return mCompletionPort; }
	int	GetIoThreadCount() { return mIoThreadCount; }


private:
	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

	static bool ReceiveCompletion(const Session* client, OverlappedIOContext* context, DWORD dwTransferred);
	static bool SendCompletion(const Session* client, OverlappedIOContext* context, DWORD dwTransferred);

private:

	HANDLE	mCompletionPort;
	int		mIoThreadCount;

	SOCKET	mSocket;
};

extern Iocp* GIocpManager;