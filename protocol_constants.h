#ifndef PROTOCOL_CONSTANTS_H
#define PROTOCOL_CONSTANTS_H

// Delimitatori
constexpr char PROTO_START = '\x02';
constexpr char PROTO_END   = '\x03';

// ID del Raspberry
constexpr char PROTO_ID_1 = 'A';
constexpr char PROTO_ID_2 = '7';
constexpr char PROTO_ID[] = "A7"; // per append()

// Tipi di messaggio
constexpr char MSG_TYPE_HEARTBEAT = 'H';
constexpr char MSG_TYPE_ACK       = 'A';
constexpr char MSG_TYPE_NACK      = 'N';
constexpr char MSG_TYPE_VALUE     = 'V';

constexpr char ESP_ID_1 = 'A';
constexpr char ESP_ID_2 = '6';
constexpr char ESP_ID[] = "A6";




#endif // PROTOCOL_CONSTANTS_H
