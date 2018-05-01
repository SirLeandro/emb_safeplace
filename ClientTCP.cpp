#include"ClientTCP.h"
#include<thread>
#include<string>
#include<stdio.h>
#include<string.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



using namespace std;

ClientTCP::ClientTCP()
{
	connected = false;
}
///------------------------------------------------------------------------------------------------------

ClientTCP::ClientTCP(int port, string ip)
{

    struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;

        //char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            exit(1);
        }
        sockfd = sock;
        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported \n");
            exit(1);
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                printf("\nConnection Failed \n");
                exit(2);
            }
        connected=true;
            /*send(sock , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
            valread = read( sock , buffer, 1024);
            printf("%s\n",buffer );
            return 0;*/
    /*
    connected = false;
    serverPortNumber = port;
    ipServer = ip;

    ///inicia o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        ///verifica se a criação do socket deu certo
        printf("ERROR opening socket");
    }

    ///tenta pegar o servidor pelo IP
    ptrServer = gethostbyname(ipServer.c_str());

    ///Se não conseguiu encontrar o server, finaliza o software
    //TODO: mudar esse exit para uma saída padrão de logs
    if (ptrServer == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        //exit(0);
        connected=false;

    }
    else{
    ///por segurança zera todos os campos da variável que representa o servidor
    bzero((char *) &servAddr, sizeof(servAddr));
    ///indica que o tipo de endereço do server é IPV4
    servAddr.sin_family = AF_INET;
    ///copia o endereço obtido do server para a variável que representa o server
    bcopy((char *)ptrServer->h_addr, (char *)&servAddr.sin_addr.s_addr, ptrServer->h_length);
    ///atribui o número da porta de conexao do server
    servAddr.sin_port = htons(serverPortNumber);
    connected=true;

    }*/

   // inicializa();
}
///------------------------------------------------------------------------------------------------------


void ClientTCP::rebind(){
    
    ///inicia o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        ///verifica se a criação do socket deu certo
        printf("ERROR opening socket");
    }

    ///tenta pegar o servidor pelo IP
    ptrServer = gethostbyname(ipServer.c_str());

    ///Se não conseguiu encontrar o server, finaliza o software
    //TODO: mudar esse exit para uma saída parão de logs
    if (ptrServer == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        //exit(0);
        connected=false;
    }
    else{
    ///por segurança zera todos os campos da variável que representa o servidor
    bzero((char *) &servAddr, sizeof(servAddr));
    ///indica que o tipo de endereço do server é IPV4
    servAddr.sin_family = AF_INET;
    ///copia o endereço obtido do server para a variável que representa o server
    bcopy((char *)ptrServer->h_addr, (char *)&servAddr.sin_addr.s_addr, ptrServer->h_length);
    ///atribui o número da porta de conexao do server
    servAddr.sin_port = htons(serverPortNumber);
    connected=true;
    }
}



ClientTCP::~ClientTCP()
{
	connected = false;
        
}
///------------------------------------------------------------------------------------------------------




    




void ClientTCP::setServerPortNumber(int portN)
{
    serverPortNumber = portN;
}
///------------------------------------------------------------------------------------------------------





int ClientTCP::sendMessageToServer(string message)
{

    int messageSize = message.size();
    int lengthOfBytes = message.length()+1;
    char * ch = new char [lengthOfBytes];
    strcpy (ch, message.c_str());
    int n = write(sockfd,ch,messageSize);
    if (n < 0)
    {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");

         return connected = 0;
    }
    usleep(10000);
    char buffer[1024];
    bzero(buffer,1024);

    int msgrcvd;

    msgrcvd = read(sockfd, buffer, 8);
    if(msgrcvd <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }


        usleep(10000);
        std::string packs_size = std::string(buffer);
        resposta = "";

        unsigned long int qtd = std::stol(packs_size);
        cout<<"QTD DE PACKS: "<<qtd<<endl;

        if(qtd==1){

            msgrcvd = read(sockfd, buffer, 8);
            if(msgrcvd <= 0) {
                 printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
                 close(sockfd);
                 connected = false;
                 return -1;
                }



        if(buffer[0]=='0')  return 0;

        else if(buffer[0]=='1')
        return 1;

        else if(buffer[0]=='2') {

            return 2;
        }

        else return -1;
        }

    for(int packs=0;packs<qtd;packs++){

        msgrcvd = read(sockfd, buffer, 8);
        if(msgrcvd <= 0) {
             printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
             close(sockfd);
             connected = false;
             return -1;
            }

        for(int i=0;i<8;i++){
        if(buffer[i]!=NULL){
        resposta.push_back(buffer[i]);

        usleep(1000);
        }
        else return 1;
        }

        cout<<"Fragmento: "<<resposta<<endl;

    }
    return 1;



}

int ClientTCP::sendLogToServer(string message)
{

    if(message=="") return 0;

    string message2;

    std::size_t f = message.find('x');
    if(f!=std::string::npos)  {
        int x, y;
            std::stringstream stream;

            x=std::stoi(message,nullptr,16);
            stream << x;
            stream >> std::hex >> y;
            message2 = std::to_string(y);


         }
    else message2 = message;



    int messageSize = message2.size();
    int lengthOfBytes = message2.length()+1;
    char * ch = new char [lengthOfBytes ];
    strcpy (ch, message2.c_str());
    cout<<"\nEnviando...\n";
    int n;
    n = write(sockfd,ch,messageSize);
    if(n <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }
    /*
    char buffer[30];
    bzero(buffer,30);
    n = read(sockfd, buffer, 30);
    if(n <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }*/
    //resposta = std::string(buffer);
    //cout<<"Resposta do Servidor:";
    //printf("-> %c", buffer[0]);
    //cout<<resposta<<"\n";
    usleep(10000);
    return 0;


}


///------------------------------------------------------------------------------------------------------
bool ClientTCP::getConnected()
{
	return connected;
}


        
      



