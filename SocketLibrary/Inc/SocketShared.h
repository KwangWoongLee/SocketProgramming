#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include "ws2tcpip.h"
#include "process.h"

#include "memory"
#include "vector"
#include "string"
#include "map"

using std::shared_ptr;
using std::vector;
using std::string;



#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "SocketUtil.h"

#include "Session.h"
#include "SessionManager.h"

#include "Iocp.h"
