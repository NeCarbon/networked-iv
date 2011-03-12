//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CHttpClient.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>

// OS Independent Defines
#define DEFAULT_CONTENT_TYPE "text/plain"
//#define DEFAULT_CONTENT_TYPE "application/x-www-form-urlencoded"

#ifndef WIN32
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

// Status codes
enum eHttpStatus
{
	HTTP_STATUS_NONE,
	HTTP_STATUS_INVALID,
	HTTP_STATUS_GET_DATA,
	HTTP_STATUS_GOT_DATA
};

// Error codes
enum eHttpError
{
	HTTP_ERROR_NONE,
	HTTP_ERROR_SOCKET_PREPARE_FAILED,
	HTTP_ERROR_INVALID_HOST,
	HTTP_ERROR_CONNECTION_FAILED,
	HTTP_ERROR_SEND_FAILED,
	HTTP_ERROR_NO_HEADER
};

class CHttpClient
{
private:
	int            m_iSocket;
	long           m_lReceiveTimeout;
	bool           m_bConnected;
	String         m_strHost;
	unsigned short m_usPort;
	eHttpStatus    m_status;
	String         m_strHeader;
	String         m_strData;
	eHttpError     m_lastError;
	String         m_strUserAgent;
	String         m_strReferer;

	void                   ResetVars();
	bool                   Connect();
	void                   Disconnect();

public:
	CHttpClient();
	~CHttpClient();

	virtual void           SetReceiveTimeout(long lReceiveTimeout);
	virtual long           GetReceiveTimeout() { return m_lReceiveTimeout; }
	virtual bool           IsConnected() { return m_bConnected; }
	virtual bool           IsInvalid() { return (m_status == HTTP_STATUS_INVALID); }
	virtual bool           GettingData() { return (m_status == HTTP_STATUS_GET_DATA); }
	virtual bool           GotData() { return (m_status == HTTP_STATUS_GOT_DATA); }
	virtual bool           IsBusy() { return (m_status == HTTP_STATUS_GET_DATA); }
	virtual String       * GetHeader() { return &m_strHeader; }
	virtual String       * GetData() { return &m_strData; }
	virtual eHttpError     GetLastError() { return m_lastError; }
	virtual void           SetUserAgent(String strUserAgent) { m_strUserAgent = strUserAgent; }
	virtual String         GetUserAgent() { return m_strUserAgent; }
	virtual void           SetReferer(String strReferer) { m_strReferer = strReferer; }
	virtual String         GetReferer() { return m_strReferer; }
	virtual void           SetHost(String strHost) { m_strHost = strHost; }
	virtual String         GetHost() { return m_strHost; }
	virtual void           SetPort(unsigned short usPort) { m_usPort = usPort; }
	virtual unsigned short GetPort() { return m_usPort; }
	virtual bool           Write(const char * szData, int iLen);
	virtual int            Read(char * szBuffer, int iLen);
	virtual bool           Get(String strPath);
	virtual bool           Post(bool bHasResponse, String strPath, String strData = "", String strContentType = DEFAULT_CONTENT_TYPE);
	virtual void           Process();
	virtual String         GetLastErrorString();
};
