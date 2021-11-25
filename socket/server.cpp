#include <winsock.h>
#include <iostream>
#include <pthread.h>

using namespace std;

struct ThreadData
{
	SOCKET accept;
	SOCKADDR_IN accept_addr;
};
 




#pragma comment(lib, "ws2_32.lib")
void initialization();
void* communication(void *data);

int main()
{
	int len = 0;
	SOCKET s_server;
	
	SOCKADDR_IN server_addr;

	initialization();
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5010);
	s_server = socket(AF_INET, SOCK_STREAM,0);
	
	if(bind(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "套接字绑定失败!"<< endl;
		WSACleanup();
	}
	else
	{
		cout << "套接字绑定成功!"<< endl;
	}
	if(listen(s_server, SOMAXCONN) < 0)
	{
		cout << "套接字监听失败!" << endl;
		WSACleanup();
	}
	else
	{
		cout << "套接字监听成功!最大连接数:"	<< SOMAXCONN << endl;
	}
	cout << "服务端正在监听连接....." << endl;
	
	len = sizeof(SOCKADDR);
	while(1)
	{
		SOCKADDR_IN accept_addr;
		SOCKET s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
		if(s_accept == SOCKET_ERROR)
		{
			cout << "连接失败!" << endl;
			WSACleanup();
			return 0;
		}
		cout << "连接建立，准备数据接收"<< endl;
		pthread_t thread;
		ThreadData data;
		data.accept = s_accept;
		data.accept_addr = accept_addr;
		int rc = pthread_create(&thread, NULL, communication, (void *) &data);
		if(rc)
		{
			cout << "创建线程失败" << endl;
		}
	}
	
	closesocket(s_server);
	
	WSACleanup();
	return 0;
}

void* communication(void *data)
{
	int send_len = 0;
	int recv_len = 0;
	ThreadData* thread_data = (ThreadData*) data;
	char send_buf[100];
	char recv_buf[100];
	SOCKET s_accept = thread_data->accept;
	SOCKADDR_IN accept_addr = thread_data->accept_addr;
	
	while(1)
	{
		recv_len = recv(s_accept, recv_buf, 100, 0);
		if(recv_len < 0)
		{
			cout << "接收失败!" << endl;
			break;
		}
		else
		{
			cout << "接收到" << inet_ntoa(accept_addr.sin_addr) << ":"<< accept_addr.sin_port <<"的数据:" << recv_buf << endl;
		}
		cout << "请输入回复信息:";
		cin >> send_buf;
		send_len = send(s_accept, send_buf, 100, 0);
		if(send_len < 0)
		{
			cout << "发送失败!" << endl;
			break;
		}
	}
	closesocket(s_accept);
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
