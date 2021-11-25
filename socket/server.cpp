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
		cout << "�׽��ְ�ʧ��!"<< endl;
		WSACleanup();
	}
	else
	{
		cout << "�׽��ְ󶨳ɹ�!"<< endl;
	}
	if(listen(s_server, SOMAXCONN) < 0)
	{
		cout << "�׽��ּ���ʧ��!" << endl;
		WSACleanup();
	}
	else
	{
		cout << "�׽��ּ����ɹ�!���������:"	<< SOMAXCONN << endl;
	}
	cout << "��������ڼ�������....." << endl;
	
	len = sizeof(SOCKADDR);
	while(1)
	{
		SOCKADDR_IN accept_addr;
		SOCKET s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
		if(s_accept == SOCKET_ERROR)
		{
			cout << "����ʧ��!" << endl;
			WSACleanup();
			return 0;
		}
		cout << "���ӽ�����׼�����ݽ���"<< endl;
		pthread_t thread;
		ThreadData data;
		data.accept = s_accept;
		data.accept_addr = accept_addr;
		int rc = pthread_create(&thread, NULL, communication, (void *) &data);
		if(rc)
		{
			cout << "�����߳�ʧ��" << endl;
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
			cout << "����ʧ��!" << endl;
			break;
		}
		else
		{
			cout << "���յ�" << inet_ntoa(accept_addr.sin_addr) << ":"<< accept_addr.sin_port <<"������:" << recv_buf << endl;
		}
		cout << "������ظ���Ϣ:";
		cin >> send_buf;
		send_len = send(s_accept, send_buf, 100, 0);
		if(send_len < 0)
		{
			cout << "����ʧ��!" << endl;
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
		cout << "��ʼ���׽��ֿ�ʧ��!" << endl;
	}
	else
	{
		cout << "��ʼ���׽��ֿ�ɹ�!" << endl;
	}
	if(LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "�׽��ֿ�汾�Ų���!" << endl;
	}
	else
	{
		cout << "�׽��ֿ�汾��ȷ!" << endl;
	}
}
