// select �Լ�: �ϳ� �̻��� ������ ���� Ȯ���ϴµ� ���Ǵ� �Լ��̴�.

// �б� ����(readfds) - �б� ���ɼ��� �˻��� ���ϵ��� �����Ѵ�
// - listenSocket - ������ ���� listen ���¶��, ������ ���� ��û�� ���ŵǾ� accept ȣ���� ���ŷ ���� ��� �Ϸ�� �� ���� �� �б� ���� ���·� ǥ�õȴ�.
// - �Ϲ����� ����(clientSocket) - �б� �����ϴٴ� ����, ���� �� �ִ� �����Ͱ� ���� ���ۿ� �����Ͽ� send �Լ��� ���ŷ ���� ������ �ǹ�.
// - TCP ������ ���, �б� ���� ���´� �������κ����� ���� ���� ��û�� �ǹ��ϱ⵵ ��
//	--  ������ ���������� ����Ǿ�����, recv �Լ��� 0�� ��ȯ�Ѵ�.

// ���� ����(writefds) - ���� ���ɼ��� �˻��� ���ϵ��� �����Ѵ�
// - send ���� ȣ���� ������ �� �ִ� ���¸� �ǹ��Ѵ�.
// - �ٸ�, ���ŷ ���Ͽ����� �۽� ������ ���� �������� len ���ڰ� ū ��� ������ ���ŷ�� �� �ִ�.

// fd_set�� �ִ� ���� ������ �⺻ 64��.

// time_out
// - select �Լ� �������� �� ���ڰ�, select �Լ��� ��� �ð��� �����Ѵ�
// - time_out�� nullptr�̸�, �ּ� �� ������ ������ ������ �� ���� ������ ��� -> ��Ʈ��ũ �̺�Ʈ ��ٸ��� Alertable-wait���°� �ȴ�.
// - TIMEVAL ����ü�� select�� ����� �ִ� �ð��� ������.
// - �Լ��� ��ȯ�� �Ŀ��� TIMEVAL�� ������ ������� ����.
// - TIMEVAL{0,0}���� �ʱ�ȭ �Ǹ� select�� ��� ��ȯ�ϸ� �̴� Polling���·� ������
// - ��� ��ȯ�Ǵ� ��� ����ŷ ȣ��� ���ֵ�.

// ���°� ǥ�õǴ� ���?
// - readSet: ���� ���ۿ� �����Ͱ� �ִ� ��� �б� ���� ���·� ǥ�õȴ�.
// - writeSet: �� �۽� ������ ������ ����ϸ� ���� ���� ���·� ǥ�õȴ�.


// Select�Լ� �ٽ� - ���� �Լ� ȣ���� ������ ������ �̸� �� �� �ִ�.
// ���� �Լ� ȣ�� ����?

// ���� ��Ȳ)
// - ���� ���ۿ� �����Ͱ� ���µ� read,
// - �۽Ź��۰� �� á�µ�, write �Ѵٰų�

// -- ���ŷ�� ��� -> ���
// -- ����ŷ�� ��� -> WSAEWOULBLOCK

// - Select���� �ٽ��� ������ ������ �̸� �����ٴ°���
// - ���ŷ�̳� ����ŷ�̳İ� �߿����� ����

// - select + ���ŷ: ������ �������� �ʾƼ� ���ŷ �Ǵ� ��Ȳ ������ �� �ִ�.
// - select + ����ŷ: ������ �������� �ʾƼ� ���ʿ��ϰ� �ݺ� üũ�ϴ� ��Ȳ ������ �� �ִ�.

// -> send/recv�ϱ� ������ ��¥�� send, Recv �� �� �ִ��� ���� ���� üũ

// socket set
// 1) �б�[���⿡�� ������ ��] ����[���⿡�� ������ ��] ����[] ���� ��� ���
// ���� ���ϴ� ������ ������ ������ ��

// ����? out of band (MSG_OOB) 
// �޴��ʿ����� �Ȱ��� OOB������ �ؾ� ���� �� ����

// ������
// 1. ���� ��� ���
// 2. select �Լ� ȣ��(readSet, writeSet, exceptSet) ��� �� �ϴ� set���� nullptr�� �о������.
// -> ���� ����
// --> �ּ� �ϳ��� �غ� �Ǹ� �������� ������ ��, ��� �ϳ��� ������ �غ�Ǹ� �����Ѵ�. -> �����ڴ� �˾Ƽ� ���ŵȴ�.

// ���� ���� üũ�ؼ� ����(���� �ֵ� ������� send/recv ȣ�����ָ� ��)
// 1,2 �ݺ��ؾ���.

// FD_ZERO: ����
// FD_SET: ���� s�� �ִ´�

// ex)
// fd_set set
// FD_ZERO(set)
// socket s
// FD_SET(s, &set);
// FD_CLR(s, &set) -> ���� s ����

// FD_ISSET(���� s�� set�� ��������� 0�� �ƴ� ���� �����Ѵ�)


// ���� set�� �Ź� �ʱ�ȭ ����� ��

//while(true) {

//}

