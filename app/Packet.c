
#include "rtthread.h"

#include "Protocol.h"
#include "Packet.h"

#define PACKET_SIZE                8

rt_uint8_t BuildSwitchMainboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result, int port)
{
	if (maxSize < PACKET_SIZE)
	{
		return 0;
	}

	rt_memset(buf, 0x00, PACKET_SIZE);
	
	rt_uint8_t index = 0;
	buf[index++] = CMD_SWITCH_MAINBOARD;
	buf[index++] = port & 0xff;
	buf[index++] = result;
	
	return PACKET_SIZE;
}

rt_uint8_t BuildDisableMainboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result)
{
	if (maxSize < PACKET_SIZE)
	{
		return 0;
	}

	rt_memset(buf, 0x00, PACKET_SIZE);
	
	rt_uint8_t index = 0;
	buf[index++] = CMD_DISABLE_MAINBOARD;
	buf[index++] = result;
	
	return PACKET_SIZE;
}

rt_uint8_t BuildSwitchSlaveboardRetPacket(rt_uint8_t *buf, rt_uint8_t maxSize, rt_uint8_t result, int mainPort, int slavePort)
{
	if (maxSize < PACKET_SIZE)
	{
		return 0;
	}

	rt_memset(buf, 0x00, PACKET_SIZE);
	
	rt_uint8_t index = 0;
	buf[index++] = CMD_SWITCH_SLAVEBOARD;
	buf[index++] = mainPort & 0xff;
	buf[index++] = slavePort & 0xff;
	buf[index++] = result;
	
	return PACKET_SIZE;
}

rt_uint8_t GetPacketCmd(const rt_uint8_t *buf)
{
	rt_uint8_t type = buf[0];

	return type;
}

rt_bool_t ParseSwitchMainboardPacket(const rt_uint8_t *buf, int *port)
{
	if (buf[0] != CMD_SWITCH_MAINBOARD)
	{
		return RT_FALSE;
	}

	if (port != RT_NULL)
	{
		*port = buf[1];
	}

	return RT_TRUE;
}

rt_bool_t ParseSWitchSlaveboardPacket(const rt_uint8_t *buf, int *mainPort, int *slavePort)
{
	if (buf[0] != CMD_SWITCH_SLAVEBOARD)
	{
		return RT_FALSE;
	}

	if (mainPort != RT_NULL)
	{
		*mainPort = buf[1];
	}

	if (slavePort != RT_NULL)
	{
		*slavePort = buf[2];
	}
	
	return RT_TRUE;
}

