#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 8080

volatile sig_atomic_t sighupReceived = 0;

void sighup_handler(int r) {
	sighupReceived = 1 ;
}

int main() {
	int sockt, connected_desc=0;
	struct sockaddr_in sockt_address;
	int address_length = sizeof(sockt_address);
	struct sigaction sig_act;
	sigset_t blockedMask, origMask;
	char buf[128]={0};
	int readBytes;

	if ((sockt = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1) {
		printf("Не удалось создать сокет");
		exit(EXIT_FAILURE);
	}
	
	printf("серверный сокет: %d",sockt);

	sockt_address.sin_family = AF_INET;
	sockt_address.sin_addr.s_addr=INADDR_ANY;
	sockt_address.sin_port = htons(PORT);

	if (bind(sockt, (struct sockaddr*)&sockt_address,sizeof(sockt_address)) == -1) {
		printf("Не удалось связать");
		exit(EXIT_FAILURE);
	}

	if (listen(sockt,1) ==-1) {
		printf("Ошибка прослушки");
		exit(EXIT_FAILURE);
	}
	
	sigaction(SIGHUP,NULL,&sig_act);
	sig_act.sa_handler = sighup_handler;
	sig_act.sa_flags |= SA_RESTART;
	sigaction(SIGHUP, &sig_act,NULL);
	
	sigemptyset(&blockedMask);
	sigemptyset(&origMask);
	sigaddset(&blockedMask,SIGHUP);
	sigprocmask(SIG_BLOCK, &blockedMask,&origMask);

	fd_set fds;
	int max_socket;
	while (1) {
	
	FD_ZERO(&fds);
	FD_SET(sockt,&fds);

	if (connected_desc > 0) {
		FD_SET(connected_desc, &fds);
		printf("Дескриптор соединения: %d",connected_desc);
	}
	max_socket = (connected_desc > sockt) ? connected_desc: sockt;
	
	if (pselect(max_socket + 1,&fds,NULL,NULL,NULL,&origMask) < 0 && errno != EINTR) {

		printf("Пришел не сигнал SIGHUP");
		exit(EXIT_FAILURE);
	}

	if (sighupReceived) {
		printf("Получен сигнал SIGHUP");
		sighupReceived=0;
		exit(EXIT_FAILURE);
	}
	

	
	if (connected_desc > 0 && FD_ISSET(connected_desc,&fds)){
		readBytes = read(connected_desc, buf, 128);

		if(readBytes > 0){
			printf("Получено сообщение клиента: %d bytes\n",readBytes);
		}
		else {
			if(readBytes == 0){
				close(connected_desc);
				connected_desc = 0;
				continue;
			}
			else {
				printf("Ошибка чтения");
			}
		}
		close(connected_desc);
		connected_desc = 0;
		continue;
	}


	if (FD_ISSET(sockt,&fds)) {
		if ((connected_desc = accept(sockt,(struct sockaddr*)&sockt_address,(socklen_t*)&address_length))<0){
			printf("Ошибка соединения");
			exit(EXIT_FAILURE);
		}

		printf("Установлено новое соединение\n");

	}

	
	}

close(sockt);

return 0;
}


