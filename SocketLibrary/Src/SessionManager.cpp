#include "SocketShared.h"

SessionManager* GSessionManager = nullptr;

Session* SessionManager::CreateClientSession(SOCKET sock)
{
	Session* client = new Session(sock);

	//TODO: lock���� ��ȣ�� �� 
	{
		mClientList.insert(ClientList::value_type(sock, client));
	}

	return client;
}


void SessionManager::DeleteClientSession(Session* client)
{
	//TODO: lock���� ��ȣ�� ��
	{
		mClientList.erase(client->mSocket);
	}

	delete client;
}