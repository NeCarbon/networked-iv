//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CHttpClient.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

// OS Dependent Defines
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define closesocket close
#endif

// OS Independent Defines
#define MAX_BUFFER 8192
#define DEFAULT_PORT 80
#define DEFAULT_USER_AGENT "Networked: IV/1.0"
#define DEFAULT_REFERER "http://networked-iv.com"

CHttpClient::CHttpClient()
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Invalidate the socket handle
	m_iSocket = INVALID_SOCKET;

	// Reset class vars
	ResetVars();
}

CHttpClient::~CHttpClient()
{
	// If we are connected to a host disconnect
	if(m_bConnected)
		Disconnect();

	// If windows cleanup winsock
#ifdef WIN32
	WSACleanup();
#endif
}

void CHttpClient::ResetVars()
{
	// Set the timeout amount
	m_lReceiveTimeout = 1;

	// Set the connected flag to false
	m_bConnected = false;

	// Reset the host
	m_strHost.Clear();

	// Reset the port
	m_usPort = DEFAULT_PORT;

	// Set the status to none
	m_status = HTTP_STATUS_NONE;

	// Reset the header
	m_strHeader.Clear();

	// Reset the data
	m_strData.Clear();

	// Set the last error to none
	m_lastError = HTTP_ERROR_NONE;

	// Set the default user agent
	m_strUserAgent = DEFAULT_USER_AGENT;

	// Set the default referer
	m_strReferer = DEFAULT_REFERER;
}

void CHttpClient::SetReceiveTimeout(long lReceiveTimeout)
{
	// Set the receive timeout amount
	m_lReceiveTimeout = lReceiveTimeout;
}

bool CHttpClient::Connect()
{
	// Prepare the socket
	m_iSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(m_iSocket == INVALID_SOCKET)
	{
		// Failed to prepare the socket, set the last error
		m_lastError = HTTP_ERROR_SOCKET_PREPARE_FAILED;
		return false;
	}

	// Get the host
	hostent * heHost = gethostbyname(m_strHost.Get());

	if(heHost == NULL)
	{
		// Failed to get the host, set the last error
		m_lastError = HTTP_ERROR_INVALID_HOST;
		return false;
	}

	// Prepare a socket address
	sockaddr_in sinAddress;
	sinAddress.sin_family = AF_INET;
	sinAddress.sin_port = htons(m_usPort);
	sinAddress.sin_addr = *((in_addr *)heHost->h_addr);
	memset(&sinAddress.sin_zero, 0, (sizeof(CHAR) * 8));

	// Set the socket receive timeout amount
	timeval tvTimeout;
	tvTimeout.tv_sec = m_lReceiveTimeout;
	tvTimeout.tv_usec = 0;
	setsockopt(m_iSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tvTimeout, sizeof(timeval));

	// Try to connect
	if(connect(m_iSocket, (sockaddr *)&sinAddress, sizeof(sockaddr)) < 0)
	{
		// Disconnect
		Disconnect();

		// Connection failed, set the last error
		m_lastError = HTTP_ERROR_CONNECTION_FAILED;
		return false;
	}

	// Set the connected flag to true
	m_bConnected = true;
	return true;
}

void CHttpClient::Disconnect()
{
	// Is the socket valid?
	if(m_iSocket != INVALID_SOCKET)
	{
		// Close the socket
		closesocket(m_iSocket);

		// Invalidate the socket
		m_iSocket = INVALID_SOCKET;
	}

	// Set the connected flag to false
	m_bConnected = false;
}

bool CHttpClient::Write(const char * szData, int iLen)
{
	// Try to send
	if(send(m_iSocket, szData, iLen, 0) == SOCKET_ERROR)
	{
		// Send failed, set the last error
		m_lastError = HTTP_ERROR_SEND_FAILED;
		return false;
	}

	return true;
}

int CHttpClient::Read(char * szBuffer, int iLen)
{
	// Try to receive
	return recv(m_iSocket, szBuffer, iLen, 0);
}

bool CHttpClient::Get(String strPath)
{
	// Connect to the host
	Connect();

	// Reset the header and data
	m_strHeader.Clear();
	m_strData.Clear();

	// Prepare the GET command
	String strGet("GET %s HTTP/1.0\r\n" \
				  "Host: %s\r\n" \
				  "User-Agent: %s\r\n" \
				  "Referer: %s\r\n" \
				  "Connection: close\r\n" \
				  "\r\n", 
				  strPath.Get(), m_strHost.Get(), 
				  m_strUserAgent.Get(), m_strReferer.Get());

	// Send the GET command
	if(!Write(strGet.Get(), strGet.GetLength()))
	{
		// Send failed
		return false;
	}

	// Set the status to get data
	m_status = HTTP_STATUS_GET_DATA;
	return true;
}

