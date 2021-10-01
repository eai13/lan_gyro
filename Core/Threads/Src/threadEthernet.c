#include "threadEthernet.h"

static inline uint8_t CheckPacket(net_packet_t * packet){
  uint8_t csum = 0xff;
  uint8_t * buffer = packet;
  for (uint8_t iter = 0; iter < sizeof(net_packet_t) - 1; iter++){
    csum = csum ^ buffer[iter];
  }
  return (uint8_t)(packet->checksum == csum);
}

static inline net_packet_t PrepareMsg(msg_type_t mt, sensor_type_t st, data_type_t dt){
  net_packet_t packet = {
    .s_type = st,
    .m_type = mt,
    .d_type = dt,
    .checksum = 0xff
  };
  uint8_t * buffer = &packet;
  for (uint8_t iter = 0; iter < sizeof(net_packet_t) - 1; iter++){
    packet.checksum = packet.checksum ^ buffer[iter]; 
  }
  return packet;
}

static inline uint8_t CheckSum(net_packet_t * packet){
  uint8_t csum = 0xff;
  uint8_t * buffer = (uint8_t *)packet;
  for (uint8_t iter = 0; iter < sizeof(net_packet_t) - 1; iter++){
    csum = csum ^ buffer[iter];
  }
  return csum;
}

uint8_t UDP_MsgProc(uint8_t * msg, uint16_t msglen){
  net_packet_t * packet = (net_packet_t *)msg;
  if (__ASSERT_PACK(packet) && CheckPacket(packet)){
    *packet = PrepareMsg(MT_ANSWER, packet->s_type, GetGyroData(packet->s_type, packet->d_type));
  }
  else{
    *packet = PrepareMsg(MT_ANSWER, ST_ERROR, DT_ERROR);
  }
  return UDP_REPLY;
}

void threadEthernet(void const * arg){
  uint16_t length;
  uint8_t buffer[NET_BUFFER_SIZE];
  ethernet_packet_t * eth_frame = buffer;
  net_packet_t packet;
  uint8_t str[64];
  uint8_t msg[] = "hello\r\n";
  print_in("Ethernet Thread Started\r\n");
  while(1){
    memset(&packet, 0x00, sizeof(net_packet_t));
    if (length = Eth_ReceivePacket(buffer, sizeof(buffer))){
      Ethernet_PacketProc(buffer, length);
    }
  }
}