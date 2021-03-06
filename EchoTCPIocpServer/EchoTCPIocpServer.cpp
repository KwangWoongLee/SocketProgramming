#include <iostream>
#include "SocketShared.h"

int main()
{
	/// Global Managers
	GSessionManager = new SessionManager;
	GIocpManager = new Iocp;


	if (false == GIocpManager->Initialize())
		return -1;

	if (false == GIocpManager->StartIoThreads())
		return -1;


	printf_s("Start Server\n");


	GIocpManager->StartAcceptLoop(); ///< block here...

	GIocpManager->Finalize();

	printf_s("End Server\n");

	delete GIocpManager;
	delete GSessionManager;

	return 0;
}

