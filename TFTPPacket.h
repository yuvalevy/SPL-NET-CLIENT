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