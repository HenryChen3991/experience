#include "KnxTpUart.h"

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );

struct KnxTpUart{
	int source_area;
	int source_line;
	int source_member;
	int listen_group_addresses[MAX_LISTEN_GROUP_ADDRESSES][3];
	int listen_group_address_count;
	bool listen_to_broadcasts;
	KnxTelegram tg;
	KnxTelegram tg_ptp;
}knx;

void inizializzaKnxTpUart(int area, int line, int member){
	knx.source_area=area; 
	knx.source_line=line;
	knx.source_member=member;
	knx.listen_group_address_count=0;
	knx.listen_to_broadcasts= false;
}

void setListenToBroadcasts(bool listen) {
    knx.listen_to_broadcasts = listen;
}

void uartReset(int uart) {
	uint8_t sendByte = 0x01;

	uart_putc(sendByte,uart); // New implementation
}

void uartStateRequest(int uart) {
	uint8_t sendByte = 0x02;

	uart_putc(sendByte,uart); // New implementation
}

void setIndividualAddress(int area, int line, int member) {
    knx.source_area = area;
    knx.source_line = line;
    knx.source_member = member;
}

/*
KnxTpUartSerialEventType serialEvent() {
	checkErrors();
		
	int incomingByte;
		
	incomingByte = uart_getc();
		
	if (isKNXControlByte(incomingByte)) {
		bool interested = readKNXTelegram();
		if (interested) {
			return KNX_TELEGRAM;
		} else {
			return IRRELEVANT_KNX_TELEGRAM;
		}
	} else if (incomingByte == TPUART_RESET_INDICATION_BYTE) {
		serialRead();
		return TPUART_RESET_INDICATION;
	} else {
		serialRead();
		return UNKNOWN;
	}

	return UNKNOWN;
}
*/

bool isKNXControlByte(int b) {
	//return ( (b | 0b00101100) == 0b10111100 ); // Ignore repeat flag and priority flag
	return b == 0b10111100;
}

void checkErrors() {
	// TODO checkErrors() routine
}

bool readKNXTelegram() {
	// Receive header
	int i,controlByte = 0;

	while (!isKNXControlByte(controlByte)) 
		controlByte = serialRead();

	setBufferByte(&knx.tg, 0, controlByte);
	for (i = 1; i < 6; i++) {
		setBufferByte(&knx.tg, i, serialRead());
	}

	int bufpos = 6;
	for (i = 0; i < getPayloadLength(&knx.tg); i++) {
		setBufferByte(&knx.tg, bufpos, serialRead());
		bufpos++; 
	}

	// Checksum
	setBufferByte(&knx.tg, bufpos, serialRead());

    // Verify if we are interested in this message - GroupAddress
	bool interested = isTargetGroup(&knx.tg) && isListeningToGroupAddress(getTargetMainGroup(&knx.tg), getTargetMiddleGroup(&knx.tg), getTargetSubGroup(&knx.tg));
    
    // Physical address
    interested = interested || ((!isTargetGroup(&knx.tg)) && getTargetArea(&knx.tg) == knx.source_area && getTargetLine(&knx.tg) == knx.source_line && getTargetMember(&knx.tg) == knx.source_member);
    
    // Broadcast (Programming Mode)
    interested = interested || (knx.listen_to_broadcasts && isTargetGroup(&knx.tg) && getTargetMainGroup(&knx.tg) == 0 && getTargetMiddleGroup(&knx.tg) == 0 && getTargetSubGroup(&knx.tg) == 0);
  
	//if (interested) 
	//	sendAck(UART1);

	// Returns if we are interested in this diagram
	return interested;
}

KnxTelegram getReceivedTelegram() {
	return knx.tg;
}

bool groupWriteBool(int mainGroup, int middleGroup, int subGroup, bool value) {
	int valueAsInt = 0;
	if (value) {
		valueAsInt = 0b00000001;
	}
	
	createKNXMessageFrame(2, KNX_COMMAND_WRITE, mainGroup, middleGroup, subGroup, valueAsInt);
	return sendMessage(UART1);
}

