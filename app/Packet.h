
#ifndef _PACKET_H_
#define _PACKET_H_

extern rt_uint8_t BuildSwitchMainboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result, int port);

extern rt_uint8_t BuildDisableMainboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result);

extern rt_uint8_t BuildSwitchSlaveboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result, int mainPort, int slavePort);

extern rt_uint8_t GetPacketCmd(const rt_uint8_t *buf);

extern rt_bool_t ParseSwitchMainboardPacket(const rt_uint8_t *buf, int *port);

extern rt_bool_t ParseSWitchSlaveboardPacket(const rt_uint8_t *buf, int *mainPort, int *slavePort);

#endif
