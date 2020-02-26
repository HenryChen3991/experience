#include "KnxTelegram.h"


void clear(KnxTelegram *tg) {
	int i;
	for (i = 0; i < MAX_KNX_TELEGRAM_SIZE; i++) {
		tg->buffer[i] = 0;
	}

	// Control Field, Normal Priority, No Repeat
	tg->buffer[0] = 0b10111100;

	// Target Group Address, Routing Counter = 6, Length = 1 (= 2 Bytes)
	tg->buffer[5] = 0b11100001;
}

int getBufferByte(KnxTelegram *tg, int index) {
	return tg->buffer[index];
}

void setBufferByte(KnxTelegram *tg, int index, int content) {
	tg->buffer[index] = content;
}

bool isRepeated(KnxTelegram *tg) {
	// Parse Repeat Flag
	if (tg->buffer[0] & 0b00100000) {
		return false;
	} else {
		return true;
	}
}

void setRepeated(KnxTelegram *tg, bool repeat) {
	if (repeat) {
		tg->buffer[0] = tg->buffer[0] & 0b11011111;
	} else {
		tg->buffer[0] = tg->buffer[0] | 0b00100000;
	}
}

void setPriority(KnxTelegram *tg, KnxPriorityType prio) {
	tg->buffer[0] = tg->buffer[0] & 0b11110011;
	tg->buffer[0] = tg->buffer[0] | (prio << 2);
}

KnxPriorityType getPriority(KnxTelegram *tg) {
	// Priority
	return (KnxPriorityType) ((tg->buffer[0] & 0b00001100) >> 2);
}

void setSourceAddress(KnxTelegram *tg, int area, int line, int member) {
	tg->buffer[1] = (area << 4) | line;	// Source Address
	tg->buffer[2] = member; // Source Address
}

int getSourceArea(KnxTelegram *tg) {
	return (tg->buffer[1] >> 4);
}

int getSourceLine(KnxTelegram *tg) {
	return (tg->buffer[1] & 0b00001111);
}

int getSourceMember(KnxTelegram *tg) {
	return tg->buffer[2];
}

void setTargetGroupAddress(KnxTelegram *tg, int main, int middle, int sub) {
	tg->buffer[3] = (main << 3) | middle;
	tg->buffer[4] = sub;
	tg->buffer[5] = tg->buffer[5] | 0b10000000;
}

void setTargetIndividualAddress(KnxTelegram *tg, int area, int line, int member) {
	tg->buffer[3] = (area << 4) | line;
	tg->buffer[4] = member;
	tg->buffer[5] = tg->buffer[5] & 0b01111111;
}

bool isTargetGroup(KnxTelegram *tg) {
	return tg->buffer[5] & 0b10000000;
}

int getTargetMainGroup(KnxTelegram *tg) {
	return ((tg->buffer[3] & 0b01111000) >> 3);
}

int getTargetMiddleGroup(KnxTelegram *tg) {
	return (tg->buffer[3] & 0b00000111);
}

int getTargetSubGroup(KnxTelegram *tg) {
	return tg->buffer[4];
}

int getTargetArea(KnxTelegram *tg) {
	return ((tg->buffer[3] & 0b11110000) >> 4);
}

int getTargetLine(KnxTelegram *tg) {
	return (tg->buffer[3] & 0b00001111);
}

int getTargetMember(KnxTelegram *tg) {
	return tg->buffer[4];
}

void setRoutingCounter(KnxTelegram *tg, int counter) {
	tg->buffer[5] = tg->buffer[5] & 0b10000000;
	tg->buffer[5] = tg->buffer[5] | (counter << 4);
}

int getRoutingCounter(KnxTelegram *tg) {
	return ((tg->buffer[5] & 0b01110000) >> 4);
}

void setPayloadLength(KnxTelegram *tg, int length) {
	tg->buffer[5] = tg->buffer[5] & 0b11110000;
	tg->buffer[5] = tg->buffer[5] | (length - 1);
}

int getPayloadLength(KnxTelegram *tg) {
	int length = (tg->buffer[5] & 0b00001111) + 1;
	return length;
}

