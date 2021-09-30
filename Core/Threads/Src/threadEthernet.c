#include "threadEthernet.h"
#include "debug.h"
#include "net_prots.h"

uint8_t CheckPacket(net_packet_t * packet){
  uint8_t csum = 0xff;
  uint8_t * buffer = packet;
  for (uint8_t iter = 0; iter < sizeof(net_packet_t) - 1; iter++){
    csum = csum ^ buffer[iter];
  }
  return (uint8_t)(packet->checksum == csum);
}

net_packet_t PrepareMsg(msg_type_t mt, sensor_type_t st, data_type_t dt){
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

uint8_t UDP_MsgProc(uint8_t * msg, uint16_t msglen){
  print_in("UDP REPLY ACTIVE\r\n");
  
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
      Ethernet_PacketProc(buffer, length/*, &packet, 100*/);
      if (1){//CheckPacket(&packet)){
        // snprintf(str, 64, "MT: %x, ST: %x, DT: %x, CS: %x\r\n", packet.m_type, packet.s_type, packet.d_type, packet.checksum);
        // print_in(str);
        // UDP_PacketSend(ip_set(192, 168, 0, 25), 11111, msg, 8);
        // // print_in("New UDP msg\r\n");
        // packet = PrepareMsg(ANSWER, GYRO_1, 326);
        // UDP_Reply(eth_frame, length, &packet, sizeof(net_packet_t));
        // print_in("after\r\n");
      }
    }
    // UDP_PacketSend(ip_set(192, 168, 0, 25), 5555, msg, 5);
    // print_db("yep\r\n");
    // osDelay(500);
  }
}