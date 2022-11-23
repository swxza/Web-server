#include<stdio.h>
#include<WinSock2.h>//包含网络编程的头文件，引入静态库
#pragma comment(lib, "ws2_32.lib")

//TCP服务器创建流程
//1.socket创建一个套接字
//2.bind绑定ip和port
//3.listen使用套接字变为可以被动连接
//4.accept等待客户端的链接
//5.recv/send接收发送数据



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
	printf("欢迎使用xz的web服务器。。。\n");

	WSADATA wsdata;
	int isok = WSAStartup(MAKEWORD(2,2),&wsdata);//确定socket版本信息
	//short 2个字节 2.2
	merror(isok, WSAEINVAL, "申请socket失败");

	//AF_INET使用Ipv4地址  协议族 决定socket的地址类型 
	//SOCK_STREAM传输类型 流格式套接字  流传输
	//IPPROTO_TCP  指定的传输协议 tcp
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	merror(server, INVALID_SOCKET, "创建socket失败");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;  //和创建的时候必须一样  ipv4
	seraddr.sin_port = htons(80);   //注意：网络中的数据和电脑上的数据存储是有区别的，网络是大端存储，pc是小端存储
	seraddr.sin_addr.s_addr = INADDR_ANY;  //监听任意地址

	isok = bind(server, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "绑定失败...\n");

	listen(server, 5);
	merror(isok, SOCKET_ERROR, "监听失败...\n");

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);

	while (1)
	{
		SOCKET client = accept(server, &claddr, &cllen);//谁连进来了
		merror(client, INVALID_SOCKET, "连接失败...\n");

		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s 共接收到%d字节数据\n", revdata, strlen(revdata));

		
		//char sendata[1024] = "<h1 style =\" color:red;\">你好，我是小王！<h1>";
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
		printf("打开文件失败\n");
		return;
	}
	char temp[1024] = "";

	do
	{
		fgets(temp, 1024, pfile);
		send(s, temp, strlen(temp), 0);
	} while (!feof(pfile));
	
}