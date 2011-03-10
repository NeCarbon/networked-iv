//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetBitStreamInterface.h
// Project: Net
// Author(s): jenksta
// License: See LICENSE in project directory
//
//==========================================================================

#pragma once

#include <StdInc.h>
#include <CString.h>
#include <CMath.h>

class CNetworkPadState;

class CNetBitStreamInterface
{
public:
	// Reset the BitStream for reuse.
	virtual void            Reset(void) = 0;

	// Reset the BitStream read pointer for reuse.
	virtual void            ResetReadPointer(void) = 0;

	// Reset the BitStream write pointer for reuse.
	virtual void            ResetWritePointer(void) = 0;

	// Write any integral type to the BitStream.
	virtual void            Write(const bool &inVar) = 0;
	virtual void            Write(const char &inVar) = 0;
	virtual void            Write(const unsigned char &inVar) = 0;
	virtual void            Write(const short &inVar) = 0;
	virtual void            Write(const unsigned short &inVar) = 0;
	virtual void            Write(const int &inVar) = 0;
	virtual void            Write(const unsigned int &inVar) = 0;
	virtual void            Write(const long &inVar) = 0;
	virtual void            Write(const unsigned long &inVar) = 0;
	virtual void            Write(const float &inVar) = 0;
	virtual void            Write(const double &inVar) = 0;
	virtual void            Write(const String &inString) = 0;
	virtual void            Write(const Vector3 &inVar) = 0;
	virtual void            Write(const CNetworkPadState &inVar) = 0;

	// Write any integral type compressed to the BitStream.
	virtual void            WriteCompressed(const bool &inVar) = 0;
	virtual void            WriteCompressed(const char &inVar) = 0;
	virtual void            WriteCompressed(const unsigned char &inVar) = 0;
	virtual void            WriteCompressed(const short &inVar) = 0;
	virtual void            WriteCompressed(const unsigned short &inVar) = 0;
	virtual void            WriteCompressed(const int &inVar) = 0;
	virtual void            WriteCompressed(const unsigned int &inVar) = 0;
	virtual void            WriteCompressed(const long &inVar) = 0;
	virtual void            WriteCompressed(const unsigned long &inVar) = 0;
	virtual void            WriteCompressed(const float &inVar) = 0;
	virtual void            WriteCompressed(const double &inVar) = 0;

	// Read any integral type from the BitStream.
	virtual bool            Read(bool &outVar) = 0;
	virtual bool            Read(char &outVar) = 0;
	virtual bool            Read(unsigned char &outVar) = 0;
	virtual bool            Read(short &outVar) = 0;
	virtual bool            Read(unsigned short &outVar) = 0;
	virtual bool            Read(int &outVar) = 0;
	virtual bool            Read(unsigned int &outVar) = 0;
	virtual bool            Read(long &outVar) = 0;
	virtual bool            Read(unsigned long &outVar) = 0;
	virtual bool            Read(float &outVar) = 0;
	virtual bool            Read(double &outVar) = 0;
	virtual bool            Read(String &outString) = 0;
	virtual bool            Read(Vector3 &outVar) = 0;
	virtual bool            Read(CNetworkPadState &outVar) = 0;

	// Read any compressed integral type from the BitStream.
	virtual bool            ReadCompressed(bool &outVar) = 0;
	virtual bool            ReadCompressed(char &outVar) = 0;
	virtual bool            ReadCompressed(unsigned char &outVar) = 0;
	virtual bool            ReadCompressed(short &outVar) = 0;
	virtual bool            ReadCompressed(unsigned short &outVar) = 0;
	virtual bool            ReadCompressed(int &outVar) = 0;
	virtual bool            ReadCompressed(unsigned int &outVar) = 0;
	virtual bool            ReadCompressed(long &outVar) = 0;
	virtual bool            ReadCompressed(unsigned long &outVar) = 0;
	virtual bool            ReadCompressed(float &outVar) = 0;
	virtual bool            ReadCompressed(double &outVar) = 0;

	// Write an array or casted stream or raw data to the BitStream.
	virtual void            Write(const char * inputByteArray, const unsigned int numberOfBytes) = 0;

	// Read an array or casted stream or raw data from the BitStream.
	virtual bool            Read(char * outputByteArray, const unsigned int numberOfBytes) = 0;

	// TODO: Write/Read compressed array

	// Write a sequence of bits to the BitStream.
	virtual void            WriteBits(const unsigned char * inByteArray, unsigned int numberOfBitsToWrite) = 0;

	// Read a sequence of bits from the BitStream.
	virtual bool            ReadBits(unsigned char * inOutByteArray, unsigned int numberOfBitsToRead) = 0;

	// Write a 0 or 1 to the BitStream.
	virtual void            WriteBit(bool bState) = 0;

	// Write a 0 to the BitStream.
	virtual void            Write0(void) = 0;

	// Write a 1 to the BitStream.
	virtual void            Write1(void) = 0;

	// Returns true if the next data read is a 1, false if it is a 0.
	virtual bool            ReadBit(void) = 0;

	// Returns a pointer to the data in the BitStream.
	virtual unsigned char * GetData() = 0;

	// Returns the number of bits used in the BitStream.
	virtual unsigned int    GetNumberOfBitsUsed() = 0;

	// Returns the number of bytes used in the BitStream.
	virtual unsigned int    GetNumberOfBytesUsed() = 0;

	// Returns the number of unread bits left in the BitStream.
	virtual unsigned int    GetNumberOfUnreadBits() = 0;

	// Ignore data we don't intend to read.
	virtual void            IgnoreBits(const unsigned int numberOfBits) = 0;

	// Ignore data we don't intend to read.
	virtual void            IgnoreBytes(const unsigned int numberOfBytes) = 0;

	// Write zeros until the bitstream is filled up to bytes
	virtual void            PadWithZeroToByteLength(unsigned int bytes) = 0;
};
