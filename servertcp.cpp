#include "servertcp.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <boost/lexical_cast.hpp>
#define BUFFER_SIZE 256


ServerTCP::ServerTCP()
{
    /*portNumber = 1234;
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //obtém o socket do sistema
    ///AF_INET = socket que aceita endereços ipv4
    if (socketfd == -1) {
        printf("Nao foi possivel criar o socket\n");
        //return -1;
    }

    //Preparando a struct do socket
    servidor.sin_family = AF_INET;//AF_INET = socket que aceita endereços ipv4
    servidor.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
    servidor.sin_port = htons(portNumber);//associa esse socket com a porta

    //Associando o socket a porta e endereco
    if (bind(socketfd, (struct sockaddr *) &servidor, sizeof (servidor)) < 0) {
        puts("Erro ao fazer bind Tente outra porta\n");
        //return -1;
    }
    puts("Bind efetuado com sucesso\n");

    // Ouvindo por conexoes
    listen(socketfd, 1); //marco esse socket como passivo, que só excuta conexões e aceita no máximo 1 conexao pendentes em sua fila
    acceptConections();*/
}
///------------------------------------------------------------------------------------------------------

ServerTCP::ServerTCP(int port)
{

    portNumber = port;
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //obtém o socket do sistema
    ///AF_INET = socket que aceita endereços ipv4
    if (socketfd == -1) {
        printf("Nao foi possivel criar o socket\n");
        //return -1;
    }

    //Preparando a struct do socket
    servidor.sin_family = AF_INET;//AF_INET = socket que aceita endereços ipv4
    servidor.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
    servidor.sin_port = htons(portNumber);//associa esse socket com a porta

    //Associando o socket a porta e endereco
    if (bind(socketfd, (struct sockaddr *) &servidor, sizeof (servidor)) < 0) {
        puts("Erro ao fazer bind Tente outra porta\n");
        //return -1;
    }
    puts("Bind efetuado com sucesso\n");

    // Ouvindo por conexoes
    listen(socketfd, 1); //marco esse socket como passivo, que só excuta conexões e aceita no máximo 1 conexao pendentes em sua fila
    acceptConections();
}
///------------------------------------------------------------------------------------------------------

ServerTCP::~ServerTCP()
{

}
///------------------------------------------------------------------------------------------------------

void ServerTCP::acceptConections()
{
    printf("Aguardando por conexoes...");
    int c = sizeof (struct sockaddr_in);
    int conexao;


    while ((conexao = accept(socketfd, (struct sockaddr *) &cliente, (socklen_t*) & c)))
    {
        clienteIp = inet_ntoa(cliente.sin_addr);
        clientePort = ntohs(cliente.sin_port);
       // printf("cliente conectou: %s : [ %s ]\n", clienteIp, clientePort);
        if (conexao < 0)
        {
            perror("Erro ao receber conexao\n");
            return;
        }
        puts("Handler assigned\n\n");

        int tamanho = 1;
        unsigned char respostab[1024];
        while(tamanho>0)
        {
            if ((tamanho = read(conexao, respostab, BUFFER_SIZE)) < 0)
            {
                perror("Erro ao receber dados do cliente: ");

            }
            else
            {

                for(int i = 0; i< tamanho;i++)
                {
                    printf("tamanho: %d\n", tamanho);///teste
                    printf("resposta[i]: %c\n", respostab[i]);///teste



                }

            //Espero aqui o PWD do novo usuario
            unsigned char resposta[tamanho];
            for(int k=0;k<sizeof(resposta);k++)
                resposta[k]=respostab[k];

            while(S_BUSY==true) {

                sleep(1);
            }

            CONNECTING=true;
            //cout<<"Sistema ocupado, cadastro não efetuado"<<endl;

            // Retorno 0 significa que nao existe PWD cadastrado
            while(CONNECTING==true){
                usleep(100000);
            }

            if(NPWD==false){ cout<<"Senha já cadastrada!"<<endl;
                acceptConections();
            }





            //Prossegue com o cadastro ->Abre a primeira porta







                cout<<"Resposta Enviada"<<endl;
                ///teste
                //resposta[tamanho] = '\0';
                //printf("O cliente falou: %s\n", resposta);

            }

            /*if (strcmp(resposta, "end") == 0) {
                close(conexao);
                printf("Servidor finalizado...\n");
                return;
            }*/
        }
    }
}

bool ServerTCP::getBusy(){
    return S_BUSY;
}

void ServerTCP::setBusy(){
    if(S_BUSY==true) S_BUSY=false;
            else S_BUSY=false;
}

bool ServerTCP::getConnecting(){
    return CONNECTING;
}

void ServerTCP::setConnecting(){
    if(CONNECTING==true) CONNECTING = false;
            else CONNECTING = true;
}

void ServerTCP::setNPWD(){
    if(NPWD==true) NPWD = false;
            else NPWD = true;
}

