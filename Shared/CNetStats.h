//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CNetStats.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

typedef unsigned long long int NetStat_t;

enum NetStatMetrics
{
	// How many bytes per pushed via a call to RakPeerInterface::Send()
	USER_MESSAGE_BYTES_PUSHED,

	// How many user message bytes were sent via a call to RakPeerInterface::Send(). This is less than or equal to USER_MESSAGE_BYTES_PUSHED.
	// A message would be pushed, but not yet sent, due to congestion control
	USER_MESSAGE_BYTES_SENT,

	// How many user message bytes were resent. A message is resent if it is marked as reliable, and either the message didn't arrive or the message ack didn't arrive.
	USER_MESSAGE_BYTES_RESENT,

	// How many user message bytes were received, and processed successfully.
	USER_MESSAGE_BYTES_RECEIVED_PROCESSED,

	// How many user message bytes were received, but ignored due to data format errors. This will usually be 0.
	USER_MESSAGE_BYTES_RECEIVED_IGNORED,

	// How many actual bytes were sent, including per-message and per-datagram overhead, and reliable message acks
	ACTUAL_BYTES_SENT,

	// How many actual bytes were received, including overead and acks.
	ACTUAL_BYTES_RECEIVED,

	NET_STAT_METRICS_COUNT
};

class CNetStats
{
public:
	// For each type in NetStatMetrics, what is the value over the last 1 second?
	NetStat_t ulValueOverLastSecond[NET_STAT_METRICS_COUNT];

	/// For each type in NetStatMetrics, what is the total value over the lifetime of the connection?
	NetStat_t uiRunningTotal[NET_STAT_METRICS_COUNT];

	// When did the connection start?
	NetStat_t ulConnectionStartTime;

	// Is our current send rate throttled by congestion control?
	// This value should be true if you send more data per second than your bandwidth capacity
	bool bIsLimitedByCongestionControl;

	/// If bIsLimitedByCongestionControl is true, what is the limit, in bytes per second?
	NetStat_t ulBPSLimitByCongestionControl;

	/// Is our current send rate throttled by a call to RakPeer::SetPerConnectionOutgoingBandwidthLimit()?
	bool bIsLimitedByOutgoingBandwidthLimit;

	// If bIsLimitedByOutgoingBandwidthLimit is true, what is the limit, in bytes per second?
	NetStat_t ulBPSLimitByOutgoingBandwidthLimit;

	// For each priority level, how many messages are waiting to be sent out?
	unsigned int uiMessageInSendBuffer[PRIORITY_COUNT];

	// For each priority level, how many bytes are waiting to be sent out?
	double dBytesInSendBuffer[PRIORITY_COUNT];

	// How many messages are waiting in the resend buffer? This includes messages waiting for an ack, so should normally be a small value
	// If the value is rising over time, you are exceeding the bandwidth capacity. See BPSLimitByCongestionControl 
	unsigned int uiMessagesInResendBuffer;

	// How many bytes are waiting in the resend buffer. See also uiMessagesInResendBuffer
	NetStat_t ulBytesInResendBuffer;

	// Over the last second, what was our packetloss? This number will range from 0.0 (for none) to 1.0 (for 100%)
	float fPacketlossLastSecond;

	// What is the average total packetloss over the lifetime of the connection?
	float fPacketlossTotal;
};
