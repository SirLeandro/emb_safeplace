/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: Leandro @ MetroSystem
 *
 * Created on June 27, 2017, 10:35 PM
 */

#include <stdio.h>
#include <iostream>
#include <lcd.h>
#include<thread>
#include<string>
#include<string.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mutex>
#include <wiringPi.h>
#include <sys/time.h>
#include "mainwindow.h"
#include <time.h>
#include <sstream>
#include "MFRC522.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>


#include "ClientTCP.h"
#include "libs/NBioAPI.h"
#include "libs/mysql.h"
#include "libs/errmsg.h"
#include "libs/driver/mysql_driver.h"
#include "libs/driver/mysql_connection.h"
#include "libs/cppconn/driver.h"
#include "libs/cppconn/exception.h"
#include "libs/cppconn/resultset.h"
#include "libs/cppconn/statement.h"
#include <libs/cppconn/prepared_statement.h>


#define SQL false
#define UI false


using namespace std;

// Conexão via socket
int SOCKET_PORT;
std::string SOCKET_IP;


/* Conexão via mysql procedure - estação base

std::string A_IP = "prcsport.mysql.uhserver.com:3306";
std::string A_ADDR = "19216830202";
std::string A_USER = "prcsport";
std::string A_PSWD = "Caio@lindo2";
std::string A_SCHEMA = "prcsport";
*/

/*
sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *pstmt;
*/

/*Variáveis do leitor biométrico*/

NBioAPI_HANDLE g_hBSP;
NBioAPI_DEVICE_ID m_DeviceID;
NBioAPI_RETURN ret;

/*P for Peripherials*/

const int P_COIL1; // Solenoide
const int P_COIL2; // Solenoide
const int P_BUTTON; // Botão de saída
const int P_BUTTON_D1; // Barreira porta 1
const int P_BUTTON_D2; // Barreira porta 2
const int P_SIREN; // Sirene
const int P_OLED; // LCD OLED
const int P_LOAD_CLK;
const int P_LOAD_DATA;


//const char  * S_MSG_SQL;

/*S for System*/
int S_VALID = 0;

/*A for Access*/
bool A_PWD = false; //confirmacao de senha
bool A_WHT = false; //confirmacao de peso
bool A_FR = false; // reconhecimento facial
bool A_DR = false; // reconhecimento de digital



void start(); // INICIA O SISTEMA
void online(); // SISTEMA ONLINE COM A ESTAÇÃO-BASE
void offline(ClientTCP ctc); // SISTEMA OFFLINE, TENTA RECONECTAR COM A ESTAÇÃO-BASE
void writeLog(std::string A_ID); // MANTÉM NUM LOG A ENTRADA E SAÍDA DE PESSOAS
int sendMessage(ClientTCP ctc,std::string A_VALID); // ENVIA COMANDOS PARA A ESTAÇÃO-BASE
int sendLog(); // ENVIA O LOG PARA A ESTAÇÃO-BASE
void pwd_validation(std::string S_PWD); // REALIZA A VALIDAÇÃO DA SENHA
void weight_validation(); // REALIZA A VALIDAÇÃO DO PESO
void finger_validation(); // REALIZA A VALIDAÇÃO DA DIGITAL
void facial_validation(); // REALIZA A VALIDAÇÃO FACIAL
void open_D1(); // ABRE A PORTA 1
void open_D2(); // ABRE A PORTA 2
void lock_D1(); // FECHA A PORTA 1
void lock_D2(); // FECHA A PORTA 2


std::string actualTime(); // FUNÇÃO QUE RETORNA A DATA ATUAL

int main(int argc, char *argv[]) {


	wiringPiSetup();
/*
    pinMode(P_COIL, OUTPUT);
    pinMode(P_OPT1, INPUT);
    pinMode(P_OPT2, INPUT);
    pinMode(P_BUTTON, INPUT);
    pinMode(P_PIC,OUTPUT);
    pinMode(P_BUZZ, OUTPUT);
    pinMode(P_LED_ON, OUTPUT);
    pinMode(P_LED_OFF, OUTPUT);	 	
    digitalWrite(P_PIC,HIGH);
    digitalWrite(P_COIL, HIGH);
    */

    //thread t(start); // Apenas para interface gráfica
    thread t2(checkRF);
    thread cs(checkOpt);
    start();


}

void start(){
	printIP();
    ClientTCP ctc(SOCKET_PORT,SOCKET_IP);
    if(ctc.getConnected()==true)
    online();
    else
    offline(ctc);

}



void online(){

    int A_STATE;
    string S_PWD;


	while(true){
        std::string S_PWD;

        std::getline(std::cin,S_PWD);

        pwd_validation(S_PWD);

}

}

void offline(ClientTCP ctc){
   while(ctc.connected==false)
   ctc.rebind();

   online();
}



