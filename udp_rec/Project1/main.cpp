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
    SOCKADDR_IN addr_Server; //�������ĵ�ַ����Ϣ
    addr_Server.sin_family = AF_INET;
    addr_Server.sin_port = htons(8881);
    addr_Server.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(sockServer, (SOCKADDR*)&addr_Server, sizeof(addr_Server)) == SOCKET_ERROR) {//�������뱾�ص�ַ��
        printf("Failed socket() %d \n", WSAGetLastError());
        return 0;
    }
    SOCKADDR_IN addr_Clt;

    int fromlen = sizeof(SOCKADDR);
    while (true) {
        int last = recvfrom(sockServer, receBuf, 2048, 0, (SOCKADDR*)&addr_Clt, &fromlen);
        if (last > 0) {      //�жϽ��յ��������Ƿ�Ϊ��
            receBuf[last] = '\0';//���ַ������һ��'\0'����ʾ�����ˡ���Ȼ���������

                for (size_t i = 0; i < last - data_move; i++)
                {
                    printf("%02x", (unsigned char)receBuf[i + data_move]);
                   
                }
                for (size_t i = 0; i < last - data_move-1; i++)
                {
                    sum = (unsigned char)receBuf[i + data_move] + sum;
                }
                printf("\n");
                printf("���յ�����CRC ��  У��� : %02x \n", (unsigned char)receBuf[rec_crc]);
                printf("���ؼ�������CRC ��  У��� : %02x \n", sum&0xff );
                sum = 0;

                printf("���յ����ݣ�%s�� ��  ������ : %02x \n", inet_ntoa(addr_Clt.sin_addr) ,(unsigned char)receBuf[cmd]);
                if ((unsigned char)receBuf[cmd] == 0xc1) {
                    printf("֡ͷ : %02x  ", (unsigned char)receBuf[data_head]);
                    printf("֡ID : %02d\n", (unsigned char)receBuf[2 + data_move]);
                    printf("©����Ԥ�� : %02x  ", (unsigned char)receBuf[1015 + data_move]);
                    printf("©�������� : %02x \n", (unsigned char)receBuf[1016 + data_move]);
                    printf("************************************************************ \n");
                } 
                if ((unsigned char)receBuf[cmd] == 0xc0) {
                    printf("��������֡ID : %02d\n", (unsigned char)receBuf[2 + data_move]);
                     
                }
        }
       
    }

    closesocket(sockServer);

    WSACleanup();
    return 0;
}