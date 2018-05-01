#ifndef SERVERTCP_H
#define SERVERTCP_H


    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <netdb.h> //sockaddr_in
    #include<thread>
    #include<iostream>
    #include <unistd.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    /*#include <mysql/mysql.h>
    #include <mysql_connection.h>
    #include <mysql_driver.h>
    #include <mysql_error.h>
    #include "libs/mysql.h"
    #include "libs/errmsg.h"
    #include "libs/driver/mysql_driver.h"
    #include "libs/driver/mysql_connection.h"
    #include "libs/errmsg.h"
    #include "libs/driver/mysql_driver.h"
    #include "libs/driver/mysql_connection.h"
    #include "libs/cppconn/driver.h"
    #include "libs/cppconn/exception.h"
    #include "libs/cppconn/resultset.h"
    #include "libs/cppconn/statement.h"
    #include "libs/cppconn/prepared_statement.h"
    */
    using namespace std;
    /*
    *Descricao: Classe que implementa um servidor TCP
    *Versao: 1.0
    */
    class ServerTCP
    {

    private:
            struct sockaddr_in servidor,//estrutura que guarda as configurações de um socket
                                cliente;
            int portNumber; //numero da porta de conexao desse servidor
            int socketfd; //retorna do método de criação de um socket
            string clienteIp; //ip do cliente
            int clientePort; //porta da aplicação no cliente
            std::string A_IP = "prcsport.mysql.uhserver.com:3306";
            //std::string A_ADDR = "19216830202";
            std::string A_USER = "prcsport";
            std::string A_PSWD = "Senha@1992";
            std::string A_SCHEMA = "prcsport";
            /*sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stmt;
            sql::ResultSet *res;
            sql::PreparedStatement *pstmt;
            */
            int A_WEIGHT;
            long int A_CPF;
            std::string A_FING;
            bool S_BUSY = false;
            bool CONNECTING = false;
            bool NPWD = false;

    public:
        /**
        *Descição: Construtor da classe
        */
        ServerTCP();

        /**
        *Descição: sobrecarga do construtor da classe
        *Argumento: port => (int) indica a porta na qual o servidor ficará escutando
        *Argumento: port => (ExecutorDeComandos*) ponteiro com a referência do executor de comandos para poder adicionar comandos à sua lista
        */
        ServerTCP(int port);

        /**
        *Descição: Destrutor da classe
        */
        ~ServerTCP();

        /**
        *Descição: Método que será utilizado dentro da thread so servidor. Fica aguardando novas conexões
        */
        void acceptConections();
        char pwd_validation(unsigned char* resposta);
        char wgt_validation(unsigned char* resposta);
        char fng_validation(unsigned char* resposta);
        char fac_validation(unsigned char* resposta);
        void fragment(std::string full_str,int conexao);
        char new_usr(unsigned char* resposta);
        bool getBusy();
        void setBusy();
        bool getConnecting();
        bool getNPWD();
        void setConnecting();
        void setNPWD();
        /**
        *Descição: método que retorna uma thread com referência para a função acceptConections
        *return: thread=> referenciando a função acceptConections()
        */
        //thread getServerThread();
    };

#endif
