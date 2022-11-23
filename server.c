#include<stdio.h>
#include<WinSock2.h>//���������̵�ͷ�ļ������뾲̬��
#pragma comment(lib, "ws2_32.lib")

//TCP��������������
//1.socket����һ���׽���
//2.bind��ip��port
//3.listenʹ���׽��ֱ�Ϊ���Ա�������
//4.accept�ȴ��ͻ��˵�����
//5.recv/send���շ�������



int merror(int redata, int error, char* showinfo)
{
	if (redata == error)
	{
		perror(showinfo);
		getchar();
		return -1;
	}
	return 0;
}
void sendhtml(SOCKET s, char *filename);
int main()
{
	printf("��ӭʹ��xz��web������������\n");

	WSADATA wsdata;
	int isok = WSAStartup(MAKEWORD(2,2),&wsdata);//ȷ��socket�汾��Ϣ
	//short 2���ֽ� 2.2
	merror(isok, WSAEINVAL, "����socketʧ��");

	//AF_INETʹ��Ipv4��ַ  Э���� ����socket�ĵ�ַ���� 
	//SOCK_STREAM�������� ����ʽ�׽���  ������
	//IPPROTO_TCP  ָ���Ĵ���Э�� tcp
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	merror(server, INVALID_SOCKET, "����socketʧ��");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;  //�ʹ�����ʱ�����һ��  ipv4
	seraddr.sin_port = htons(80);   //ע�⣺�����е����ݺ͵����ϵ����ݴ洢��������ģ������Ǵ�˴洢��pc��С�˴洢
	seraddr.sin_addr.s_addr = INADDR_ANY;  //���������ַ

	isok = bind(server, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "��ʧ��...\n");

	listen(server, 5);
	merror(isok, SOCKET_ERROR, "����ʧ��...\n");

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);

	while (1)
	{
		SOCKET client = accept(server, &claddr, &cllen);//˭��������
		merror(client, INVALID_SOCKET, "����ʧ��...\n");

		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s �����յ�%d�ֽ�����\n", revdata, strlen(revdata));

		
		//char sendata[1024] = "<h1 style =\" color:red;\">��ã�����С����<h1>";
		//send(client, sendata, strlen(sendata), 0);

		char *filename = "./index.html";
		sendhtml(client, filename);

		closesocket(client);

	}

	closesocket(server);
	WSACleanup();




	getchar();
	return 0;
}

void sendhtml(SOCKET s, char *filename)
{
	FILE *pfile = fopen(filename, "r");
	if (pfile == NULL)
	{
		printf("���ļ�ʧ��\n");
		return;
	}
	char temp[1024] = "";

	do
	{
		fgets(temp, 1024, pfile);
		send(s, temp, strlen(temp), 0);
	} while (!feof(pfile));
	
}