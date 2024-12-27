#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main() {
	int sockd;
	const char *host_name ="127.0.0.1";
	
	struct addrinfo result;
	result.ai_family = AF_INET;
	result.ai_socktype =  SOCK_STREAM;
	result.ai_protocol = IPPROTO_TCP;

	struct sockaddr_in addr;
	addr.sin_family =AF_INET;
	addr.sin_port = htons(PORT);

	if ((sockd = socket(result.ai_family,result.ai_socktype,result.ai_protocol)) == -1){
		printf("Не удалось создать сокет");
		exit(EXIT_FAILURE);
	}

	if (inet_pton(AF_INET, host_name, &addr.sin_addr) == -1){
		printf("Не удалось преобразовать адрес");
		exit(EXIT_FAILURE);
	}

	if (connect(sockd,(struct sockaddr*)&addr,sizeof(addr)) == -1){
		printf("Ошибка подключения");
		exit(EXIT_FAILURE);
	}

	char *msg ="Клиент отправил сообщение\n";
	send(sockd, msg,strlen(msg),0);
	printf("Сообщение отправлено\n");

	close(sockd);
	
	return 0;
}
