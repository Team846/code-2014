#include "MessageBuffer.h"
#include "Util.h"
#include "MessengerReadOnlyBufferException.h"
#include "MessengerBufferOverflowException.h"
#include "MessengerArgumentOutOfRangeException.h"
#include <bitset>
#include <iostream>

using namespace Rhesus::Messenger;

const int MessageBuffer::kBufferResizeOverAllocateBytes = 4;

MessageBuffer::MessageBuffer()
{
	construct(NULL, 0);
}

MessageBuffer::MessageBuffer(int bufferDefaultSize)
{
	construct(NULL, bufferDefaultSize);
}

MessageBuffer::MessageBuffer(const UINT8* buff, int len)
{
	construct(buff, len);
}

MessageBuffer::~MessageBuffer()
{
	delete[] m_internalBuffer;
	m_internalBuffer = NULL;
}

void MessageBuffer::construct(const UINT8* buff, int size)
{
	m_internalBitPos = 0;
	m_internalBuffer = NULL;

	if(size > 0)
	{
		m_internalBuffer = new UINT8[size];

		if(buff == NULL)
			memset(m_internalBuffer, 0, size * sizeof(UINT8));
		else
			memcpy(m_internalBuffer, buff, size * sizeof(UINT8));
	}

	m_internalBufferSize = size;
	
	m_isReadOnly = buff != NULL;
	m_sent = false;
}

void MessageBuffer::Write(UINT8 c)
{
	InternalWriteByte(c, sizeof(UINT8)*8);
}

void MessageBuffer::Write(UINT8* c, UINT16 len)
{
	InternalWriteInteger(len, sizeof(UINT16)*8);
	InternalWriteBytes(c, len);
}

void MessageBuffer::WriteRaw(UINT8* c, UINT16 len)
{
	InternalWriteBytes(c, len);
}

void MessageBuffer::Write(std::string str)
{
	InternalWriteInteger((ULONG)str.length(), sizeof(UINT16) * 8);
	InternalWriteBytes((UINT8*)(str.c_str()), str.length());
}

void MessageBuffer::Write(bool b)
{
	InternalWriteByte(b ? 1 : 0, 1);
}

void MessageBuffer::Write(double d)
{
	UINT64 packed = Util::PackDouble(d);

	Write(packed);
}

void MessageBuffer::Write(float f)
{
	UINT32 packed = Util::PackFloat(f);

	Write(packed);
}

void MessageBuffer::Write(INT64 l)
{
	InternalWriteInteger((UINT64)l, sizeof(INT64) * 8);
}

void MessageBuffer::Write(UINT64 l)
{
	InternalWriteInteger(l, sizeof(UINT64) * 8);
}

void MessageBuffer::Write(INT32 i)
{
	InternalWriteInteger(i, sizeof(INT32)*8);
}

void MessageBuffer::Write(UINT32 i)
{
	InternalWriteInteger(i, sizeof(UINT32)*8);
}

void MessageBuffer::Write(INT16 s)
{
	InternalWriteInteger(s, sizeof(INT16)*8);
}

void MessageBuffer::WritePadBits()
{	
	if(m_isReadOnly)
	{
		throw MessengerReadOnlyBufferException("Can't write to a read-only buffer!");
		return;
	}
	
	m_internalBitPos += 8 - m_internalBitPos % 8;
}

UINT8 MessageBuffer::ReadChar()
{
	return InternalReadByte(sizeof(UINT8)*8);
}

UINT8* MessageBuffer::ReadBytes()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	return InternalReadBytes(len);
}

std::string MessageBuffer::ReadStdString()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	char* str = (char*)InternalReadBytes(len);
	return std::string(str, str + len);
}

INT64 MessageBuffer::ReadInt64()
{
	return (INT64)InternalReadInteger(sizeof(INT64) * 8);
}

UINT64 MessageBuffer::ReadUInt64()
{
	return InternalReadInteger(sizeof(UINT64) * 8);
}

INT32 MessageBuffer::ReadInt32()
{
	return (INT32)InternalReadInteger(sizeof(INT32) * 8);
}

UINT32 MessageBuffer::ReadUInt32()
{
	return (UINT32)InternalReadInteger(sizeof(UINT32) * 8);
}

INT16 MessageBuffer::ReadInt16()
{
	return (INT16)InternalReadInteger(sizeof(INT16) * 8);
}

double MessageBuffer::ReadDouble()
{
	UINT64 packed = ReadUInt64();

	return Util::UnpackDouble(packed);
}

float MessageBuffer::ReadFloat()
{
	UINT32 packed = ReadUInt32();
	
	return Util::UnpackFloat(packed);
}

bool MessageBuffer::ReadBool()
{
	return InternalReadByte(1) == 1;
}

void MessageBuffer::SkipPadBits()
{
	m_internalBitPos += 8 - m_internalBitPos % 8;
}

bool MessageBuffer::AssertBufferHasSpace(UINT32 bits)
{
	return ((bits + 7) >> 3) <= m_internalBufferSize;
}