bool groupWrite2ByteFloat(int mainGroup, int middleGroup, int subGroup, float value) {
	createKNXMessageFrame(2, KNX_COMMAND_WRITE, mainGroup, middleGroup, subGroup, 0);
	set2ByteFloatValue(&knx.tg, value);
	createChecksum(&knx.tg);
	return sendMessage(UART1);
}

bool groupAnswerBool(int mainGroup, int middleGroup, int subGroup, bool value) {
	int valueAsInt = 0;
	if (value) {
		valueAsInt = 0b00000001;
	}
	
	createKNXMessageFrame(2, KNX_COMMAND_ANSWER, mainGroup, middleGroup, subGroup, valueAsInt);
	return sendMessage(UART1);
}

bool groupAnswer2ByteFloat(int mainGroup, int middleGroup, int subGroup, float value) {
	createKNXMessageFrame(2, KNX_COMMAND_ANSWER, mainGroup, middleGroup, subGroup, 0);
	set2ByteFloatValue(&knx.tg, value);
	createChecksum(&knx.tg);
	return sendMessage(UART1);
}

bool individualAnswerAddress() {
    createKNXMessageFrame(2, KNX_COMMAND_INDIVIDUAL_ADDR_RESPONSE, 0, 0, 0, 0);
	createChecksum(&knx.tg);
	return sendMessage(UART1);    
}

bool individualAnswerMaskVersion(int area, int line, int member) {
    createKNXMessageFrameIndividual(4, KNX_COMMAND_MASK_VERSION_RESPONSE, area, line, member, 0);
    setCommunicationType(&knx.tg, KNX_COMM_NDP);
    setBufferByte(&knx.tg, 8, 0x07); // Mask version part 1 for BIM M 112
    setBufferByte(&knx.tg, 9, 0x01); // Mask version part 2 for BIM M 112
    createChecksum(&knx.tg);
    return sendMessage(UART1);
}

bool individualAnswerAuth(int accessLevel, int sequenceNo, int area, int line, int member) {
    createKNXMessageFrameIndividual(3, KNX_COMMAND_ESCAPE, area, line, member, KNX_EXT_COMMAND_AUTH_RESPONSE);
    setCommunicationType(&knx.tg, KNX_COMM_NDP);
    setSequenceNumber(&knx.tg, sequenceNo);
    setBufferByte(&knx.tg, 8, accessLevel);
    createChecksum(&knx.tg);
    return sendMessage(UART1);
}

void createKNXMessageFrame(int payloadlength, KnxCommandType command, int mainGroup, int middleGroup, int subGroup, int firstDataByte) {
	clear(&knx.tg);
	setSourceAddress(&knx.tg, knx.source_area, knx.source_line, knx.source_member);
	setTargetGroupAddress(&knx.tg, mainGroup, middleGroup, subGroup);
	setFirstDataByte(&knx.tg, firstDataByte);
	setCommand(&knx.tg, command);
    setPayloadLength(&knx.tg, payloadlength);
	createChecksum(&knx.tg);
}

void createKNXMessageFrameIndividual(int payloadlength, KnxCommandType command, int area, int line, int member, int firstDataByte) {
	clear(&knx.tg);
	setSourceAddress(&knx.tg, knx.source_area, knx.source_line, knx.source_member);
	setTargetIndividualAddress(&knx.tg, area, line, member);
	setFirstDataByte(&knx.tg, firstDataByte);
	setCommand(&knx.tg, command);
    setPayloadLength(&knx.tg, payloadlength);
	createChecksum(&knx.tg);
}

bool sendNCDPosConfirm(int sequenceNo, int area, int line, int member) {
    clear(&knx.tg_ptp);
	setSourceAddress(&knx.tg_ptp, knx.source_area, knx.source_line, knx.source_member);
	setTargetIndividualAddress(&knx.tg_ptp, area, line, member);
	setSequenceNumber(&knx.tg_ptp, sequenceNo);
    setCommunicationType(&knx.tg_ptp, KNX_COMM_NCD);
    setControlData(&knx.tg_ptp, KNX_CONTROLDATA_POS_CONFIRM);
    setPayloadLength(&knx.tg_ptp, 1);
	createChecksum(&knx.tg_ptp);
    
	return sendMessage(UART1);
}

