// select 함수: 하나 이상의 소켓의 상태 확인하는데 사용되는 함수이다.

// 읽기 상태(readfds) - 읽기 가능성을 검사할 소켓들을 지정한다
// - listenSocket - 소켓이 현재 listen 상태라면, 들어오는 연결 요청이 수신되어 accept 호출이 블로킹 없이 즉시 완료될 수 있을 떄 읽기 가능 상태로 표시된다.
// - 일반적인 소켓(clientSocket) - 읽기 가능하다는 것은, 읽을 수 있는 데이터가 수신 버퍼에 존재하여 send 함수가 블로킹 되지 않음을 의미.
// - TCP 소켓의 경우, 읽기 가능 상태는 상대방으로부터의 연결 종료 요청을 의미하기도 함
//	--  연결이 정상적으로 종료되었으면, recv 함수는 0을 반환한다.

// 쓰기 상태(writefds) - 쓰기 가능성을 검사할 소켓들을 지정한다
// - send 등의 호출이 성공할 수 있는 상태를 의미한다.
// - 다만, 블로킹 소켓에서는 송신 버퍼의 여유 공간보다 len 인자가 큰 경우 여전히 블로킹될 수 있다.

// fd_set의 최대 소켓 개수는 기본 64다.

// time_out
// - select 함수 마지막에 들어갈 인자고, select 함수의 대기 시간을 제어한다
// - time_out이 nullptr이면, 최소 한 소켓이 조건을 만족할 때 까지 무기한 대기 -> 네트워크 이벤트 기다리며 Alertable-wait상태가 된다.
// - TIMEVAL 구조체는 select가 대기할 최대 시간을 지정함.
// - 함수가 반환된 후에도 TIMEVAL의 내용은 변경되지 않음.
// - TIMEVAL{0,0}으로 초기화 되면 select는 즉시 반환하며 이는 Polling형태로 동작함
// - 즉시 반환되는 경우 논블로킹 호출로 간주됨.

// 상태가 표시되는 경우?
// - readSet: 수신 버퍼에 데이터가 있는 경우 읽기 가능 상태로 표시된다.
// - writeSet: 내 송신 버퍼의 공간이 충분하면 쓰기 가능 상태로 표시된다.


// Select함수 핵심 - 소켓 함수 호출이 성공할 시점을 미리 알 수 있다.
// 소켓 함수 호출 성공?

// 문제 상황)
// - 수신 버퍼에 데이터가 없는데 read,
// - 송신버퍼가 꽉 찼는데, write 한다거나

// -- 블로킹일 경우 -> 대기
// -- 논블로킹일 경우 -> WSAEWOULBLOCK

// - Select모델의 핵심은 성공할 시점을 미리 엿본다는것임
// - 블로킹이냐 논블로킹이냐가 중요하진 않음

// - select + 블로킹: 조건이 만족되지 않아서 블로킹 되는 상황 예방할 수 있다.
// - select + 논블로킹: 조건이 만족되지 않아서 불필요하게 반복 체크하는 상황 예방할 수 있다.

// -> send/recv하기 이전에 진짜로 send, Recv 할 수 있는지 여부 먼저 체크

// socket set
// 1) 읽기[여기에는 소켓이 들어감] 쓰기[여기에는 소켓이 들어감] 예외[] 관찰 대상 등록
// 내가 원하는 관찰에 소켓을 넣으면 됨

// 예외? out of band (MSG_OOB) 
// 받는쪽에서도 똑같이 OOB세팅을 해야 받을 수 있음

// 시퀀스
// 1. 관살 대상 등록
// 2. select 함수 호출(readSet, writeSet, exceptSet) 사용 안 하는 set들은 nullptr로 밀어버린다.
// -> 관찰 시작
// --> 최소 하나라도 준비가 되면 그제서야 리턴을 함, 적어도 하나의 소켓이 준비되면 리턴한다. -> 낙오자는 알아서 제거된다.

// 남은 소켓 체크해서 진행(남은 애들 대상으로 send/recv 호출해주면 됨)
// 1,2 반복해야함.

// FD_ZERO: 비운다
// FD_SET: 소켓 s를 넣는다

// ex)
// fd_set set
// FD_ZERO(set)
// socket s
// FD_SET(s, &set);
// FD_CLR(s, &set) -> 소켓 s 제거

// FD_ISSET(소켓 s가 set에 들어있으면 0이 아닌 값을 리턴한다)


// 소켓 set은 매번 초기화 해줘야 함

//while(true) {

//}

