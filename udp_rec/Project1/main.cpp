#include<winsock2.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define BUFFER_SIZE 2048

int  data_move = 8;
int  data_head = data_move + 0;
int  cmd = data_move + 1;
int  rec_crc = data_move + 1039;
int sum;

int main() {


    WSADATA WSAData;
    char  receBuf[BUFFER_SIZE];
    char Response[] = "";
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        printf("��ʼ��ʧ��");
        exit(1);
    }
    SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockServer == INVALID_SOCKET)
    {
        printf("Failed socket() \n");
        return 0;
    }
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(5008);
    sin.sin_addr.s_addr = inet_addr("172.16.5.35");//���ն�IP��ַ

    SOCKADDR_IN addr_Server; //�������ĵ�ַ����Ϣ
    addr_Server.sin_family = AF_INET;
    addr_Server.sin_port = htons(8881);
    addr_Server.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(sockServer, (SOCKADDR*)&addr_Server, sizeof(addr_Server)) == SOCKET_ERROR) {//�������뱾�ص�ַ��
        printf("Failed socket() %d \n", WSAGetLastError());
        return 0;
    }
    SOCKADDR_IN addr_Clt;
    printf("APP begin \n");
    int fromlen = sizeof(SOCKADDR);

    //sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);


    while (true) {
        int last = recvfrom(sockServer, receBuf, 2048, 0, (SOCKADDR*)&addr_Clt, &fromlen);
        if (last > 0) {      //�жϽ��յ��������Ƿ�Ϊ��
            receBuf[last] = '\0';//���ַ������һ��'\0'����ʾ�����ˡ���Ȼ���������

               //for (size_t i = 0; i < last - data_move; i++)
               //{
               //    printf("%02x", (unsigned char)receBuf[i + data_move]);
               //   
               //}
               //printf("\n");
       sendto(sockServer, receBuf, 2048, 0, (struct sockaddr*)&sin, sizeof(struct sockaddr));
              
     
        }
       
    }

    closesocket(sockServer);

    WSACleanup();
    return 0;
}