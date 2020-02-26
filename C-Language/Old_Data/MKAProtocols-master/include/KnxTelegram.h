//#include<math.h>

#ifndef KnxTelegram_h
#define KnxTelegram_h

#define MAX_KNX_TELEGRAM_SIZE 23
#define KNX_TELEGRAM_HEADER_SIZE 6

typedef int bool;
#define true 1
#define false 0

#define abs(n) (n >= 0) ? n : (-1*n)

// KNX priorities - Link layer control field
typedef enum {
	KNX_PRIORITY_SYSTEM = 0b00,
	KNX_PRIORITY_ALARM = 0b10,
	KNX_PRIORITY_HIGH = 0b01,
	KNX_PRIORITY_NORMAL = 0b11
}KnxPriorityType;

// KNX commands / APCI Coding - Application Layer
typedef enum {
	KNX_COMMAND_READ = 0b0000,
	KNX_COMMAND_WRITE = 0b0010,
	KNX_COMMAND_ANSWER = 0b0001,
    KNX_COMMAND_INDIVIDUAL_ADDR_WRITE = 0b0011,
    KNX_COMMAND_INDIVIDUAL_ADDR_REQUEST = 0b0100,
    KNX_COMMAND_INDIVIDUAL_ADDR_RESPONSE = 0b0101,
    KNX_COMMAND_MASK_VERSION_READ = 0b1100,
    KNX_COMMAND_MASK_VERSION_RESPONSE = 0b1101,
    KNX_COMMAND_RESTART = 0b1110,
    KNX_COMMAND_ESCAPE = 0b1111
}KnxCommandType;

// Extended (escaped) KNX commands
typedef enum {
    KNX_EXT_COMMAND_AUTH_REQUEST = 0b010001,
    KNX_EXT_COMMAND_AUTH_RESPONSE = 0b010010
}KnxExtendedCommandType;

// KNX Transport Layer Communication Type
typedef enum {
    KNX_COMM_UDP = 0b00, // Unnumbered Data Packet
    KNX_COMM_NDP = 0b01, // Numbered Data Packet
    KNX_COMM_UCD = 0b10, // Unnumbered Control Data
    KNX_COMM_NCD = 0b11  // Numbered Control Data
}KnxCommunicationType;

// KNX Control Data (for UCD / NCD packets) - Application Layer
typedef enum {
    KNX_CONTROLDATA_CONNECT = 0b00,      // UCD
    KNX_CONTROLDATA_DISCONNECT = 0b01,   // UCD
    KNX_CONTROLDATA_POS_CONFIRM = 0b10,  // NCD
    KNX_CONTROLDATA_NEG_CONFIRM = 0b11   // NCD
}KnxControlDataType;


typedef struct KnxTelegram{
	int buffer[MAX_KNX_TELEGRAM_SIZE];
}KnxTelegram;
	
		void clear(KnxTelegram *tg);
		void setBufferByte(KnxTelegram *tg, int index, int content);
		int getBufferByte(KnxTelegram *tg, int index);
		void setPayloadLength(KnxTelegram *tg, int size);
		int getPayloadLength(KnxTelegram *tg);
		void setRepeated(KnxTelegram *tg, bool repeat);
		bool isRepeated(KnxTelegram *tg);
		void setPriority(KnxTelegram *tg, KnxPriorityType prio);
		KnxPriorityType getPriority(KnxTelegram *tg);
		void setSourceAddress(KnxTelegram *tg, int area, int line, int member);
		int getSourceArea(KnxTelegram *tg);
		int getSourceLine(KnxTelegram *tg);
		int getSourceMember(KnxTelegram *tg);
		void setTargetGroupAddress(KnxTelegram *tg, int main, int middle, int sub);
        void setTargetIndividualAddress(KnxTelegram *tg, int area, int line, int member);
		bool isTargetGroup(KnxTelegram *tg);
		int getTargetMainGroup(KnxTelegram *tg);
		int getTargetMiddleGroup(KnxTelegram *tg);
		int getTargetSubGroup(KnxTelegram *tg);
        int getTargetArea(KnxTelegram *tg);
        int getTargetLine(KnxTelegram *tg);
        int getTargetMember(KnxTelegram *tg);
		void setRoutingCounter(KnxTelegram *tg, int counter);
		int getRoutingCounter(KnxTelegram *tg);
		void setCommand(KnxTelegram *tg, KnxCommandType command);
		KnxCommandType getCommand(KnxTelegram *tg);
		void setFirstDataByte(KnxTelegram *tg, int data);
		int getFirstDataByte(KnxTelegram *tg);
		void set2ByteFloatValue(KnxTelegram *tg, float value);
		float get2ByteFloatValue(KnxTelegram *tg);
		void createChecksum(KnxTelegram *tg);
		bool verifyChecksum(KnxTelegram *tg);
		int getChecksum(KnxTelegram *tg);
		//void print(KnxTelegram *tg, FILE *TPUART_SERIAL_CLASS);
		int getTotalLength(KnxTelegram *tg);
        KnxCommunicationType getCommunicationType(KnxTelegram *tg);
        void setCommunicationType(KnxTelegram *tg, KnxCommunicationType);
        int getSequenceNumber(KnxTelegram *tg);
        void setSequenceNumber(KnxTelegram *tg, int);
        KnxControlDataType getControlData(KnxTelegram *tg);
        void setControlData(KnxTelegram *tg, KnxControlDataType);

#endif
