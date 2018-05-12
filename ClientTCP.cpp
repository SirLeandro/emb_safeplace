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
#include <string>
#define BUFFER_SIZE 1024
#define SLEEP_TIME 15000



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
    usleep(SLEEP_TIME);
    fragment(message,sockfd);
    /*
    int messageSize = message.size();
    int lengthOfBytes = message.length();
    cout<<"LENGTH: "<<lengthOfBytes<<endl;
    char * ch = new char [lengthOfBytes];
    strcpy (ch, message.c_str());
    int n = write(sockfd,ch,messageSize);
    if (n < 0)
    {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");

         return connected = 0;
    }*/
    usleep(SLEEP_TIME);
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);

    int msgrcvd;

    msgrcvd = read(sockfd, buffer, BUFFER_SIZE);
    if(msgrcvd <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }

        cout<<"PRIMEIRO PACOTE RECEBIDO!: "<<std::string(buffer)<<endl;
        usleep(SLEEP_TIME);
        std::string packs_size = std::string(buffer);
        resposta = "";

        unsigned long int qtd = std::stol(packs_size);
        cout<<"QTD DE PACKS: "<<qtd<<endl;

        if(qtd==1){
        char nbuf[BUFFER_SIZE];
            msgrcvd = read(sockfd, nbuf, BUFFER_SIZE);
            if(msgrcvd <= 0) {
                 printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
                 close(sockfd);
                 connected = false;
                 return -1;
                }

         cout<<"BUFFER[O]: "<<nbuf[0]<<endl;

        if(nbuf[0]=='0')  return 0;

        else if(nbuf[0]=='1')
        return 1;

        else if(nbuf[0]=='2') {

            return 2;
        }

        else return -1;
        }
    else{
    for(int packs=0;packs<qtd;packs++){

        msgrcvd = read(sockfd, buffer, BUFFER_SIZE);
        if(msgrcvd < 0) {
             printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
             close(sockfd);
             connected = false;
             return -1;
            }

        for(int i=0;i<BUFFER_SIZE;i++){
        if(buffer[i]!=NULL){
        resposta.push_back(buffer[i]);

        }
        else{
            cout<<"i: "<<i<<"   packs: "<<packs<<endl;
            return 1;
        }

        }
        bzero(buffer,BUFFER_SIZE);
        cout<<"Fragmento: "<<resposta<<endl;
    usleep(SLEEP_TIME);
    }
        }
    return 3;



}

int ClientTCP::sendImageToServer(char* toSEND,unsigned long int tambytes)
{
    usleep(SLEEP_TIME);
    img_fragment(toSEND,tambytes,sockfd);
    /*
    int messageSize = message.size();
    int lengthOfBytes = message.length();
    cout<<"LENGTH: "<<lengthOfBytes<<endl;
    char * ch = new char [lengthOfBytes];
    strcpy (ch, message.c_str());
    int n = write(sockfd,ch,messageSize);
    if (n < 0)
    {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");

         return connected = 0;
    }*/
    usleep(SLEEP_TIME);
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);

    int msgrcvd;

    msgrcvd = read(sockfd, buffer, BUFFER_SIZE);
    if(msgrcvd <= 0) {
         printf("ClientTCP::sendMessageToServer = Erro ao escrever no socket");
         close(sockfd);
         connected = false;
         return -1;
    }

        cout<<"PRIMEIRO PACOTE RECEBIDO!: "<<std::string(buffer)<<endl;
        usleep(SLEEP_TIME);
        std::string packs_size = std::string(buffer);
        resposta = "";

        unsigned long int qtd = std::stol(packs_size);
        cout<<"QTD DE PACKS: "<<qtd<<endl;

        if(qtd==1){
        char nbuf[BUFFER_SIZE];
            msgrcvd = read(sockfd, nbuf, BUFFER_SIZE);
            if(msgrcvd <= 0) {
                 printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
                 close(sockfd);
                 connected = false;
                 return -1;
                }

         cout<<"BUFFER[O]: "<<nbuf[0]<<endl;

        if(nbuf[0]=='0')  return 0;

        else if(nbuf[0]=='1')
        return 1;

        else if(nbuf[0]=='2') {

            return 2;
        }

        else return -1;
        }
    else{
            int bytes=0;
    for(int packs=0;packs<qtd;packs++){

        msgrcvd = read(sockfd, buffer, BUFFER_SIZE);
        if(msgrcvd < 0) {
             printf("ClientTCP::sendMessageToServer = Erro ao ler no socket");
             close(sockfd);
             connected = false;
             return -1;
            }

        for(int i=0;i<BUFFER_SIZE;i++){
        if(bytes<qtd){//buffer[i]!=NULL){
        resposta.push_back(buffer[i]);
        bytes++;
        }
        else{
            cout<<"i: "<<i<<"   packs: "<<packs<<endl;
            return 1;
        }

        }
        //delete [] buffer;
        usleep(SLEEP_TIME);
        //bzero(buffer,BUFFER_SIZE);
        cout<<"Fragmento: "<<resposta<<endl;

    }
        }
    return 3;



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
    usleep(SLEEP_TIME);
    return 0;


}