// send �� send�� ũ�� ��������
// ���ŷ send: ��� ������ �� ����
// ����ŷ send: �Ϻθ� ���� �� ����(���� ���� ���� ��Ȳ�� ����, 100����Ʈ�� �������� �ϴµ� ���� ���� ���� ������ 10����Ʈ�ۿ� ���� �� ���ٸ� sendLen�� 10�� ����, 
// ���Ĺ������� �̷��� ���������� Ŀ���ʿ��� �� �濡 �����Բ� �ϱ� ������ ���� �߻����� ������ FM��� �ϸ� sendLen ���� �������� ������ �� ���� �� ����)

// ��쿡 ���� �ϳ��� ���Ͽ� ���� READ_SET, WRITE_SET ���ÿ� ����ص� ��

// �ٽ�: ������ accept, send, recv ȣ���ϴ°� �ƴ�, �Լ��� ȣ��� �غ� �Ǿ��ִ��� ���� üũ �� �� �� ������ �����ϰ� ���ú�� ���� �Ѵ�!
// ����: FD_SETSIZE -> �� ���� �������� �� �ִ� �ִ� ũ�� 64
// ���� SET ������� 64�������ۿ� ��� ����
// �׷� ������ 64?�� �ƴϰ� ���� 640�� �ް� ������ SET�� 10�� ����� ��


// ��ε� ĳ��Ʈ�� �ᱹ Ŭ���̾�Ʈ ���鼭, ������ Ŭ���̾�Ʈ SendBuffer�� ��Ŷ ������ SendBuffer�� �����ϴ°���
// �׸��� ���߿� ��� Ŭ���̾�Ʈ ���鼭 Send�ϸ� Broadcast�� ������ ���޵Ǵ°�.

// ������ �� ��:
// - Session
//	-- SendBuffer
//	-- RecvBuffer

// void Broadcast(SendPkt&& pkt)
//  for(auto& s : sessions){
		// s->AppendToSendBuffer(std::forward<SendPkt>(pkt)...);
// }

	// IOExecutor
	// 1. accept socket üũ�ؼ� Accept
	// 2. Recv -> �ش� Session ���� Update
	// -- �̵���Ŷ �Դ� -> �̵��� �� �ִ��� �˻� ->(���� ���� ����, ���� �ִ� ��)
	// --- 1) ����, �� �� ���� �����̸� �ƹ��͵� ������
	// --- 2) ���̰� �ִ� ���̿����� �� �� �÷��̾ ���� �Ծ��ٰ� Broadcast
	// --- 3) �׳� �̵������� �̵��ߴٰ� Broadcast
	// 3. Send -> SendBuffer�� ���� ũ�⸸ŭ Send

	// GameThread
	// - Update ���鼭 �ֱ������� ���� ���� �� Broadcast(Append Pkt to session's sendBuffer)
	// 

	// MainThread


	/*
		vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		// ���� �� �ʱ�ȭ
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		// ListenSocket ���
		FD_SET(listenSocket, &reads);

		// ���� ���
		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		// [�ɼ�] ������ timeout ���� ���� ����
		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;

		// Listener ���� üũ
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		// ������ ���� üũ
		for (Session& s : sessions)
		{
			// Read
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// TODO : sessions ����
					continue;
				}

				s.recvBytes = recvLen;
			}

			// Write
			if (FD_ISSET(s.socket, &writes))
			{
				// ���ŷ ��� -> ��� ������ �� ����
				// ����ŷ ��� -> �Ϻθ� ���� ���� ���� (���� ���� ���� ��Ȳ�� ����)
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR)
				{
					// TODO : sessions ����
					continue;
				}
					
				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
			}
		}
	}
	*/




// -----------------------------------------------------------------------------

// 1. recvȣ���Ѵ�
// recv�� ȣ���ϰ� �Ǹ�, ���ڷ� �� writeBuffer ������ġ�� �����Ͱ� ���¸�ŭ ���̰� �ǰ�, recv�� ������ ũ�� ��ȯ�Ѵ�.

// 2. recvBuffer�� writePos�� recv���� ũ�⸸ŭ �о��ش�.

// 3. ���� dataSize(recvBuffer�� ����ִ� ������ ũ��)�� Ȯ���Ѵ�.

// 4. ProcessRecv�� ȣ���Ѵ�, ���ڷ� recvBuffer�� readPos��ġ�� ���� dataSize�� �ش�. 
// ProcessRecv���� ���ѷ����� ����.
// ó���� ������ ũ�⸦ ���Ѵ�(���� ���ۿ� ����ִ� ������ ũ�� - ó���� ������ ũ��)
// ó���� ������ ũ�Ⱑ ��Ŷ ������� ������ Ż���Ѵ�
// &readPos[processedLen]�̸�, �� ��Ŷ����� ĳ�����Ѵ� -> ���⼱ ��� ��Ŷ �����ŭ �����͸� ���� �� �ִٴ� ���!
// ���� ó���� ������ ũ�Ⱑ ��Ŷ ����� ���� ������ ũ�⺸�� ������ Ż���Ѵ�
// �װ� �ƴ϶��, ProcessPacket�� �����Ѵ�(readPos[processLen], ��Ŷ ����� ���� ������ ũ��)
// ��Ŷ�� ó��������, processedLen�� ó���� ������ ũ�⸦ �������ش�
// ó���Ѹ�ŭ recvBuffer�� ReadPos�� �������ش�.