void setCommand(KnxTelegram *tg, KnxCommandType command) {
	tg->buffer[6] = tg->buffer[6] & 0b11111100;
	tg->buffer[7] = tg->buffer[7] & 0b00111111;

	tg->buffer[6] = tg->buffer[6] | (command >> 2); // Command first two bits
	tg->buffer[7] = tg->buffer[7] | (command << 6); // Command last two bits
}

KnxCommandType getCommand(KnxTelegram *tg) {
	return (KnxCommandType) (((tg->buffer[6] & 0b00000011) << 2) | ((tg->buffer[7] & 0b11000000) >> 6));
}

void setControlData(KnxTelegram *tg, KnxControlDataType cd) {
    tg->buffer[6] = tg->buffer[6] & 0b11111100;
    tg->buffer[6] = tg->buffer[6] | cd;
}

KnxControlDataType getControlData(KnxTelegram *tg) {
    return (KnxControlDataType) (tg->buffer[6] & 0b00000011);
}

KnxCommunicationType getCommunicationType(KnxTelegram *tg) {
    return (KnxCommunicationType) ((tg->buffer[6] & 0b11000000) >> 6);
}

void setCommunicationType(KnxTelegram *tg, KnxCommunicationType type) {
    tg->buffer[6] = tg->buffer[6] & 0b00111111;
    tg->buffer[6] = tg->buffer[6] | (type << 6);
}

int getSequenceNumber(KnxTelegram *tg) {
    return (tg->buffer[6] & 0b00111100) >> 2;
}

void setSequenceNumber(KnxTelegram *tg, int number) {
    tg->buffer[6] = tg->buffer[6] & 0b11000011;
    tg->buffer[6] = tg->buffer[6] | (number << 2);
}

void setFirstDataByte(KnxTelegram *tg, int data) {
	tg->buffer[7] = tg->buffer[7] & 0b11000000;
	tg->buffer[7] = tg->buffer[7] | data;
}

int getFirstDataByte(KnxTelegram *tg) {
	return (tg->buffer[7] & 0b00111111);
}

int calculateChecksum(KnxTelegram *tg) {
	int bcc = 0xFF;
	int size = getPayloadLength(tg) + KNX_TELEGRAM_HEADER_SIZE;
	
	int i;
	for (i = 0; i < size; i++) {
		bcc ^= tg->buffer[i];
	}

	return bcc;
}

void createChecksum(KnxTelegram *tg) {
	int checksumPos = getPayloadLength(tg) + KNX_TELEGRAM_HEADER_SIZE;
	tg->buffer[checksumPos] = calculateChecksum(tg);
}

int getChecksum(KnxTelegram *tg) {
	int checksumPos = getPayloadLength(tg) + KNX_TELEGRAM_HEADER_SIZE;
	return tg->buffer[checksumPos];
}

bool verifyChecksum(KnxTelegram *tg) {
	int calculatedChecksum = calculateChecksum(tg);
	return (getChecksum(tg) == calculatedChecksum);
}

