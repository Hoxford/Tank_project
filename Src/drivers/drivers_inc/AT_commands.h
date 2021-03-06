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

//general test; basic
#define AT_CMD_TEST         "AT\r\n"

//restart the module; basic
#define AT_CMD_RESET        "AT+RST\r\n"

//check firmware version; basic
#define AT_CMD_FW_VER       "AT+GMR"

//wifi mode
#define AT_CMD_SET_WIFI_MODE    "AT+CWMODE=%d\r\n"
#define AT_CMD_INQ_WIFI_MODE    "AT+CWMODE?"
#define AT_CMD_TST_WIFI_MODE    "AT+CWMODE=?"
    #define AT_CMD_WIFI_MODE_STA         1  //Sta is the default mode of router,
    #define AT_CMD_WIFI_MODE_AP          2  //AP is a normal mode for devices
    #define AT_CMD_WIFI_MODE_STA_AND_AP  3
  #define AT_CMD_WIFI_MODE  AT_CMD_WIFI_MODE_STA
	
//join the AP;
#define AT_CMD_JOIN_AP      "AT+CWJAP=\"%s\",\"%s\"\r\n" //join the AP;wifi; AT+ CWJAP =<ssid>,< pwd >;AT+ CWJAP?;ssid = ssid, pwd = wifi password
#define AT_CMD_INQ_AP       "AT+CWJAP?\r\n"

//list the AP;wifi;AT+CWLAP
#define AT_CMD_LST_AP       "AT+CWLAP\r\n"

//quit the AP;wifi; AT+CWQAP;AT+CWQAP=?;
#define AT_CMD_QUIT_AP      "AT+CWQAP"
#define AT_CMD_TST_QUIT_AP  "AT+CWQAP=?"

//set the parameters of AP;wifi; AT+ CWSAP= <ssid>,<pwd>,<chl>, <ecn>; AT+ CWSAP?;ssid, pwd, chl = channel, ecn = encryption; eg. Connect to your router: AT+CWJAP="www.electrodragon.com","helloworld"; and check if connected: AT+CWJAP?
#define AT_CMD_SET_PARAM_AP       "AT+CWSAP=%s,%s,%s,%s"
#define AT_CMD_INQ_PARAM_AP       "AT+CWSAP?"

//check join devices' IP;wifi; AT+CWLIF;
#define AT_CMD_CHK_JOIN_DEV      "AT+CWLIF"

//get the connection status;TCP/IP; AT+ CIPSTATUS;<id>,<type>,<addr>,<port>,<tetype>= client or server mode
#define AT_CMD_CONN_STAT    "AT+CIPSTATUS"

//set up TCP or UDP connection;TCP/IP;1)single connection (+CIPMUX=0) AT+CIPSTART= <type>,<addr>,<port>; 2) multiple connection (+CIPMUX=1) AT+CIPSTART= <id><type>,<addr>, <port>; AT+CIPSTART=?;id = 0-4, type = TCP/UDP, addr = IP address, port= port; eg. Connect to another TCP server, set multiple connection first: AT+CIPMUX=1; connect: AT+CIPSTART=4,"TCP","X1.X2.X3.X4",9999
#define AT_CMD_SET_SNGL_PCONN      "AT+CIPSTART=%s,%s,%s"
#define AT_CMD_SET_MTPL_PCONN      "AT+CIPSTART=%s,%s,%s,%s"
    #define AT_CMD_SET_PCONN_ID0    0
    #define AT_CMD_SET_PCONN_ID1    1
    #define AT_CMD_SET_PCONN_ID2    2
    #define AT_CMD_SET_PCONN_ID3    3
    #define AT_CMD_SET_PCONN_ID4    4
  #define AT_CMD_SET_PCONN_ID    AT_CMD_SET_PCONN_ID0
    #define AT_CMD_SET_TYPE_TCP  "TCP"
    #define AT_CMD_SET_TYPE_UDP  "UDP"
  #define AT_CMD_SET_PCONN_TYPE    AT_CMD_SET_TYPE_TCP
#define AT_CMD_TST_PCONN      "AT+CIPSTART=?"

//set data transmission mode;TCP/IP; AT+CIPMODE=<mode>; AT+CIPSEND?;0 not data mode, 1 data mode; return "Link is builded"
#define AT_CMD_SET_DTX_MD   "AT+CIPMODE=%s"
    #define AT_CMD_SET_DTX_NO_DATA  0
    #define AT_CMD_SET_DTX_DATA     1
  #define AT_CMD_DTX_MODE    AT_CMD_SET_DTX_DATA
#define AT_CMD_INQ_DTX_MD   "AT+CIPSEND?"

//send data;TCP/IP;1)single connection(+CIPMUX=0) AT+CIPSEND=<length>; 2) multiple connection (+CIPMUX=1) AT+CIPSEND= <id>,<length>; AT+CIPSEND=?;eg. send data: AT+CIPSEND=4,15 and then enter the data.
#define AT_CMD_SINGLE_SEND         "AT+CIPSEND=%s"
#define AT_CMD_MULTI_SEND          "AT+CIPSEND=%s,%s"
    #define AT_CMD_MULTI_SEND_ID0    0
    #define AT_CMD_MULTI_SEND_ID1    1
    #define AT_CMD_MULTI_SEND_ID2    2
    #define AT_CMD_MULTI_SEND_ID3    3
    #define AT_CMD_MULTI_SEND_ID4    4
  #define AT_CMD_MULTI_SEND_ID     AT_CMD_MULTI_SEND_ID0