void ClientTCP::fragment(std::string full_str,int conexao){


    unsigned long int len = full_str.size();//length();
    cout<<"FULL STR LEN: "<<len<<endl;
    unsigned long int m=10;
    unsigned long int i=1;
    int j=1;
    while(true){
    m = len/i;
    cout<<m<<endl;
    if(m<BUFFER_SIZE) break;
    i++;

    }

    unsigned long int packs;
    if(len%BUFFER_SIZE==0) packs = i-1;
    else packs = i;
    cout<<"PACKS A SEREM ENVIADOS: "<<packs<<endl;
    char * resp = new char[BUFFER_SIZE];
    bzero(resp,BUFFER_SIZE);
    i =0;
    j=0;
    char * fstr = new char[full_str.size()];
    //bzero(fstr, full_str.size());
    strcpy (fstr, full_str.c_str());

    std::string s = std::to_string(len);//packs);
    char const *pchar = s.c_str();
    memcpy (resp,pchar,BUFFER_SIZE);

    cout<<"Primeiro envio: "<<resp<<endl;
    int p=write(conexao,resp,std::string(pchar).size());
    if(p<0) exit(1);
    usleep(SLEEP_TIME);
    cout<<fstr<<endl;
    cout<<"FULL STRING: "<<full_str<<endl;
    for(int n=0;n<packs;n++)
    {
        resp = new char[BUFFER_SIZE];
        int aux = i;
        for (i=aux; i<aux+BUFFER_SIZE;i++){
            if(fstr[i]!=NULL){
            resp[j]=fstr[i];
            j++;
            }
            else {
                break;
                //resp[j]=' ';break;
                //j++;
            }
        }


                char send[j];
                for(int h=0;h<j;h++)
                send[h]=resp[h];
        cout<<"Enviando isto: "<<send<<endl;
        cout<<"Tamanho: "<<sizeof(send)<<endl;
        int p=write(conexao,send,sizeof(send)); //sizeof(resp)+1
        if (p < 0) break;
        usleep(SLEEP_TIME);
        j=0;
        //delete [] resp;

    }


}

void ClientTCP::img_fragment(char* toSEND, unsigned long int tambytes, int conexao){



    unsigned long int len = tambytes;//full_str.size();//length();
    cout<<"FULL STR LEN: "<<len<<endl;
    unsigned long int m=10;
    unsigned long int i=1;
    int j=1;
    unsigned long int packs = 0;
    if(tambytes%BUFFER_SIZE==0) packs = tambytes/BUFFER_SIZE;
    else packs = tambytes/BUFFER_SIZE +1;

    //unsigned long int packs = tambytes;//fileSIZE;
    //if(len%BUFFER_SIZE==0) packs = i-1;
    //else packs = i;
    //cout<<"PACKS A SEREM ENVIADOS: "<<packs<<endl;
    char * resp = new char[BUFFER_SIZE];
    bzero(resp,BUFFER_SIZE);
    i =0;
    j=0;
    //char * fstr = new char[full_str.size()];
    //bzero(fstr, full_str.size());
    //strcpy (fstr, full_str.c_str());

    std::string s = std::to_string(tambytes);
    char const *pchar = s.c_str();
    memcpy (resp,pchar,BUFFER_SIZE);

    //cout<<"Primeiro envio(Tamanho de BYTES) = "<<resp<<endl;
    int p=write(conexao,resp,std::string(pchar).size());
    if(p<0) exit(1);
    usleep(SLEEP_TIME);
    //cout<<fstr<<endl;
    //cout<<"FULL STRING: "<<full_str<<endl;
    unsigned long int byteatual=0;
    for(int n=0;n<packs;n++)
    {
        //send = new char[1];
        //int aux = i;
        //for (i=aux; i<aux+1;i++){
            //if(fstr[i]!=NULL)
            /*if(i<d){
            resp[j]=toSEND[i];//fstr[i];
            j++;
            }
            else {
                break;
                //resp[j]=' ';break;
                //j++;
            }
        }


                char send[j];
                for(int h=0;h<j;h++)
                send[h]=resp[h];
        cout<<"Enviando isto: "<<send<<endl;
        cout<<"Tamanho: "<<sizeof(send)<<endl;
        int p=write(conexao,send,sizeof(send)); //sizeof(resp)+1
        if (p < 0) break;
        usleep(15000);
        j=0;
        delete [] resp;
        */  char send[BUFFER_SIZE];
            int bufferbyte=0;

            for(bufferbyte=0;bufferbyte<BUFFER_SIZE;bufferbyte++){
            if(byteatual<tambytes){
            send[bufferbyte] = toSEND[byteatual];
            byteatual++;
            }
            else break;
            }

            int p=write(conexao,send,BUFFER_SIZE); //sizeof(resp)+1
            if (p < 0) break;
            cout<<100*n/packs<<"% Completo"<<endl;
            usleep(SLEEP_TIME);
            //delete [] send;
    }
    cout<<"IMAGEM ENVIADA"<<endl;

}



///------------------------------------------------------------------------------------------------------
bool ClientTCP::getConnected()
{
	return connected;
}


        
      



