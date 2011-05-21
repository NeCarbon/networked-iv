//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetworkPadState.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

CNetworkPadState::CNetworkPadState()
{
	memset(this, 0, sizeof(CNetworkPadState));
}

bool CNetworkPadState::operator== (const CNetworkPadState& o) const
{
	// todo: alternative
	return ((!memcmp(this, &o, sizeof(CNetworkPadState))) != 0);
}

bool CNetworkPadState::operator!= (const CNetworkPadState& o) const
{
	// todo: alternative
	return (memcmp(this, &o, sizeof(CNetworkPadState)) != 0);
}

void CNetworkPadState::Serialize(CBitStream * pBitStream) const
{
	pBitStream->Write((char *)byteLeftAnalogLR, sizeof(byteLeftAnalogLR));
	pBitStream->Write((char *)byteLeftAnalogUD, sizeof(byteLeftAnalogUD));
	pBitStream->WriteBits((unsigned char *)&keys, KEY_COUNT);
}

bool CNetworkPadState::Deserialize(CBitStream * pBitStream)
{
	if(!pBitStream->Read((char *)byteLeftAnalogLR, sizeof(byteLeftAnalogLR)))
		return false;

	if(!pBitStream->Read((char *)byteLeftAnalogUD, sizeof(byteLeftAnalogUD)))
		return false;

	if(!pBitStream->ReadBits((unsigned char *)&keys, KEY_COUNT))
		return false;

	return true;
}
