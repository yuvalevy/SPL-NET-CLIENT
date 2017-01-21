#ifndef TFTPPACKET_H_
#define TFTPPACKET_H_
#pragma once

	class TFTPPacket
	{
	private:
		short opCode;
	protected:
		TFTPPacket(short opcode);
	public:
		 short getOpcode();
	};
#endif