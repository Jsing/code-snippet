BOOL CreateNonBlockingSocket(SOCKET *pSocket)
{
	*pSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if (*pSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	u_long iMode = 1; // 1 : non-blocking mode, 0 : blocking mode

	int iResult = ioctlsocket(*pSocket, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
	{
		return FALSE;
	}

	return TRUE;
}

int ConnectWithTimeout(SOCKET socket, sockaddr_in addr, int timeoutSeconds)
{
	if (connect(socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		int error = WSAGetLastError();

		if (error != WSAEWOULDBLOCK) {
			return -error;
		}
	}

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(socket, &writefds);

	struct timeval tv;              
	memset(&tv, 0, sizeof(tv));
	tv.tv_sec = timeoutSeconds; 
	tv.tv_usec = 0;

	int result = select(0 /*ignored in windows*/, NULL, &writefds, NULL, &tv);
	if (result == 0 || result == -1)
	{
		return SOCKET_ERROR;
	}

	return 0;
}

int ReceiveWithTimeout(SOCKET socket, char *buf, int len, int timeoutSeconds)
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);

	struct timeval tv;  
	tv.tv_sec = timeoutSeconds;
	tv.tv_usec = 0;

	int result = select(0 /*ignored in windows*/, &readfds, NULL, NULL, &tv);
	if (result == 0 || result == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}

	return recv(socket, buf, len, 0);
}