void MessageBuffer::InternalWriteByte(const UINT8 data, int bit_length)
{
	if(m_isReadOnly)
	{
		throw MessengerReadOnlyBufferException("Can't write to a read-only buffer!");
		return;
	}
	
	if(bit_length < 1 || bit_length > 8)
	{
		throw MessengerArgumentOutOfRangeException("Can't write less than one bit or more than eight bits!");
		return;
	}
	
	FitBufferToSize(m_internalBitPos + bit_length);
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	// this operation performs a logical AND on the given data and the bit_length
	// in order to get rid of the unnecessary bits.
	// The data is AND'ed with 0xFF (1111 1111) shifted to the right by 8 - bit_length, creating the masker.
	UINT8 data_masked = (UINT8)(((UINT32)data & ((UINT32)(~(UINT32)(0)) >> (8 - bit_length))) << (8 - bit_length));
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;

	m_internalBuffer[GetBytePos()] |= (UINT8)(data_masked >> ((8 - bit_length) + overflow));

	// this byte is finished
	if(overflow <= 0)
	{
	}
	// write into the next byte
	else if(overflow > 0)
	{
		remainingBits = overflow;
		
		// mask off written bits
		data_masked &= ((UINT8)(~(UINT32)(0)) >> (8 - remainingBits));
		
		m_internalBuffer[GetBytePos() + 1] |= (data_masked << (8 - remainingBits));
	}

	m_internalBitPos += bit_length;
}

void MessageBuffer::InternalWriteBytes(const UINT8 data[], int bytes)
{
	if(m_isReadOnly)
	{
		throw MessengerReadOnlyBufferException("Can't write to a read-only buffer!");
		return;
	}
	
	for(int i = 0; i < bytes; i++)
	{
		InternalWriteByte(data[i], 8);
	}
}

void MessageBuffer::InternalWriteInteger(const UINT64 data, int bits)
{	
	if(m_isReadOnly)
	{
		throw MessengerReadOnlyBufferException("Can't write to a read-only buffer!");
		return;
	}
	
	for(int i = 0; i < bits; i += 8)
	{
		if(i + 8 > bits)
		{
			// this is the last bitset
			int rem_bits = bits - i;
			InternalWriteByte((UINT8)(data >> i), rem_bits);
		}
		else
		{
			InternalWriteByte((UINT8)(data >> i), 8);
		}
	}
}

UINT8 MessageBuffer::InternalReadByte(int bit_length)
{
	if(bit_length < 1 || bit_length > 8)
	{
		throw MessengerArgumentOutOfRangeException("Can't write less than one bit or more than eight bits!");
		return 0;
	}
	
	if(!AssertBufferHasSpace(m_internalBitPos + bit_length))
	{
		throw MessengerBufferOverflowException("Can't read past the buffer!");
		return 0;
	}
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;
	
	int pos = GetBytePos();
	
	UINT32 masker = 0;

	if(overflow < 0)
	{
		masker = ((UINT32)0xff) << (-overflow);
	}
	else
	{
		masker = ((UINT32)0xff) >> (overflow);
	}

	UINT32 retrieved = (UINT32)(((UINT32)m_internalBuffer[pos]) & masker);
	
	if(overflow <= 0)
	{
		// we're done.
	}
	else if(overflow > 0)
	{
		retrieved = retrieved << (overflow);
		retrieved = (UINT8)((UINT32)(retrieved) | ((m_internalBuffer[GetBytePos() + 1] & (~(UINT32)(0) << (8 - overflow))) >> (8 - overflow))); 
	}
	
	m_internalBitPos += bit_length;
	return (UINT8)(retrieved >> (8 - bit_length));
}

UINT8* MessageBuffer::InternalReadBytes(int bytes)
{	
	// uses a the heap so that the array stays alive after the function returns
	UINT8* retrieved = new UINT8[bytes];
	
	for(int i = 0; i < bytes; i++)
	{
		retrieved[i] = InternalReadByte(8);
	}
	
	return retrieved;
}

UINT64 MessageBuffer::InternalReadInteger(int bits)
{	
	UINT64 retrieved = 0;
	
	for(int i = 0; i < bits; i += 8)
	{
		if(i + 8 > bits)
		{
			// this is the last bitset
			int rem_bits = bits - i;
			retrieved |= (UINT64)InternalReadByte(rem_bits) << i;
		}
		else
		{
			retrieved |= (UINT64)InternalReadByte(8) << i;
		}
	}
	
	return retrieved;
}

int MessageBuffer::GetBufferLength()
{
	return ((m_internalBitPos + 7) / 8);
}

int MessageBuffer::GetBytePos()
{
	return (int)(m_internalBitPos / 8);
}

int MessageBuffer::GetBitIndexInCurrentByte()
{
	return m_internalBitPos % 8;
}

void MessageBuffer::FitBufferToSize(UINT32 bits)
{
	UINT32 bytes = (bits + 7) >> 3;
	
	if(m_internalBuffer == NULL)
	{
		int len = bytes + MessageBuffer::kBufferResizeOverAllocateBytes;
		m_internalBuffer = new UINT8[len];
		m_internalBufferSize = len;
		memset(m_internalBuffer, 0, sizeof(UINT8) * len);
	}
	else if(bytes > m_internalBufferSize)
	{
		int len = bytes + MessageBuffer::kBufferResizeOverAllocateBytes;
		UINT8* newBuff = new UINT8[len];
		
		memset(newBuff, 0, sizeof(UINT8) * len);

		memcpy(newBuff, m_internalBuffer, m_internalBufferSize);
		m_internalBufferSize = len;
		
		delete[] m_internalBuffer;
		
		m_internalBuffer = newBuff;
	}
}

const UINT8* MessageBuffer::GetBuffer()
{
	return m_internalBuffer;
}