#define AT_CMD_TST_SEND         "AT+CIPSEND=?"

//close TCP or UDP connection;TCP/IP; AT+CIPCLOSE=<id> or AT+CIPCLOSE; AT+CIPCLOSE=?
#define AT_CMD_CLOSE_PCONN      "AT+CIPCLOSE"
#define AT_CMD_TST_CLOSE_PCONN  "AT+CIPCLOSE=?"

//Get IP address;TCP/IP; AT+CIFSR; AT+ CIFSR=?
#define AT_CMD_GET_IP_ADD   "AT+CIFSR\r\n"
#define AT_CMD_TST_IP_ADD   "AT+CIFSR=?\r\n"

//set multiple connection;TCP/IP; AT+ CIPMUX=<mode>; AT+ CIPMUX?;0 for single connection 1 for multiple connection
#define AT_CMD_SET_MCONN    "AT+CIPMUX=%s"
    #define AT_CMD_MCONN_SINGLE  0
    #define AT_CMD_MCONN_MULTI   1
  #define AT_CMD_MCONN_MODE  AT_CMD_MCONN_SINGLE
#define AT_CMD_INQ_MCONN    "AT+CIPMUX?"

//set as server;TCP/IP; AT+ CIPSERVER= <mode>[,<port> ];mode 0 to close server mode, mode 1 to open; port = port; eg. turn on as a TCP server: AT+CIPSERVER=1,8888, check the self server IP address: AT+CIFSR=?;
#define AT_CMD_SET_SERVER_OPEN    "AT+CIPSERVER=1,%s"
#define AT_CMD_SET_SERVER_CLOSE   "AT+CIPSERVER=0"

//Set the server timeout; AT+CIPSTO=<time>; AT+CIPSTO?;<time>0~28800 in second;
#define AT_CMD_SET_TIMEOUT  "AT+CIPSTO=%s"
#define AT_CMD_INQ_TIMEOUT  "AT+CIPSTO?"

//received data;For Single Connection mode(CIPMUX=0): + IPD, <len>:;For Multi Connection mode(CIPMUX=1): + IPD, <id>, <len>: <data>
#define RCV_DATA "+IPD"

//change baud
#define AT_CMD_SET_CIO_BAUD  "AT+CIOBAUD=%s\r\n"
    #define AT_CMD_CIO_BAUD_115K  "115200"
    #define AT_CMD_CIO_BAUD_57K   "57600"
    #define AT_CMD_CIO_BAUD_9K    "9600"
  #define AT_CMD_CIO_BAUD  AT_CMD_CIO_BAUD_115K

//AT+UART=<Param>,<Param2>,<Param3> OK  Param1: Baud Param2: Stop bit Param3: Parity
//AT+UART?  +UART=<Param>,<Param2>, <Param3> OK Param1: Baud Param2: Stop bit Param3: Parity
#define AT_CMD_HC05_SET_UART  "AT+UART=%s,%s,%s\r\n"
  //param1 uart baud
    #define AT_CMD_HC05_SET_UART_BAUD_1382K   "1382400"
    #define AT_CMD_HC05_SET_UART_BAUD_921K    "921600"
    #define AT_CMD_HC05_SET_UART_BAUD_460K    "460800"
    #define AT_CMD_HC05_SET_UART_BAUD_230K    "230400"
    #define AT_CMD_HC05_SET_UART_BAUD_115K    "115200"
    #define AT_CMD_HC05_SET_UART_BAUD_57K     "57600"
    #define AT_CMD_HC05_SET_UART_BAUD_38K     "38400"
    #define AT_CMD_HC05_SET_UART_BAUD_9K      "9600"
  //param2 stop bit
    #define AT_CMD_HC05_SET_UART_STOP_BIT_1   "0"
    #define AT_CMD_HC05_SET_UART_STOP_BIT_2   "1"
  //param3 parity
    #define AT_CMD_HC05_SET_UART_PARITY_NONE  "0"
    #define AT_CMD_HC05_SET_UART_PARITY_ODD   "1"
    #define AT_CMD_HC05_SET_UART_PARITY_EVEN  "2"
    #define AT_CMD_HC05_SET_UART_PARITY_MARK  "3"

#define AT_CMD_HC05_GET_UART_BAUD  "AT+UART?"

//set echo
#define AT_ECHO_OFF       "ATE0\r\n"
#define AT_ECHO_ON     "ATE1\r\n"

//get device mac address
#define AT_CMD_GET_ADDR  "AT+ADDR?\r\n"

//get device name
#define AT_CMD_GET_NAME  "AT+NAME?\r\n"

//set device name
#define AT_CMD_SET_NAME  "AT+NAME=%s\r\n"

//set factory defaults
#define AT_FACTORY_DEFAULT  "AT&F0\r\n"

//OK response
#define AT_OK    "\r\nOK\r\n"

//ERROR response
#define AT_ERROR "ERROR\r\n"
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
