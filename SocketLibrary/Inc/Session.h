#define BUFSIZE	4096

class Session;
class SessionManager;

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT
};

enum DisconnectReason
{
	DR_NONE,
	DR_RECV_ZERO,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_COMPLETION_ERROR,
};

struct OverlappedIOContext
{
	OverlappedIOContext(const Session* owner, IOType ioType) : mSessionObject(owner), mIoType(ioType)
	{
		memset(&mOverlapped, 0, sizeof(OVERLAPPED));
		memset(&mWsaBuf, 0, sizeof(WSABUF));
		memset(mBuffer, 0, BUFSIZE);
	}

	OVERLAPPED				mOverlapped;
	const Session* mSessionObject;
	IOType			mIoType;
	WSABUF			mWsaBuf;
	char			mBuffer[BUFSIZE];
};


class Session
{
public:
	Session(SOCKET sock)
		: mSocket(sock), mConnected(false)
	{
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	}

	~Session() {}

	bool	OnConnect(SOCKADDR_IN* addr);
	bool	IsConnected() const { return mConnected; }

	bool	PostRecv() const;
	bool	PostSend(const char* buf, int len) const;
	void	Disconnect(DisconnectReason dr);


private:
	bool			mConnected;
	SOCKET			mSocket;

	SOCKADDR_IN		mClientAddr;

	//TODO: mLock; 

	friend class SessionManager;
};
