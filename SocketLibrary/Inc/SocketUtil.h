enum SocketAddressFamily
{
	IPv4 = AF_INET,
	IPv6 = AF_INET6
};

class SocketUtil
{
public:

	static bool			StaticInit();
	static void			CleanUp();

	static void			ReportError(const char* inOperationDesc);
	static int			GetLastError();

	static int			Select(const vector< TCPSocketPtr >* inReadSet,
		vector< TCPSocketPtr >* outReadSet,
		const vector< TCPSocketPtr >* inWriteSet,
		vector< TCPSocketPtr >* outWriteSet,
		const vector< TCPSocketPtr >* inExceptSet,
		vector< TCPSocketPtr >* outExceptSet);

	static UDPSocketPtr	CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr	CreateTCPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr	CreateIocpTCPSocket(SocketAddressFamily inFamily);

	static SocketAddressPtr CreateIPv4FromString(const string& inString);

private:

	static fd_set* FillSetFromVector(fd_set& outSet, const vector< TCPSocketPtr >* inSockets);
	static void FillVectorFromSet(vector< TCPSocketPtr >* outSockets, const vector< TCPSocketPtr >* inSockets, const fd_set& inSet);
};