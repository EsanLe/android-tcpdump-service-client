#include <stdbool.h>
bool getDataPacketFromIP (char* ip, char* packet);
bool getDataPacketToIP (char* ip, char* packet);
bool getDataPacketFromIPAndPort  (char* ip, char* port, char* packet);
bool getDataPacketFromPort (char* port, char* packet);
bool getDataPacketToPort (char* port, char* packet);
bool getDataPacketFromIPToIP (char* ip, char* ipto, char* packet);
bool getDataPacketFromMac (char* mac, char* packet);
bool getDataPacketToMac (char* mac, char* packet);
bool getDataPacketFromMacToMac (char* srcmac, char* desmac, char* packet);
bool getDataPacketWithProtocol (char* type, char* packet);
