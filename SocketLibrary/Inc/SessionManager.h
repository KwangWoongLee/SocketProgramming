
class Session;

class SessionManager
{
public:
	SessionManager() : mCurrentConnectionCount(0) {}

	Session* CreateClientSession(SOCKET sock);

	void DeleteClientSession(Session* client);


private:
	using ClientList = std::map<SOCKET, Session*>;
	ClientList	mClientList;

	//TODO: mLock;

	volatile long mCurrentConnectionCount;
};

extern SessionManager* GSessionManager;