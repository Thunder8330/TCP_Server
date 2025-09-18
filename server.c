#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    struct sockaddr_in client, server, server_wan;
    int lfd, n, confd, msg_len;
    int lfd_wan;
    FILE *stream;
    int ret,i;

	i = 0;    
    	stream = fopen("Log_file.log","w");
    	printf("stream : %X",stream);

    	printf("Begin");
	char r_buff[200] = "", s_buff[200] = "";
	char r_buff_wan[200] = "", s_buff_wan[200] = "";
	char client_add[INET_ADDRSTRLEN];

		
	lfd = socket(AF_INET, SOCK_STREAM, 0);
//	fcntl(lfd, F_SETFL, fcntl(lfd, F_GETFL, 0) | O_NONBLOCK);
	server.sin_family = AF_INET;
	server.sin_port = htons(5002);
	server.sin_addr.s_addr = inet_addr("192.168.0.7");

	lfd_wan = socket(AF_INET, SOCK_STREAM ,0);
	fcntl(lfd_wan, F_SETFL, fcntl(lfd_wan, F_GETFL, 0) | O_NONBLOCK);
	server_wan.sin_family = AF_INET;
	server_wan.sin_port = htons(7700);
	server_wan.sin_addr.s_addr = inet_addr("47.88.85.196");

	ret = connect(lfd_wan, (struct sockaddr *)&server_wan, sizeof server_wan);

	bind(lfd, (struct sockaddr *)&server, sizeof server);
	listen(lfd, 1);
	n = sizeof client;
	confd = accept(lfd, (struct sockaddr *)&client, &n);
	printf("\nLoop");									
	fcntl(confd, F_SETFL, fcntl(confd, F_GETFL, 0) | O_NONBLOCK);


	while (1) {

		msg_len = recv(confd, r_buff, sizeof r_buff, 0);

		
		if (msg_len > 0)
		{
			r_buff[msg_len] = 0;
			inet_ntop(AF_INET,&client.sin_addr.s_addr,client_add,sizeof(client_add));
			printf("\n[Tracker] %d,%s Rx:%d %s", client.sin_port, client_add , msg_len, r_buff);
			msg_len = send(lfd_wan, r_buff, msg_len, 0);
			printf(", size :%d",msg_len);
		}
		else
		{
			sleep(1);
			printf("#");
		}
		
		msg_len = recv(lfd_wan, r_buff, sizeof r_buff, 0);
		
		if (msg_len > 0)
		{
		
			r_buff[msg_len] = 0;
			//inet_ntop(AF_INET,&client.sin_addr.s_addr,client_add,sizeof(client_add));
			printf("\n[Server]  %s", r_buff);
			msg_len = send(confd, r_buff, msg_len, 0);
			printf(", size :%d",msg_len);
		}
		else
		{
			sleep(1);
			printf("$");
		}

		fflush(stdout);
	}
	close(confd);
	close(lfd);
	close (lfd_wan);

			   
	return 0;
                                                                                                                                                        }
