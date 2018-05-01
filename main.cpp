/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: Leandro
 *
 *
 */

#include <stdio.h>
#include <iostream>
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
#include <time.h>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
//#include <boost/algorithm/string.hpp>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"

#include "ClientTCP.h"
#include "libs/NBioAPI.h"
#include "libs/NBioAPI_Type.h"
#include "display.h"
#include "servertcp.h"
#include <sys/types.h>
#include <sys/wait.h>
/*
#include "libs/mysql.h"
#include "libs/errmsg.h"
#include "libs/driver/mysql_driver.h"
#include "libs/driver/mysql_connection.h"
#include "libs/cppconn/driver.h"
#include "libs/cppconn/exception.h"
#include "libs/cppconn/resultset.h"
#include "libs/cppconn/statement.h"
#include <libs/cppconn/prepared_statement.h>
*/

#define SQL false
#define UI false


using namespace std;

// Conexão via socket
int SERVER_PORT = 8080;
int SOCKET_PORT = 1444;
std::string SOCKET_IP = "192.168.25.5";


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

//const int P_COIL1; // Solenoide
//const int P_COIL2; // Solenoide
//const int P_BUTTON; // Botão de saída
//const int P_BUTTON_D1; // Barreira porta 1
//const int P_BUTTON_D2; // Barreira porta 2
const int P_SIREN = 0; // Sirene -> PINO 11 FÍSICO
//const int P_OLED; // LCD OLED
//const int P_LOAD_CLK;
//const int P_LOAD_DATA;


//const char  * S_MSG_SQL;

/*S for System*/
int S_VALID = 0;
bool S_EMRG = false;
bool S_ADM = false;
std::string S_NPWD;
std::string S_NCPF;
std::string S_NWGT;
std::string S_NF1;
std::string S_NF2;
std::string S_NF3;


/*A for Access*/

std::string A_SOS = "3246"; //Senha de emergencia
std::string A_ADM = "9845"; //Senha do admin -- apenas offline
std::string A_ADG = "AQAAABQAAADkAAAAAQASAAMAZAAAAAAA4AAAAERyVQOa/g54gumeVN0a7O8YKuLIZsaOVvK8M5w*MPdAxurkWAdVC4QdgXHKzNRvaS45Jp/UKVkj3YyBqAQ4cHeZagpji5wumrxu8ZE3IjUcQe36rVP5sbnpTe/n/gdtDDx48cggRf8ZMmEnRbxyz*qH3ff8V/exxO4P8yxYT84hcYyzqV/y*8j*F3wEAkS310*We*yo9djnRYl8XN2NP07nbg*H18UlJG1hSZ6va/ygRq2F8MW0ttxI3B4vWg2AymSCCC33d4PcOtgyIFilKLcMSqHQrkJAxxCB8dRypysX";

Display display(DISPLAY_SSD1306);
bool isDetected = display.isDetected();
ServerTCP stc;

void start(); // INICIA O SISTEMA
void online(ClientTCP ctc); // SISTEMA ONLINE COM A ESTAÇÃO-BASE
void offline(ClientTCP ctc); // SISTEMA OFFLINE, TENTA RECONECTAR COM A ESTAÇÃO-BASE
void writeLog(std::string A_ID); // MANTÉM NUM LOG A ENTRADA E SAÍDA DE PESSOAS
int sendMessage(ClientTCP ctc,std::string A_VALID); // ENVIA COMANDOS PARA A ESTAÇÃO-BASE
//int sendLog(); // ENVIA O LOG PARA A ESTAÇÃO-BASE
void pwd_validation(ClientTCP ctc, std::string S_PWD); // REALIZA A VALIDAÇÃO DA SENHA
void weight_validation(ClientTCP ctc); // REALIZA A VALIDAÇÃO DO PESO
void finger_validation(ClientTCP ctc,std::string fnp); // REALIZA A VALIDAÇÃO DA DIGITAL
void facial_validation(ClientTCP ctc); // REALIZA A VALIDAÇÃO FACIAL
void open_D1(); // ABRE A PORTA 1
void open_D2(); // ABRE A PORTA 2
void lock_D1(); // FECHA A PORTA 1
void lock_D2(); // FECHA A PORTA 2
void initFNP(ClientTCP ctc);
void adm_validation(ClientTCP ctc);
void newUser();
void setupServer();

std::string actualTime(); // FUNÇÃO QUE RETORNA A DATA ATUAL

int main(int argc, char *argv[]) {


    wiringPiSetup();

    pinMode(P_SIREN, OUTPUT);
    digitalWrite(P_SIREN,LOW);

    /*pinMode(P_OPT1, INPUT);
    pinMode(P_OPT2, INPUT);
    pinMode(P_BUTTON, INPUT);
    pinMode(P_PIC,OUTPUT);
    pinMode(P_BUZZ, OUTPUT);
    pinMode(P_LED_ON, OUTPUT);
    pinMode(P_LED_OFF, OUTPUT);	 	
    digitalWrite(P_PIC,HIGH);
    digitalWrite(P_COIL, HIGH);
    */




        if (isDetected) {

            display.PutString("Digite a sua senha: ");
            display.SetCursorPos(0,1); // Primeira coluna [0] segunda linha [1]


         }



    //thread t(start); // Apenas para interface gráfica
    //start();
       start();

}

