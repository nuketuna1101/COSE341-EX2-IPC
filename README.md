# COSE341-EX2-IPC
2022-COSE341-OS 2nd exercise 운영체제

- 1-1 shared memory

- 1-2 semaphore

작업의 원자적인 수행 보장하도록 s_quit(), s_wait() 사용

- 2 pipe

클라이언트가 서버에 정수 값을 보낼 시, 서버가 받은 값을 제곱하여 클라이언트에 반환 


IPC와 관련하여, shared memory 공유 버퍼를 이용한 방식을 구현해보았습니다. 또한, 특히 Client-Server 구조에 있어서 semaphor와 pipe 방식을 구현하여, race condition이 발생하는 것을 회피하기 위해 원자성을 보존하는 것을 이해하였습니다.