// send 시 send한 크기 리턴해줌
// 블로킹 send: 모든 데이터 다 보냄
// 논블로킹 send: 일부만 보낼 수 있음(상대방 수신 버퍼 상황에 따라, 100바이트를 보내려고 하는데 상대방 수신 버퍼 측에서 10바이트밖에 받을 수 없다면 sendLen은 10을 리턴, 
// 공식문서에는 이렇게 나와있지만 커널쪽에서 한 방에 보내게끔 하기 때문에 거의 발생하지 않지만 FM대로 하면 sendLen 갑싱 보내려는 값보다 더 작을 수 있음)

// 경우에 따라서 하나의 소켓에 대해 READ_SET, WRITE_SET 동시에 등록해도 됨

// 핵심: 무작정 accept, send, recv 호출하는게 아닌, 함수가 호출될 준비가 되어있는지 먼저 체크 한 후 그 다음에 안전하게 리시브랑 샌드 한다!
// 단점: FD_SETSIZE -> 한 번에 세팅해줄 수 있는 최대 크기 64
// 단일 SET 대상으로 64개까지밖에 등록 못함
// 그럼 동접이 64?는 아니고 만약 640명 받고 싶으면 SET을 10개 만들면 됨


// 브로드 캐스트는 결국 클라이언트 돌면서, 각자의 클라이언트 SendBuffer에 패킷 내용을 SendBuffer에 누적하는것임
// 그리고 나중에 모든 클라이언트 돌면서 Send하면 Broadcast한 내용이 전달되는것.

// 만들어야 할 것:
// - Session
//	-- SendBuffer
//	-- RecvBuffer

// void Broadcast(SendPkt&& pkt)
//  for(auto& s : sessions){
		// s->AppendToSendBuffer(std::forward<SendPkt>(pkt)...);
// }

	// IOExecutor
	// 1. accept socket 체크해서 Accept
	// 2. Recv -> 해당 Session 정보 Update
	// -- 이동패킷 왔다 -> 이동할 수 있는지 검사 ->(갈수 없는 영역, 먹이 있는 곳)
	// --- 1) 만약, 갈 수 없는 영역이면 아무것도 안해줌
	// --- 2) 먹이가 있는 곳이였으면 몇 번 플레이어가 먹이 먹었다고 Broadcast
	// --- 3) 그냥 이동했으면 이동했다고 Broadcast
	// 3. Send -> SendBuffer에 적힌 크기만큼 Send

	// GameThread
	// - Update 돌면서 주기적으로 먹이 생성 후 Broadcast(Append Pkt to session's sendBuffer)
	// 

	// MainThread


	/*
		vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		// 소켓 셋 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		// ListenSocket 등록
		FD_SET(listenSocket, &reads);

		// 소켓 등록
		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		// [옵션] 마지막 timeout 인자 설정 가능
		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;

		// Listener 소켓 체크
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

		// 나머지 소켓 체크
		for (Session& s : sessions)
		{
			// Read
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// TODO : sessions 제거
					continue;
				}

				s.recvBytes = recvLen;
			}

			// Write
			if (FD_ISSET(s.socket, &writes))
			{
				// 블로킹 모드 -> 모든 데이터 다 보냄
				// 논블로킹 모드 -> 일부만 보낼 수가 있음 (상대방 수신 버퍼 상황에 따라)
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR)
				{
					// TODO : sessions 제거
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

// 1. recv호출한다
// recv를 호출하게 되면, 인자로 준 writeBuffer 시작위치에 데이터가 들어온만큼 쌓이게 되고, recv한 데이터 크기 반환한다.

// 2. recvBuffer의 writePos를 recv받은 크기만큼 밀어준다.

// 3. 현재 dataSize(recvBuffer에 들어있는 데이터 크기)를 확인한다.

// 4. ProcessRecv를 호출한다, 인자로 recvBuffer의 readPos위치와 현재 dataSize를 준다. 
// ProcessRecv에선 무한루프를 돈다.
// 처리할 데이터 크기를 구한다(현재 버퍼에 들어있는 데이터 크기 - 처리한 데이터 크기)
// 처리할 데이터 크기가 패킷 헤더보다 작으면 탈출한다
// &readPos[processedLen]이면, 을 패킷헤더로 캐스팅한다 -> 여기선 적어도 패킷 헤더만큼 데이터를 읽을 수 있다는 얘기!
// 만약 처리할 데이터 크기가 패킷 헤더에 적힌 데이터 크기보다 작으면 탈출한다
// 그게 아니라면, ProcessPacket을 진행한다(readPos[processLen], 패킷 헤더에 적힌 데이터 크기)
// 패킷을 처리했으니, processedLen의 처리할 데이터 크기를 누적해준다
// 처리한만큼 recvBuffer의 ReadPos를 움직여준다.