MainThread
- AcceptThread: accept만 담당하는 쓰레드
 - Client Thread 1
 - Client Thread 2
  - Client Thread 3
 - ....
 - Client Thread N
- gameThread: 충돌판정, 먹이 생성한다.