/*
void print(KnxTelegram *tg, FILE *TPUART_SERIAL_CLASS) {
	
	fprintf(TPUART_SERIAL_CLASS,"Repeated: ");
	if (isRepeated(tg)) fprintf(TPUART_SERIAL_CLASS,"true\n");
	else fprintf(TPUART_SERIAL_CLASS,"false\n");

	fprintf(TPUART_SERIAL_CLASS,"Priority: ");
	KnxPriorityType priority;
	priority = getPriority(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", priority);
	
	fprintf(TPUART_SERIAL_CLASS,"Source: ");
	int source_area = getSourceArea(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d", source_area);
	fprintf(TPUART_SERIAL_CLASS,".");
	int source_line = getSourceLine(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d", source_line);
	fprintf(TPUART_SERIAL_CLASS,".");
	int source_member = getSourceMember(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", source_member);

    if (isTargetGroup(tg)) {
		fprintf(TPUART_SERIAL_CLASS,"Target Group: ");
		int targetMainGroup = getTargetMainGroup(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d", targetMainGroup);
		fprintf(TPUART_SERIAL_CLASS,"/");
		int targetMiddleGroup = getTargetMiddleGroup(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d", targetMiddleGroup);
		fprintf(TPUART_SERIAL_CLASS,"/");
		int targetSubGroup = getTargetSubGroup(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d\n", targetSubGroup);       
        
    } else {
		fprintf(TPUART_SERIAL_CLASS,"Target Physical: ");
		int targetArea = getTargetArea(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d", targetArea);
		fprintf(TPUART_SERIAL_CLASS,".");
		int targetLine = getTargetLine(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d", targetLine);
		fprintf(TPUART_SERIAL_CLASS,".");
		int targetMember = getTargetMember(tg);
		fprintf(TPUART_SERIAL_CLASS,"%d\n", targetMember);
    }
    
	fprintf(TPUART_SERIAL_CLASS,"Routing Counter: ");
	int routingCounter = getRoutingCounter(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", routingCounter);
	
	fprintf(TPUART_SERIAL_CLASS,"Payload Length: ");
	int payloadLength = getPayloadLength(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", payloadLength);
	
	fprintf(TPUART_SERIAL_CLASS,"Command: ");
	KnxCommandType command = getCommand(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", command);

	fprintf(TPUART_SERIAL_CLASS,"First Data Byte: ");
	KnxControlDataType firstDataByte = getFirstDataByte(tg);
	fprintf(TPUART_SERIAL_CLASS,"%d\n", firstDataByte);
	
	int i;
	int binario[32];
	int j;
	for (i = 2; i < getPayloadLength(tg); i++) {
		fprintf(TPUART_SERIAL_CLASS,"Data Byte ");
		fprintf(TPUART_SERIAL_CLASS,"%d", i);
		fprintf(TPUART_SERIAL_CLASS,": ");
		
		for (j=0; j<32; j++){
		binario[32-1-j]=tg->buffer[6+i]%2;
		tg->buffer[6+i]=tg->buffer[6+i]/2;
		}
		for(j=0; j<32;j++){ 
		fprintf(TPUART_SERIAL_CLASS, "%d (BIN)", binario[j]); 
		}
		fprintf(TPUART_SERIAL_CLASS, "\n"); 
	}

	if (verifyChecksum(tg)) {
		fprintf(TPUART_SERIAL_CLASS, "Checksum matches\n");
	} else {
		fprintf(TPUART_SERIAL_CLASS, "Checksum mismatch\n");
		int checksum;
		checksum=getChecksum(tg);
		for (j=0; j<32; j++){
		binario[32-1-j]=checksum%2;
		checksum=checksum/2;
		}
		for(j=0; j<32;j++){ 
		fprintf(TPUART_SERIAL_CLASS, "%d (BIN)", binario[j]); 
		}
		fprintf(TPUART_SERIAL_CLASS, "\n"); 
		
		int bcc;
		bcc=calculateChecksum(tg);
		for (j=0; j<32; j++){
		binario[32-1-j]=bcc%2;
		bcc=bcc/2;
		}
		for(j=0; j<32;j++){ 
		fprintf(TPUART_SERIAL_CLASS, "%d (BIN)", binario[j]); 
		}
		fprintf(TPUART_SERIAL_CLASS, "\n"); 
	}

}
*/

int getTotalLength(KnxTelegram *tg) {
	return KNX_TELEGRAM_HEADER_SIZE + getPayloadLength(tg) + 1;
}

void set2ByteFloatValue(KnxTelegram *tg, float value) {
	setPayloadLength(tg, 4);

	int v = value * 100.0;

	if (v < 0) {
		tg->buffer[8] = 0b10000000;
		v = abs(v);
	} else {
		tg->buffer[8] = 0b00000000;
	}

	int exponent = 0;
	while (0xF800 & v) {
		v >>= 1;
		exponent += 1;
	}

	tg->buffer[8] = tg->buffer[8] | ((0b1111 & exponent) << 3);
	tg->buffer[8] = tg->buffer[8] | (0b00000111 & (v >> 8));
	tg->buffer[9] = v;
}

/*
float get2ByteFloatValue(KnxTelegram *tg) {
	if (getPayloadLength(tg) != 4) {
		// Wrong payload length
		return 0;
	}

	int exponent = (tg->buffer[8] & 0b01111000) >> 3;
	int mantissa = ((tg->buffer[8] & 0b00000111) << 8) | (tg->buffer[9]);

	int sign = 1;

	if (tg->buffer[8] & 0b10000000) {
		sign = -1;
	}

	return (mantissa * 0.01) * pow(2.0, exponent);
}*/

