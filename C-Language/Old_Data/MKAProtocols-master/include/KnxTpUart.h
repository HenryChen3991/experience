#ifndef KnxTpUart_h
#define KnxTpUart_h

#include "KnxTelegram.h"
#include "stm32f4d_helper.h"

// Services from TPUART
#define TPUART_RESET_INDICATION_BYTE 0b11

// Services to TPUART
#define TPUART_DATA_START_CONTINUE 0b10000000
#define TPUART_DATA_END 0b01000000

// Maximum number of group addresses that can be listened on
#define MAX_LISTEN_GROUP_ADDRESSES 5

typedef int bool;
#define true 1
#define false 0

typedef unsigned char 	uint8_t;

typedef enum {
	TPUART_RESET_INDICATION,
	KNX_TELEGRAM,
	IRRELEVANT_KNX_TELEGRAM,
	UNKNOWN
}KnxTpUartSerialEventType;

	void inizializzaKnxTpUart(int, int, int);
	void uartReset(int uart);
	void uartStateRequest(int uart);
	//KnxTpUartSerialEventType serialEvent();
	KnxTelegram getReceivedTelegram();
	
    void setIndividualAddress(int, int, int);
    
	void sendAck(int uart);
	void sendNack(int uart);
	
	bool groupWriteBool(int, int, int, bool);
	bool groupWrite2ByteFloat(int, int, int, float);
	
	bool groupAnswerBool(int, int, int, bool);
	bool groupAnswer2ByteFloat(int, int, int, float);

	void addListenGroupAddress(int, int, int);
	bool isListeningToGroupAddress(int, int, int);
    
    bool individualAnswerAddress();
    bool individualAnswerMaskVersion(int, int, int);
    bool individualAnswerAuth(int, int, int, int, int);
    
    void setListenToBroadcasts(bool);
    
    

	
	bool isKNXControlByte(int);
	void checkErrors();
	bool readKNXTelegram();
	void createKNXMessageFrame(int, KnxCommandType, int, int, int, int);
	void createKNXMessageFrameIndividual(int, KnxCommandType, int, int, int, int);
	bool sendMessage(int uart);
    bool sendNCDPosConfirm(int, int, int, int);
	int serialRead();
	void sendData(int mainGroup, int middleGroup, int subGroup,unsigned char *data,int len,int uart);
	void sendDataIndividual(int area, int line, int member, unsigned char *data,int len,int uart);
	void getReceivedPayload(unsigned char *payload,int *olen);
	void receiveData(unsigned char *buf,int len);



#endif
