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


using namespace std;

ClientTCP::ClientTCP()
{
	connected = false;
}
///------------------------------------------------------------------------------------------------------

ClientTCP::ClientTCP(int port, string ip)
{
    
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
    }
    
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


    if(message=="") return 0;
    
    string message2;
    time_t timer;
        char temp[20];
        struct tm* tm_info;

        time(&timer);



    std::size_t f = message.find('x');
    if(f!=std::string::npos)  {
        int x, y;
            std::stringstream stream;

            //x=std::stoi(message,nullptr,16);
            stream << message;
            stream >> std::hex >> y;
            message2 = std::to_string(y);


         }
    else message2 = message;

    tm_info = localtime(&timer);
    strftime(temp, 20, "%d/%m/%Y,%H:%M:%S", tm_info);
    //message2 = message2 + "@" + temp;


    int messageSize = message2.size();
    int lengthOfBytes = message2.length()+1;
    char * ch = new char [lengthOfBytes ];
    strcpy (ch, message2.c_str());
    cout<<"\nEnviando...\n";
    int n;
    n=write(sockfd,ch,messageSize);
    usleep(10000);
    
    char buffer[30];
    bzero(buffer,30);
    n = read(sockfd, buffer, 30);
    if(n <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }
    resposta = std::string(buffer);
    cout<<"Resposta do Servidor:";
    //printf("-> %c", buffer[0]);
    cout<<resposta<<"\n";
    
    if(buffer[0]=='0')
        return 0;
    //delete[] buffer;
    
    else if(buffer[0]=='1'&&buffer[1]==0)
    return 1;

    else return 2;
    
    usleep(1000);
      
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


        
      



