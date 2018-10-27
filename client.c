#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in addr;
	char sentence[8192];
	int len;

	//����socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//����Ŀ��������ip��port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(21);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {			//ת��ip��ַ:���ʮ����-->������
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//������Ŀ����������socket��Ŀ���������ӣ�-- ��������
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	int m = recv(sockfd, sentence, 8192, 0);
	if (!m) return 1;
	sentence[m] = '\0';
	printf("%s", sentence);

	int code;
	while (1) {
		printf("my_ftpclient > ");
		//��ȡ��������
		fgets(sentence, 4096, stdin);
		len = strlen(sentence);
		sentence[len - 1] = '\r';
		sentence[len] = '\n';
		sentence[len + 1] = '\0';
		len++;

		send(sockfd, sentence, len, MSG_WAITALL);
		// printf("msg sent to the server.\n");
		
		int m = recv(sockfd, sentence, 8192, 0);
		// printf("read from server: %d\n", m);
		sentence[m] = '\0';

		printf("FROM SERVER: %s", sentence);

		if (sscanf(sentence, "%d", &code) != 0)
			if (code == 221) break;
	}

	close(sockfd);

	return 0;
}