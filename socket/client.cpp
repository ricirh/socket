#include <iostream>
#include <winsock.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

void initialization();

int main()
{
	int send_len = 0;
	int recv_len = 0;
	char send_buf[100];
	char recv_buf[100];
	SOCKET s_server;
	
	SOCKADDR_IN server_addr;
	initialization();
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5010);
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if( connect(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "服务器连接失败!" << endl;
		WSACleanup();
	}
	else
	{
		cout << "服务器连接成功!" << endl;
	}
	
	while(1)
	{
		cout << "请输入发送消息:";
		cin >> send_buf;
		send_len = send(s_server, send_buf, 100, 0);
		if(send_len < 0)
		{
			cout << "发送失败!" << endl;
			break;
		}
		recv_len = recv(s_server, recv_buf, 100, 0);
		
		if(recv_len < 0)
		{
			cout << "接收失败!" << endl;
			break;
		}
		else
		{
			cout << "服务端消息:" << recv_buf << endl;
		}
	}
	closesocket(s_server);
	WSACleanup();
	return 0;
}

void initialization()
{
	WORD w_req = MAKEWORD(2,2);
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if(err != 0)
	{
		cout << "初始化套接字库失败!" << endl;
	}
	else
	{
		cout << "初始化套接字库成功!" << endl;
	}
	if(LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "套接字库版本号不符!" << endl;
	}
	else
	{
		cout << "套接字库版本正确!" << endl;
	}
}