bool CHttpClient::Post(bool bHasResponse, String strPath, String strData, String strContentType)
{
	// Connect to the host
	Connect();

	// Reset the header and data
	m_strHeader.Clear();
	m_strData.Clear();

	// Prepare the POST command
	String strGet("POST %s HTTP/1.0\r\n" \
				  "Host: %s\r\n" \
				  "User-Agent: %s\r\n\r\n" \
				  "Referer: %s\r\n" \
				  "Content-Type: %s\r\n" \
				  "Content-Length: %d\r\n" \
				  "Connection: close\r\n" \
				  "\r\n" \
				  "%s", 
				  strPath.Get(), m_strHost.Get(), m_strUserAgent.Get(), 
				  m_strReferer.Get(), strContentType.Get(), strData.GetLength(), 
				  strData.Get());

	// Send the POST command
	if(!Write(strGet.Get(), strGet.GetLength()))
	{
		// Send failed
		return false;
	}

	// Do we have a response
	if(bHasResponse)
	{
		// Set the status to get data
		m_status = HTTP_STATUS_GET_DATA;
	}
	else
	{
		// Disconnect from the host
		Disconnect();
	}

	return true;
	
}

/*void ParseHeader(String strHeader)
{
	printf("Parsing header...\n");
	size_t sCurrent = 0;
	size_t sHttpType = strHeader.Find(' ');

	if(sHttpType == String::nPos)
	{
		printf("Invalid http type...\n");
		return;
	}

	String strHttpType = strHeader.SubStr(sCurrent, sHttpType);
	printf("Http type: (%s)\n", strHttpType.Get());

	sCurrent += (sHttpType + 1);

	size_t sResponseCode = strHeader.Find(' ', sCurrent);

	if(sResponseCode == String::nPos)
	{
		printf("Invalid response code...\n");
		return;
	}

	String strResponseCode = strHeader.SubStr(sCurrent, (sResponseCode - sCurrent));
	printf("Response code: (%s)\n", strResponseCode.Get());
}*/

void CHttpClient::Process()
{
	// Are we not in idle status?
	if(m_status != HTTP_STATUS_NONE)
	{
		switch(m_status)
		{
		case HTTP_STATUS_GET_DATA:
			{
				// Prepare a buffer
				char szBuffer[MAX_BUFFER];
				memset(szBuffer, 0, sizeof(szBuffer));

				// Try to read from the socket
				int iBytesRecieved = Read(szBuffer, sizeof(szBuffer));

				// Did we get anything?
				if(iBytesRecieved > 0)
				{
					// Is the header empty?
					if(m_strHeader.IsEmpty())
					{
						// Make sure we have enough data in the buffer
						if(iBytesRecieved < 4)
						{
							// We don't have a header, set the status
							m_status = HTTP_STATUS_INVALID;

							// Set the last error
							m_lastError = HTTP_ERROR_NO_HEADER;

							// Disconnect from the host
							Disconnect();
							return;
						}

						// Get the header size from the buffer
						int iHeaderSize = -1;

						for(int i = 0; i < (iBytesRecieved - 3); i++)
						{
							if(szBuffer[i] == '\r' && szBuffer[i + 1] == '\n' && 
								szBuffer[i + 2] == '\r' && szBuffer[i + 3] == '\n')
								iHeaderSize = i;
						}

						// Check if we have a valid header size
						if(iHeaderSize == -1)
						{
							// We don't have a header, set the status
							m_status = HTTP_STATUS_INVALID;

							// Set the last error
							m_lastError = HTTP_ERROR_NO_HEADER;

							// Disconnect from the host
							Disconnect();
							return;
						}

						// Set the data
						m_strData.Append(szBuffer + iHeaderSize + 4); // +4 '\r\n\r\n'

						// Null terminate the buffer at the end of the header
						szBuffer[iHeaderSize] = '\0';

						// Set the header
						m_strHeader.Set(szBuffer);

						//ParseHeader(m_strHeader);
					}
					else
					{
						// Append the buffer directly to the data
						m_strData.Append(szBuffer);
					}
				}
				else
				{
					// We got data, set the status
					m_status = HTTP_STATUS_GOT_DATA;

					// Disconnect from the host
					Disconnect();
				}
			}
			break;
		}
	}
}

String CHttpClient::GetLastErrorString()
{
	String strError("Unknown");

	switch(GetLastError())
	{
	case HTTP_ERROR_SOCKET_PREPARE_FAILED:
		strError.Set("Failed to prepare socket");
		break;
	case HTTP_ERROR_INVALID_HOST:
		strError.Set("Invalid host");
		break;
	case HTTP_ERROR_CONNECTION_FAILED:
		strError.Set("Connection failed");
		break;
	case HTTP_ERROR_SEND_FAILED:
		strError.Set("Send failed");
	case HTTP_ERROR_NO_HEADER:
		strError.Set("No header");
	}

	return strError;
}
