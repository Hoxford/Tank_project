/******************************************************************************
*
* AT_commands.h - 1 sentence file description
* Copyright (c) notice
*
******************************************************************************/
#ifndef __AT_COMMANDS_H__
#define __AT_COMMANDS_H__
/******************************************************************************
* includes ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public defines ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#define AT_CMD_TEST         "AT"     //general test; basic
#define AT_CMD_RESET        "AT+RST" //restart the module; basic
#define AT_CMD_FW_VER       "AT+GMR" //check firmware version; basic

 //wifi mode
#define AT_CMD_SET_WIFI_MODE    "AT+CWMODE=%n"
#define AT_CMD_INQ_WIFI_MODE    "AT+CWMODE?"
#define AT_CMD_TST_WIFI_MODE    "AT+CWMODE=?"
  #define AT_CMD_WIFI_MODE_STA         1  //Sta is the default mode of router, 
  #define AT_CMD_WIFI_MODE_AP          2  //AP is a normal mode for devices
  #define AT_CMD_WIFI_MODE_STA_AND_AP  3
	
//join the AP;
#define AT_CMD_JOIN_AP      "AT+CWJAP=%s,%s" //join the AP;wifi; AT+ CWJAP =<ssid>,< pwd >;AT+ CWJAP?;ssid = ssid, pwd = wifi password
#define AT_CMD_INQ_AP       "AT+CWJAP?"

#define AT_CMD_LST_AP       "AT+CWLAP"//list the AP;wifi;AT+CWLAP
#define AT_CMD_QUIT_AP      "AT+CWQAP"//quit the AP;wifi; AT+CWQAP;AT+CWQAP=?;
#define AT_CMD_SET_AP       "AT+CWSAP"//set the parameters of AP;wifi; AT+ CWSAP= <ssid>,<pwd>,<chl>, <ecn>; AT+ CWSAP?;ssid, pwd, chl = channel, ecn = encryption; eg. Connect to your router: AT+CWJAP="www.electrodragon.com","helloworld"; and check if connected: AT+CWJAP?
#define AT_CMD_JOIN_AP      "AT+CWLIF"//check join devices' IP;wifi; AT+CWLIF;
#define AT_CMD_CONN_STAT    "AT+CIPSTATUS"//get the connection status;TCP/IP; AT+ CIPSTATUS;<id>,<type>,<addr>,<port>,<tetype>= client or server mode
#define AT_CMD_SET_TCP      "AT+CIPSTART"//set up TCP or UDP connection;TCP/IP;1)single connection (+CIPMUX=0) AT+CIPSTART= <type>,<addr>,<port>; 2) multiple connection (+CIPMUX=1) AT+CIPSTART= <id><type>,<addr>, <port>; AT+CIPSTART=?;id = 0-4, type = TCP/UDP, addr = IP address, port= port; eg. Connect to another TCP server, set multiple connection first: AT+CIPMUX=1; connect: AT+CIPSTART=4,"TCP","X1.X2.X3.X4",9999
#define AT_CMD_SET_UDP      "AT+CIPSTART"//set up TCP or UDP connection;TCP/IP;1)single connection (+CIPMUX=0) AT+CIPSTART= <type>,<addr>,<port>; 2) multiple connection (+CIPMUX=1) AT+CIPSTART= <id><type>,<addr>, <port>; AT+CIPSTART=?;id = 0-4, type = TCP/UDP, addr = IP address, port= port; eg. Connect to another TCP server, set multiple connection first: AT+CIPMUX=1; connect: AT+CIPSTART=4,"TCP","X1.X2.X3.X4",9999
#define AT_CMD_SET_DTX_MD   "AT+CIPMODE"//set data transmission mode;TCP/IP; AT+CIPMODE=<mode>; AT+CIPSEND?;0 not data mode, 1 data mode; return "Link is builded"
#define AT_CMD_SEND         "AT+CIPSEND"//send data;TCP/IP;1)single connection(+CIPMUX=0) AT+CIPSEND=<length>; 2) multiple connection (+CIPMUX=1) AT+CIPSEND= <id>,<length>; AT+CIPSEND=?;eg. send data: AT+CIPSEND=4,15 and then enter the data.
#define AT_CMD_CLOSE_TCP    "AT+CIPCLOSE"//close TCP or UDP connection;TCP/IP; AT+CIPCLOSE=<id> or AT+CIPCLOSE; AT+CIPCLOSE=?
#define AT_CMD_GET_IP_ADD   "AT+CIFSR"//Get IP address;TCP/IP; AT+CIFSR; AT+ CIFSR=?
#define AT_CMD_SET_MCONN    "AT+CIPMUX"//set mutiple connection;TCP/IP; AT+ CIPMUX=<mode>; AT+ CIPMUX?;0 for single connection 1 for multiple connection
#define AT_CMD_SET_SERVER   "AT+CIPSERVER"//set as server;TCP/IP; AT+ CIPSERVER= <mode>[,<port> ];mode 0 to close server mode, mode 1 to open; port = port; eg. turn on as a TCP server: AT+CIPSERVER=1,8888, check the self server IP address: AT+CIFSR=?;
#define AT_CMD_SET_TIMEOUT  "AT+CIPSTO"//Set the server timeout; AT+CIPSTO=<time>; AT+CIPSTO?;<time>0~28800 in second;
#define +IPD//received data;For Single Connection mode(CIPMUX=0): + IPD, <len>:;For Multi Connection mode(CIPMUX=1): + IPD, <id>, <len>: <data>
/******************************************************************************
*public variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public enums /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
*public structures ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* external functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

/******************************************************************************
* public functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
******************************************************************************/

#endif //__AT_COMMANDS_H__