bool sendMessage(int uart) {
	int i,confirmation,messageSize = getTotalLength(&knx.tg);
    uint8_t send;

	turnGPIO(PORTB,0,ON);

	for (i = 0; i < messageSize; i++) {
		send = getBufferByte(&knx.tg, i);
		uart_putc(send,uart);
	}

	confirmation = true;
	turnGPIO(PORTB,0,OFF);
	return confirmation;

	/*
	confirmation = serialRead();
	if (confirmation == 0b11001100) 
		return true; // Sent successfully
	else
		return false;
	*/
}

void sendAck(int uart) {
	uint8_t sendByte = 0b11001100;
	
	uart_putc(sendByte,uart);
}

void sendNack(int uart) {
	uint8_t sendByte = 0b00001100;
	
	uart_putc(sendByte,uart);
}

int serialRead() {
	return uart_getc();
}

void addListenGroupAddress(int main, int middle, int sub) {
	if (knx.listen_group_address_count >= MAX_LISTEN_GROUP_ADDRESSES)
		return;

	knx.listen_group_addresses[knx.listen_group_address_count][0] = main;
	knx.listen_group_addresses[knx.listen_group_address_count][1] = middle;
	knx.listen_group_addresses[knx.listen_group_address_count][2] = sub;

	knx.listen_group_address_count++;
}

bool isListeningToGroupAddress(int main, int middle, int sub) {
	int i;
	for (i = 0; i < knx.listen_group_address_count; i++) {
		if ( (knx.listen_group_addresses[i][0] == main)
				&& (knx.listen_group_addresses[i][1] == middle)
				&& (knx.listen_group_addresses[i][2] == sub)) {
			return true;
		}
	}

	return false;
}

void sendData(int mainGroup, int middleGroup, int subGroup,unsigned char *data,int len,int uart) {
	int i = 0,j = 7;

	turnGPIO(PORTB,1,ON);

	clear(&knx.tg);
	setSourceAddress(&knx.tg, knx.source_area, knx.source_line, knx.source_member);
	setTargetGroupAddress(&knx.tg, mainGroup, middleGroup, subGroup);
	
	while (i < len) {
		if (j < (MAX_KNX_TELEGRAM_SIZE-1)) {
			setBufferByte(&knx.tg,j,(int) data[i]);
			i++; j++;
		}
		else {
			setPayloadLength(&knx.tg,j-6);
			createChecksum(&knx.tg);
			sendMessage(uart);
			j = 7;	
		}
	}	
	setPayloadLength(&knx.tg,j-6);
	createChecksum(&knx.tg);
	sendMessage(uart);

	turnGPIO(PORTB,1,OFF);
}

void sendDataIndividual(int area, int line, int member,
                        unsigned char *data,int len,int uart) {
	int i = 0,j = 7;

	clear(&knx.tg);
	setSourceAddress(&knx.tg, knx.source_area, knx.source_line, knx.source_member);
	setTargetIndividualAddress(&knx.tg, area, line, member);
	
	while (i < len) {
		if (j < (MAX_KNX_TELEGRAM_SIZE-1)) {
			setBufferByte(&knx.tg,j,(int) data[i]);
			i++; j++;
		}
		else {
			setPayloadLength(&knx.tg,j-6);
			createChecksum(&knx.tg);
			sendMessage(uart);
			j = 7;	
		}
	}	
	setPayloadLength(&knx.tg,j-6);
	createChecksum(&knx.tg);
	sendMessage(uart);
}

void getReceivedPayload(unsigned char *payload,int *olen) {
	int i;

	*olen = getPayloadLength(&knx.tg)-1;
	for(i = 0; i < *olen; i++) {
		payload[i] = (unsigned char) getBufferByte(&knx.tg,i+7);
	}	
}

void receiveData(unsigned char *buf,int len) {
	int olen;
	unsigned char payload[15];

	while (len > 0) {	
		if (readKNXTelegram()) {
			turnGPIO(PORTC,1,ON);
			getReceivedPayload(payload,&olen);
			if (olen <= len) {
				memcpy(buf,payload,olen);
				buf += olen;
				len -= olen;
			}
			else {
				memcpy(buf,payload,len);
				len = 0;
			}
			turnGPIO(PORTC,1,OFF);
		}
		else turnGPIO(PORTD,14,ON);
	}
}