/*
char ServerTCP::pwd_validation(unsigned char* resposta){
    
    std::string PWD = "";
    for(int i=1;i<5;i++){
       PWD.append(1,resposta[i]);
    }

    
    int IPWD = boost::lexical_cast<int>(PWD); 
    
    
    
    driver = get_driver_instance();
    try{
        con = driver->connect(A_IP, A_USER, A_PSWD);
        con->setSchema(A_SCHEMA);
        printf("Conexão estabelecida com o servidor\n");
        printf("Pronto para o envio de mensagens\n");

    }

    catch(sql::SQLException &e){
    cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        }
    
    std::auto_ptr< sql::Connection > con(driver->connect(A_IP, A_USER, A_PSWD));
        con->setSchema(A_SCHEMA);
        std::auto_ptr< sql::Statement > stmt(con->createStatement());
        std::auto_ptr< sql::PreparedStatement >  pstmt;
        std::auto_ptr< sql::ResultSet > res;
        pstmt.reset(con->prepareStatement("CALL safep_rt(?,@cadastro,@fing,@wht)"));
        pstmt->setInt(1,IPWD);
        pstmt->execute();
        pstmt.reset(con->prepareStatement("SELECT @cadastro AS A_CPF"));
            res.reset(pstmt->executeQuery());
            while (res->next()) {
                A_CPF = res->getUInt64("A_CPF");
            }

            pstmt.reset(con->prepareStatement("SELECT @fing AS A_FING"));
                res.reset(pstmt->executeQuery());
                while (res->next()) {
                    A_FING = res->getString("A_FING");
                }
                pstmt.reset(con->prepareStatement("SELECT @wht AS A_WEIGHT"));
                    res.reset(pstmt->executeQuery());
                    while (res->next()) {
                        A_WEIGHT = res->getInt("A_WEIGHT");
                    }
            cout<<A_CPF<<endl;
            cout<<A_FING<<endl;
            cout<<A_WEIGHT<<endl;

            if(A_CPF!=0) return '1';
            else return '0';

        }

char ServerTCP::wgt_validation(unsigned char* resposta){
    std::string WGT;
    int len = sizeof(resposta);
    cout<<len<<endl;
    for(int i=1;i<sizeof(resposta);i++)
    WGT.append(1,resposta[i]);

    cout<<"STRING-> :"<<WGT<<endl;
    int weight = atoi(WGT.c_str());
    cout<<weight<<endl;
    cout<<"PESO RECEBIDO: "<<weight<<endl;
    cout<<"PESO BD: "<<A_WEIGHT<<endl;
    float tol = 0.05*A_WEIGHT;
    cout<<"TOLERANCIA: "<<tol<<endl;
    if(weight>=(A_WEIGHT-tol)&&weight<=(A_WEIGHT+tol)) return '1';

    else return '0';
}

char ServerTCP::fng_validation(unsigned char* resposta){

}

char ServerTCP::fac_validation(unsigned char* resposta){

}

char ServerTCP::new_usr(unsigned char* resposta){
/*

    driver = get_driver_instance();
    try{
        con = driver->connect(A_IP, A_USER, A_PSWD);
        con->setSchema(A_SCHEMA);
        printf("Conexão estabelecida com o servidor\n");
        printf("Pronto para o envio de mensagens\n");

    }

    catch(sql::SQLException &e){
    cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        }

    std::auto_ptr< sql::Connection > con(driver->connect(A_IP, A_USER, A_PSWD));
        con->setSchema(A_SCHEMA);
        std::auto_ptr< sql::Statement > stmt(con->createStatement());
        std::auto_ptr< sql::PreparedStatement >  pstmt;
        std::auto_ptr< sql::ResultSet > res;
        pstmt.reset(con->prepareStatement("CALL safep_new(?,?,?)"));
        pstmt->setInt(1,NPWD);
        pstmt->setUInt64(2,NCPF);
        pstmt->setString(3,NF1);
        pstmt->setString(4,NF2);
        pstmt->setString(5,NF3);
        pstmt->setString(6,NWGT);
        pstmt->execute();
        pstmt.reset(con->prepareStatement("SELECT @cadastro AS A_CPF"));
            res.reset(pstmt->executeQuery());
            while (res->next()) {
                A_CPF = res->getUInt64("A_CPF");
            }

            pstmt.reset(con->prepareStatement("SELECT @fing AS A_FING"));
                res.reset(pstmt->executeQuery());
                while (res->next()) {
                    A_FING = res->getString("A_FING");
                }
                pstmt.reset(con->prepareStatement("SELECT @wht AS A_WEIGHT"));
                    res.reset(pstmt->executeQuery());
                    while (res->next()) {
                        A_WEIGHT = res->getInt("A_WEIGHT");
                    }
*/
/*
}

void ServerTCP::fragment(std::string full_str,int conexao){


    int len = full_str.length();
    cout<<len<<endl;
    int m=10;
    unsigned long int i=1;
    int j=1;
    while(true){
    m = len/i;
    cout<<m<<endl;
    if(m<8) break;
    i++;

    }

    unsigned long int packs = i;
    cout<<packs<<endl;
    char * resp = new char[8];
    bzero(resp,8);
    i =0;
    j=0;
    char * fstr = new char[full_str.length()];
    strcpy (fstr, full_str.c_str());

    std::string s = std::to_string(packs);
    char const *pchar = s.c_str();
    memcpy (resp,pchar,8);

    cout<<"Primeiro envio: "<<resp<<endl;
    int p=write(conexao,resp,sizeof(resp));
    if(p<0) exit(1);
    usleep(10000);
    for(int n=0;n<packs;n++)
    {
        int aux = i;
        for (i=aux; i<aux+8;i++){
            resp[j]=fstr[i];
            j++;
        }
        j=0;
        cout<<"Enviando isto: "<<resp<<endl;
        int p=write(conexao,resp,sizeof(resp)); //sizeof(resp)+1
        if (p < 0) break;
        usleep(10000);
    }


}
*/

///------------------------------------------------------------------------------------------------------

//thread ServerTCP::getServerThread()
//{
//  return thread([=] { acceptConections(); });
//}
///------------------------------------------------------------------------------------------------------