void start(){

    ClientTCP ctc(SOCKET_PORT,SOCKET_IP);
    if(ctc.getConnected()==true)
    online(ctc);
    else
    offline(ctc);

}



void online(ClientTCP ctc){

    //thread t(setupServer);
    string S_PWD;
    cout<<"Server criado!"<<endl;


	while(true){

        std::getline(std::cin,S_PWD);
        display.ClearLine(0);
        display.PutString("Autenticando...");
        pwd_validation(ctc,S_PWD);

}

}

void offline(ClientTCP ctc){
   cout<<"MODO OFFLINE";
    while(ctc.connected==false)
   ctc.rebind();

   online(ctc);
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




void pwd_validation(ClientTCP ctc,string S_PWD){

    int A_VALID;
    int messageSize = S_PWD.size();
    int lengthOfBytes = S_PWD.length()+1;
    if(messageSize!=4) return;
    char * pwd = new char [lengthOfBytes];
    char * sos = new char [4];
    char * adm = new char [4];
    strcpy (pwd, S_PWD.c_str());
    strcpy (sos, A_SOS.c_str());
    strcpy (adm, A_ADM.c_str());
    if((strcmp(pwd,sos))==0){

    S_PWD = "0" + S_PWD;
    }
    else if((strcmp(pwd,adm))==0){
    if(ctc.getConnected()==false) adm_validation(ctc);
    else{
        cout<<"AUTENTICACAO FALHOU"<<endl;
        display.Cls();
        display.SetCursorPos(0,0);
        display.PutString("Acesso negado");
        return;
    }
    }
    else{

    S_PWD = "1" + S_PWD;
}

    A_VALID = ctc.sendMessageToServer(S_PWD);
    cout<<"RETORNO: "<<A_VALID<<endl;
    if(A_VALID==2){
        //Abre a porta -> MODO HELP
        cout<<"Abrindo PORTA - EMERGENCIA"<<endl;
        S_EMRG = true;
        display.Cls();
        display.SetCursorPos(0,0);
        display.PutString("Autenticando...");
        display.Cls();
        display.SetCursorPos(0,0);
        display.PutString("Acesso confirmado");
        weight_validation(ctc);
    }
    else if(A_VALID==1){
        //Abre a porta -> MODO NORMAL
        cout<<"Abrindo PORTA - NORMAL"<<endl;
        display.Cls();
        display.SetCursorPos(0,0);
        display.PutString("Acesso confirmado");
        weight_validation(ctc);

    }
    else if(A_VALID==0){
        //Nao abre a porta
        cout<<"AUTENTICACAO FALHOU"<<endl;
        display.Cls();
        display.SetCursorPos(0,0);
        display.PutString("Acesso negado");
        return;
    }
    return;
}

void weight_validation(ClientTCP ctc){

    //Recebe dados da balança na string S_WGT em formato inteiro
    if(S_EMRG == true) {
        //Aguarda receber dados da balança

        //Fecha porta -> Aguarda reed switch
        digitalWrite(P_SIREN,HIGH);

    }

    std::string S_WGT = "2";
    S_WGT += "106";
    cout<<"VALIDANDO PESO..."<<endl;
    int A_VALID = ctc.sendMessageToServer(S_WGT);
    if(A_VALID == 1){
    cout<<"PESO VALIDADO"<<endl;
    thread t1(system,"mpg123 /home/pi/msg_peso_validado.mp3");
    t1.join();
    sleep(1);
    cout<<"FECHANDO PORTA"<<endl;
    sleep(2);
    cout<<"PORTA FECHADA"<<endl;
    ctc.sendMessageToServer("3");
    std::string fnp = ctc.resposta;
    finger_validation(ctc,fnp);
    }
    else if(A_VALID == 0){
        //Inicia mensagem de audio
        cout<<"Peso não confirmado"<<endl;
        thread t1(system,"mpg123 /home/pi/msg_peso_incorreto.mp3");
        t1.join();
        for(int i=0;i<2;i++){
            A_VALID = ctc.sendMessageToServer(S_WGT);
            if(A_VALID == 1){
            cout<<"PESO VALIDADO"<<endl;
            thread t1(system,"mpg123 /home/pi/msg_peso_validado.mp3");
            t1.join();
            sleep(1);
            cout<<"FECHANDO PORTA"<<endl;
            sleep(2);
            cout<<"PORTA FECHADA"<<endl;
            ctc.sendMessageToServer("3");
            std::string fnp = ctc.resposta;
            finger_validation(ctc,fnp);
            }
            else{
                thread t1(system,"mpg123 /home/pi/msg_peso_incorreto.mp3");
                t1.join();
                }
        }
        return;
        }
    }









void finger_validation(ClientTCP ctc,std::string fnp){
    for(int k=0;k<3;k++){

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

               /* NBioAPI_FIR_TEXTENCODE g_firText; // Text encoded FIR

                            // Get Text encoded FIR from FIR handle
                            ret = NBioAPI_GetTextFIRFromHandle(g_hBSP,g_hCapturedFIR,&g_firText,NBioAPI_FALSE);


                            if ( ret == NBioAPIERROR_NONE ){
                                char* text_stream;
                                int length;
                                length = strlen(g_firText.TextFIR);
                                std::cout<<length;
                                if (g_firText.IsWideChar == NBioAPI_TRUE)
                                text_stream = new char [(length + 1)*2];
                                else
                                text_stream = new char [length + 1];

                                memcpy(text_stream, g_firText.TextFIR, length + 1);

                                // Save text_stream to File or Database
                                std::ofstream fpdata;
                                fpdata.open ("./DigitalString.txt");
                                fpdata << text_stream;
                                fpdata.close();

                                delete [] text_stream;
                            }

                            NBioAPI_FreeTextFIR(g_hBSP, &g_firText); // Free TextFIR handle

                            std::cout<<"Digital gravada com sucesso \n\n.";*/

                NBioAPI_BOOL result;
                            NBioAPI_INPUT_FIR storedFIR, inputFIR;

                            // Read stored data and convert into FIR(fir1)
                            //std::ifstream t("DigitalString.txt");
                            //std::string text_stream((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
                            cout<<"Fingerprint: "<<fnp<<endl;
                            int length;

                            //length = strlen(text_stream.c_str());
                            length = strlen(fnp.c_str());

                            NBioAPI_FIR_TEXTENCODE g_storedTextFIR; // Set input FIR.
                            g_storedTextFIR.IsWideChar = NBioAPI_FALSE;			// It depends on application
                            //g_storedTextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
                            g_storedTextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
                            //memcpy(g_storedTextFIR.TextFIR, text_stream.c_str(), length + 1);
                            memcpy(g_storedTextFIR.TextFIR, fnp.c_str(), length + 1);

                            storedFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // stored FIR
                            storedFIR.InputFIR.TextFIR = &g_storedTextFIR;

                            // Read input stream and convert into FIR(fir2)

                            // Text encode the FIR generated
                            NBioAPI_FIR_TEXTENCODE g_newTextFIR; // Text encoded FIR

                            // Get Text encoded FIR from FIR handle
                            ret = NBioAPI_GetTextFIRFromHandle(g_hBSP, g_hCapturedFIR, &g_newTextFIR, NBioAPI_FALSE);

                            inputFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // input FIR to be compared
                            inputFIR.InputFIR.TextFIR = &g_newTextFIR;
                            //inputFIR.InputFIR.TextFIR = &g_storedTextFIR;
                            cout<<"INPUT FIR: "<<g_newTextFIR.TextFIR<<endl;

                            ret = NBioAPI_VerifyMatch( // Matching use with stored FIR
                            g_hBSP,		// Handle of NBioBSP module
                            &storedFIR,	// stored FIR
                            &inputFIR,	// input FIR for matching
                            &result,	// Result of matching
                            NULL	// Payload
                            );
                            if (result == NBioAPI_TRUE){
                            std::cout<<"Combinou com a entrada.\n";
                            thread t1(system,"mpg123 /home/pi/msg_digital_confirmada.mp3");
                            t1.join();

                            if(S_ADM == false) facial_validation(ctc);
                            else return;
                            return;
                            }
                            else{
                            cout<<"Autenticacao por digital falhou"<<endl<<"Tente novamente"<<endl;
                            thread t1(system,"mpg123 /home/pi/msg_digital_incorreta.mp3");
                            t1.join();
                            }
                            ret = NBioAPI_Terminate(g_hBSP);
                            }
            }

        }
    thread t1(system,"mpg123 /home/pi/tentativas_max.mp3");
    t1.join();
return;
}

void facial_validation(ClientTCP ctc){
    //Executa procedimento para tirar foto com a raspicam
    cout<<"Aguardando para tirar foto"<<endl;
    //Envia foto

    while(1);

}

void open_D1(){

}

void open_D2(){

}

void lock_D1(){

}

void lock_D2(){

}

void newUser(ClientTCP ctc){

    while(stc.getConnecting()==false);
    int A_VALID;
    A_VALID = ctc.sendMessageToServer(S_NPWD);
    if(A_VALID==0) {stc.setNPWD();
        cout<<"Procedendo com auth"<<endl;
    }
    else return;
}

void adm_validation(ClientTCP ctc){
    S_ADM = true;
    cout<<"Bem vindo, administrador";
    finger_validation(ctc,A_ADG);
    //Abre a porta

}
void setupServer(){
    stc = ServerTCP(SERVER_PORT);
}