int sendMessage(ClientTCP ctc,std::string A_CMD){

    //Dados a serem enviados para o servidor> Possivelmente hash 40-50 digitos para recuperacao de digital (no maximo receber 3 digitais de resposta)
    S_VALID = ctc.sendMessageToServer(A_VALID);


        if(S_VALID==0){
    P_BUTTON_ACTUAL_STATE = digitalRead(P_BUTTON);

    time_t start = time(0);
    double b;

    while((b=difftime(time(0),start))<=3){
        P_BUTTON_STATE = digitalRead(P_BUTTON);

        if(P_BUTTON_STATE!=P_BUTTON_ACTUAL_STATE){


        S_VALID=1;

    break;

        }

    }

    return 1;
}
else if(S_VALID==1){

    digitalWrite(P_PIC, LOW);
    time_t start = time(0);
    double b;
    while((b=difftime(time(0),start))<=8){
        if(S_PASS==true){
        S_VALID = 0;
        S_PASS=false;
        lcdClear(LCD);
        lcdPosition(LCD,0,0);
        lcdPuts(LCD, "#======ONLINE======#");
            break;
        }
    }
    digitalWrite(P_PIC, HIGH);
    S_VALID=0;

    return 1;
}
else if(S_VALID==2){

    digitalWrite(P_PIC, LOW);
    sleep(4);
    S_VALID=0;
    digitalWrite(P_PIC, HIGH);
    return 1;
}

return 0;

}




void writeLog(std::string A_ID){
	
    if(A_ID=="") return;

    string S_LOG_MSG;
	time_t timer;

	time(&timer);

    S_LOG_MSG = A_ID+"@"+actualTime();

    std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app);
    log_file << S_LOG_MSG << std::endl;
	
}

std::string actualTime(){

	time_t timer;
	char buffer[20];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buffer);

}



int sendLog(){

    std::ifstream infile("log.txt");
    std::string S_LINE;
    while (std::getline(infile, S_LINE))
	{

        std::istringstream iss(S_LINE);
        std::string S_LOGLINE;
        if (!(iss>>S_LOGLINE)) { break;}

        std::vector<std::string> A_STR;
        boost::split(A_STR, S_LOGLINE, boost::is_any_of("@"));

	
        int A_STATE = sendLogToServer(A_STR[0],A_STR[1]);
        if(A_STATE == 0) return 0;
        usleep(10000);
	}
    string fileName = "log.txt";
	remove(fileName.c_str());
    return 1;

}




void pwd_validation(){


}

void weight_validation(){

}


void finger_validation(){
    // Initialize device
        if ( NBioAPI_Init(&g_hBSP) != NBioAPIERROR_NONE ){
            std::cout<<"Inicializacao falhou";
            return;
            }
        else{
          // Open device, device auto detect
            m_DeviceID = NBioAPI_DEVICE_ID_AUTO;
            ret = NBioAPI_OpenDevice(g_hBSP, m_DeviceID);

            // Open device
            if ( ret != NBioAPIERROR_NONE ){
                std::cout<<"Abertura do dispositivo falhou";
                return;
                }
            else{
              std::cout<<"Dispositivo inicializado com sucesso.\n\nAproxime o dedo do Sensor.\n";

                //Enroll Fingerprint variables
                NBioAPI_FIR_HANDLE g_hCapturedFIR;
                NBioAPI_FIR_PURPOSE m_Purpose;

                m_Purpose = NBioAPI_FIR_PURPOSE_ENROLL;
                ret = NBioAPI_Capture(
                    g_hBSP,           // Handle of NBioBSP Module
                    m_Purpose,        // Purpose of capture
                    &g_hCapturedFIR,  // Handle of captured FIR
                    3910000,          // Capture timeout
                    NULL,             // Audit data
                    NULL              // Window option
                    );
                std::cout<<"Digital Capturada com sucesso.\n\n";

                NBioAPI_BOOL result;
                            NBioAPI_INPUT_FIR storedFIR, inputFIR;

                            // Read stored data and convert into FIR(fir1)
                            std::ifstream t("DigitalString.txt");
                            std::string text_stream((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
                            int length;

                            length = strlen(text_stream.c_str());

                            NBioAPI_FIR_TEXTENCODE g_storedTextFIR; // Set input FIR.
                            g_storedTextFIR.IsWideChar = NBioAPI_FALSE;			// It depends on application
                            g_storedTextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
                            memcpy(g_storedTextFIR.TextFIR, text_stream.c_str(), length + 1);

                            storedFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // stored FIR
                            storedFIR.InputFIR.TextFIR = &g_storedTextFIR;

                            // Read input stream and convert into FIR(fir2)

                            // Text encode the FIR generated
                            NBioAPI_FIR_TEXTENCODE g_newTextFIR; // Text encoded FIR

                            // Get Text encoded FIR from FIR handle
                            ret = NBioAPI_GetTextFIRFromHandle(g_hBSP, g_hCapturedFIR, &g_newTextFIR, NBioAPI_FALSE);

                            inputFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // input FIR to be compared
                            inputFIR.InputFIR.TextFIR = &g_newTextFIR;

                            ret = NBioAPI_VerifyMatch( // Matching use with stored FIR
                            g_hBSP,		// Handle of NBioBSP module
                            &storedFIR,	// stored FIR
                            &inputFIR,	// input FIR for matching
                            &result,	// Result of matching
                            NULL	// Payload
                            );
                            if ( result == NBioAPI_TRUE)
                            std::cout<<"Combinou com o arquivo de texto.\n";
            }
        }

}

void facial_validation(){

}

void open_D1(){

}

void open_D2(){

}

void lock_D1(){

}

void lock_D2(){

}


