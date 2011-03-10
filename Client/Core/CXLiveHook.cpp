//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CXLiveHook.cpp
// Project: Client
// Author(s): jenksta
// License: See LICENSE in root directory
//          Contains public domain code from xliveless by listener
//
//==========================================================================

#include <StdInc.h>

#define FAKE_MAGIC 0xDEADDEAD

// XLive Functions
int WINAPI XWSAStartup(WORD wVersionRequested, LPWSADATA lpWsaData)
{
	lpWsaData->wVersion = 2;
	return 0;
}

void WINAPI XWSACleanup()
{
	return;
}

SOCKET WINAPI XSocketCreate(int af, int type, int protocol)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketClose(SOCKET s)
{
	return 0;
}

int WINAPI XSocketShutdown(SOCKET s, int how)
{
	return 0;
}

int WINAPI XSocketIOCTLSocket(SOCKET s, long cmd, long * argp)
{
	return 0;
}

int WINAPI XSocketSetSockOpt(SOCKET s, int level, int optname, const char * optval, int optlen)
{
	return 0;
}

int WINAPI XSocketGetSockName(SOCKET s, sockaddr_in * name, int * namelen)
{
	if(namelen && name && *namelen == sizeof(sockaddr_in)) 
		memset(name, 0, sizeof(sockaddr_in));

	return 0;
}

SOCKET WINAPI XSocketBind(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketConnect(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return 0;
}

int WINAPI XSocketListen(SOCKET s, int backlog)
{
	return 0;
}

SOCKET WINAPI XSocketAccept(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketSelect(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, const struct timeval * timeout)
{
	return 0;
}

int WINAPI XSocketRecv(SOCKET s, char * buf, int len, int flags)
{
	return 0;
}

int WINAPI XSocketRecvFrom(SOCKET s, char * buf, int len, int flags, sockaddr_in * from, int fromlen)
{
	return 0;
}

int WINAPI XSocketSend(SOCKET s, char * buf, int len, int flags)
{
	return 0;
}

int WINAPI XSocketSendTo(SOCKET s, char * buf, int len, int flags, sockaddr_in * to, int tolen)
{
	return 0;
}

int WINAPI XSocketInet_Addr(const char * cp)
{
	return 0;
}

int WINAPI XWSAGetLastError()
{
	return WSAENETDOWN; // 0 ?
}

WORD WINAPI XSocketNTOHS(u_short netshort)
{
	return((netshort & 0xFF00) >> 8) | ((netshort & 0xFF) << 8);
}

DWORD WINAPI XSocketNTOHL(u_long netlong)
{
	return((netlong & 0xFF000000) >> 24) | ((netlong & 0x00FF0000) >> 8) | ((netlong & 0x0000FF00) << 8) | ((netlong & 0x000000FF) << 24);
}

int WINAPI XNetStartup(void *)
{
	return 0;
}

int WINAPI XNetCleanup()
{
	return 0;
}

int WINAPI XNetXnAddrToInAddr(DWORD, DWORD, DWORD * p)
{
	*p = 0;
	return 0;
}

DWORD WINAPI XNetServerToInAddr(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XNetUnregisterInAddr(DWORD)
{
	return 0;
}

int WINAPI XNetGetConnectStatus(DWORD)
{
	return 0;	
}

DWORD WINAPI XNetQosListen(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XNetQosLookup(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XNetQosRelease(DWORD)
{
	return 0; 
}

DWORD WINAPI XNetGetTitleXnAddr(DWORD * pAddr)
{
	*pAddr = 0x0100007F; // 127.0.0.1
	return 4; 
}

DWORD WINAPI XNetGetEthernetLinkStatus()
{
	return 1; 
}

DWORD WINAPI XNetSetSystemLinkPort(DWORD)
{
	return 0; 
}

int WINAPI XNotifyGetNext(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XNotifyPositionUI(DWORD dwPosition)
{
	return 0;
}

DWORD WINAPI XGetOverlappedExtendedError(void *)
{
	return 0;
}

DWORD WINAPI XGetOverlappedResult(void *, DWORD * pResult, DWORD)
{
	if(pResult)
		*pResult = 0;	// 0 elements enumerated

	return 0;
}

int WINAPI XLiveInitialize(DWORD)
{
	return 0;
}

int WINAPI XLiveInput(DWORD * p)
{
	p[5] = 0;
	return 1;	// -1 ?
}

int WINAPI XLiveRender()
{
	return 0;
}

int WINAPI XLiveUninitialize()
{
	return 0;
}

int WINAPI XLiveOnCreateDevice(DWORD, DWORD)
{
	return 0;
}

int WINAPI XLiveOnResetDevice(DWORD)
{
	return 0;
}

int WINAPI XHVCreateEngine(DWORD, DWORD, DWORD)
{
	// Disable live voice
	return -1;
}

DWORD WINAPI XLivePBufferAllocate(int size, FakeProtectedBuffer ** pBuffer)
{
	*pBuffer = (FakeProtectedBuffer *)malloc(size + 16);

	if(!*pBuffer) 
		return E_OUTOFMEMORY;

	// Some arbitrary number
	(*pBuffer)->dwMagick = FAKE_MAGIC;
	(*pBuffer)->nSize = size;
	return 0;
}

DWORD WINAPI XLivePBufferFree(FakeProtectedBuffer * pBuffer)
{
	if(pBuffer && pBuffer->dwMagick == FAKE_MAGIC)
		free(pBuffer);

	return 0;
}

DWORD WINAPI XLivePBufferGetByte(FakeProtectedBuffer * pBuffer, int offset, BYTE * value)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || !value || offset < 0 || offset > pBuffer->nSize)
		return 0;

	*value = pBuffer->bData[offset];
	return 0;
}

DWORD WINAPI XLivePBufferSetByte(FakeProtectedBuffer * pBuffer, int offset, BYTE value)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || offset < 0 || offset > pBuffer->nSize)
		return 0;

	pBuffer->bData[offset] = value;
	return 0;
}

DWORD WINAPI XLivePBufferGetDWORD(FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD * pdwValue)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || dwOffset < 0 || dwOffset > (DWORD)(pBuffer->nSize - 4) || !pdwValue)
		return 0;

	*pdwValue = *(DWORD *)(pBuffer->bData + dwOffset);
	return 0;
}

DWORD WINAPI XLivePBufferSetDWORD(FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD dwValue)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || dwOffset < 0 || dwOffset > (DWORD)(pBuffer->nSize - 4))
		return 0;

	*(DWORD *)(pBuffer->bData + dwOffset) = dwValue;
	return 0;
}

int WINAPI XLiveGetUpdateInformation(DWORD)
{
	// No update
	return -1;
}

int WINAPI XLiveUpdateSystem(DWORD)
{
	// No update
	return -1;
}

int WINAPI XLivePreTranslateMessage(DWORD)
{
	return 0;
}

DWORD WINAPI XLiveProtectData(BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h)
{
	*pDataSize = dwInDataSize;

	if(*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy(pOutBuffer, pInBuffer, dwInDataSize);

	return 0;
}

DWORD WINAPI XLiveUnprotectData(BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h)
{
	*pDataSize = dwInDataSize;

	if(*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy(pOutBuffer, pInBuffer, dwInDataSize);

	return 0;
}

DWORD WINAPI XLiveCreateProtectedDataContext(DWORD * dwType, HANDLE * pHandle)
{
	if(pHandle)
		*pHandle =(HANDLE)1;

	return 0;
}

DWORD WINAPI XLiveQueryProtectedDataInformation(HANDLE h, DWORD * p)
{
	return 0;
}

DWORD WINAPI XLiveCloseProtectedDataContext(HANDLE h)
{
	return 0;
}

int WINAPI XShowPlayerReviewUI(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XShowGuideUI(DWORD)
{
	return 1;
}

int WINAPI XCloseHandle(DWORD)
{
	return 0;
}

int WINAPI XShowGamerCardUI(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XCancelOverlapped(DWORD)
{
	return 0;
}

int WINAPI XEnumerate(HANDLE hEnum, void * pvBuffer, DWORD cbBuffer, DWORD *, void * pOverlapped)
{
	// Some error?
	return 0;
}

int WINAPI XShowSigninUI(DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserGetXUID(DWORD, DWORD * pXuid)
{
	pXuid[0] = pXuid[1] = 0x10001000; 

	// ???
	return 0;
}

int WINAPI XUserGetSigninState(DWORD)
{
	// eXUserSigninState_SignedInLocally
	return 1;
}

int WINAPI XUserGetName(DWORD dwUserId, char * pBuffer, DWORD dwBufLen)
{
	if(dwBufLen < 6)
		return 1;

	strcpy(pBuffer, "IV:MP");
	return 0;
}

int WINAPI XUserAreUsersFriends(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserCheckPrivilege(DWORD user, DWORD priv, PBOOL b)
{
	*b = false;
	return 0;
}

int WINAPI XUserGetSigninInfo(DWORD dwUser, DWORD type, DWORD * pInfo)
{
	// Some arbitrary id for offline user, INVALID_XUID for online user
	pInfo[0] = pInfo[1] = (type == 2) ? ((dwUser + 1) * 0x10001000) : 0;
	return 0;
}

HANDLE WINAPI XNotifyCreateListener(DWORD l, DWORD h)
{
	// Any non-zero value (Zero treated as fatal error).
	return(HANDLE)1;
}

int WINAPI XUserSetProperty(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserSetContext(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XUserWriteAchievements(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XUserCreateAchievementEnumerator(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XUserReadStats(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD * pcbResults, DWORD * pResults, void *)
{
	*pcbResults = 4;
	*pResults = 0;
	return 0;
}

DWORD WINAPI XUserCreateStatsEnumeratorByRank(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XUserCreateStatsEnumeratorByXuid(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

int WINAPI XUserSetContextEx(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XLivePBufferGetByteArray(FakeProtectedBuffer * pBuffer, int offset, BYTE * destination, int size)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || !destination || offset < 0 || offset+size > pBuffer->nSize)
		return 0;

	memcpy(destination, pBuffer->bData+offset, size);
	return 0;
}

DWORD WINAPI XLivePBufferSetByteArray(FakeProtectedBuffer * pBuffer, int offset, BYTE * source, int size)
{
	if(!pBuffer || pBuffer->dwMagick != FAKE_MAGIC || !source || offset < 0 || offset+size > pBuffer->nSize)
		return 0;

	memcpy(pBuffer->bData+offset, source, size);
	return 0;
}

int WINAPI XLiveInitializeEx(void *, int)
{
	return 0;
}

DWORD WINAPI XSessionCreate(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return -1;
}

DWORD WINAPI XStringVerify(DWORD, DWORD, DWORD, DWORD, DWORD, WORD * pResult, DWORD)
{
	*pResult = 0;
	return 0;
}

DWORD WINAPI XStorageUploadFromMemory(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XOnlineStartup()
{
	return 0; 
}

int WINAPI XOnlineCleanup()
{
	return 0;
}

DWORD WINAPI XFriendsCreateEnumerator(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserMuteListQuery(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XInviteGetAcceptedInfo(DWORD, DWORD)
{
	return 1; 
}

int WINAPI XInviteSend(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionWriteStats(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XSessionStart(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionSearchEx(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionModify(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionMigrateHost(DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XOnlineGetNatType()
{
	return 0; 
}

DWORD WINAPI XSessionLeaveLocal(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionJoinRemote(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionJoinLocal(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionGetDetails(DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XSessionFlushStats(DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionDelete(DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XUserReadProfileSettings(DWORD dwTitleId, DWORD dwUserIndex, DWORD dwNumSettingIds, DWORD * pdwSettingIds, DWORD * pcbResults, XUSER_READ_PROFILE_SETTINGS * pResults, DWORD pOverlapped)
{
	if(*pcbResults < 1036) 
	{
		// TODO: make correct calculation by IDs.
		*pcbResults = 1036;
		return ERROR_INSUFFICIENT_BUFFER;
	}

	memset(pResults, 0, *pcbResults);
	pResults->dwLength = *pcbResults - sizeof(XUSER_READ_PROFILE_SETTINGS);
	pResults->pSettings =(BYTE *)pResults + sizeof(XUSER_READ_PROFILE_SETTINGS);
	return 0;
}

int WINAPI XSessionEnd(DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionArbitrationRegister(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) 
{ 
	return 0; 
}

DWORD WINAPI XTitleServerCreateEnumerator(DWORD, DWORD, DWORD, PHANDLE phEnum) 
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XSessionLeaveRemote(DWORD, DWORD, DWORD, DWORD)
{ 
	return 0; 
}

DWORD WINAPI XUserWriteProfileSettings(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XStorageBuildServerPath(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XStorageDownloadToMemory(DWORD dwUserIndex, DWORD, DWORD, DWORD, DWORD, DWORD, void *)
{
	return 0; 
}

DWORD WINAPI XLiveProtectedVerifyFile(HANDLE hContentAccess, VOID * pvReserved, PCWSTR pszFilePath)
{
	return 0;
}

DWORD WINAPI XLiveContentCreateAccessHandle(DWORD dwTitleId, void * pContentInfo, DWORD dwLicenseInfoVersion, void * xebBuffer, DWORD dwOffset, HANDLE * phAccess, void * pOverlapped)
{
	if(phAccess)
		*phAccess = INVALID_HANDLE_VALUE;

	// TODO: Fix it
	return E_OUTOFMEMORY;
}

DWORD WINAPI XLiveContentUninstall(void * pContentInfo, void * pxuidFor, void * pInstallCallbackParams)
{
	return 0;
}

DWORD WINAPI XLiveContentGetPath(DWORD dwUserIndex, void * pContentInfo, wchar_t * pszPath, DWORD * pcchPath)
{
	if(pcchPath)
		*pcchPath = 0;

	if(pszPath)
		*pszPath = 0;

	return 0;
}

DWORD WINAPI XLiveContentCreateEnumerator(DWORD, void *, DWORD *pchBuffer, HANDLE * phContent)
{
	if(phContent)
		*phContent = INVALID_HANDLE_VALUE;

	return 0;
}

DWORD WINAPI XLiveContentRetrieveOffersByDate(DWORD dwUserIndex, DWORD dwOffserInfoVersion, SYSTEMTIME * pstStartDate, void * pOffserInfoArray, DWORD * pcOfferInfo, void * pOverlapped)
{
	if(pcOfferInfo)
		*pcOfferInfo = 0;

	return 0;
} 

DWORD WINAPI XShowMarketplaceUI(DWORD dwUserIndex, DWORD dwEntryPoint, ULONGLONG dwOfferId, DWORD dwContentCategories)
{
	return 1;
}

DWORD WINAPI xlive_5367(HANDLE, DWORD, DWORD, BYTE *, DWORD)
{
	return 1;
}

DWORD WINAPI xlive_5372(HANDLE, DWORD, DWORD, DWORD, BYTE *, HANDLE)
{
	return 1;
}

CXLiveHook::CXLiveHook()
{
	m_bInstalled = false;
}

struct XLiveHook_t
{
	unsigned int uiOrdinal;
	DWORD dwFunction;
};

#define XLIVEHOOK_SIZE 121

XLiveHook_t xliveHookFunctions[XLIVEHOOK_SIZE] = 
{
	{ 1,   (DWORD)XWSAStartup },
	{ 2,   (DWORD)XWSACleanup },
	{ 3,   (DWORD)XSocketCreate },
	{ 4,   (DWORD)XSocketClose },
	{ 5,   (DWORD)XSocketShutdown },
	{ 6,   (DWORD)XSocketIOCTLSocket },
	{ 7,   (DWORD)XSocketSetSockOpt },
	{ 9,   (DWORD)XSocketGetSockName },
	{ 11,  (DWORD)XSocketBind },
	{ 12,  (DWORD)XSocketConnect },
	{ 13,  (DWORD)XSocketListen },
	{ 14,  (DWORD)XSocketAccept },
	{ 15,  (DWORD)XSocketSelect },
	{ 18,  (DWORD)XSocketRecv },
	{ 20,  (DWORD)XSocketRecvFrom },
	{ 22,  (DWORD)XSocketSend },
	{ 24,  (DWORD)XSocketSendTo },
	{ 26,  (DWORD)XSocketInet_Addr },
	{ 27,  (DWORD)XWSAGetLastError },
	{ 38,  (DWORD)XSocketNTOHS },
	{ 39,  (DWORD)XSocketNTOHL },
	{ 51,  (DWORD)XNetStartup },
	{ 52,  (DWORD)XNetCleanup },
	{ 57,  (DWORD)XNetXnAddrToInAddr },
	{ 58,  (DWORD)XNetServerToInAddr },
	{ 63,  (DWORD)XNetUnregisterInAddr },
	{ 66,  (DWORD)XNetGetConnectStatus },
	{ 69,  (DWORD)XNetQosListen },
	{ 70,  (DWORD)XNetQosLookup },
	{ 72,  (DWORD)XNetQosRelease },
	{ 73,  (DWORD)XNetGetTitleXnAddr },
	{ 75,  (DWORD)XNetGetEthernetLinkStatus },
	{ 84,  (DWORD)XNetSetSystemLinkPort },
	{ 651, (DWORD)XNotifyGetNext },
	{ 652, (DWORD)XNotifyPositionUI },
	{ 1082, (DWORD)XGetOverlappedExtendedError },
	{ 1083, (DWORD)XGetOverlappedResult },
	{ 5000, (DWORD)XLiveInitialize },
	{ 5001, (DWORD)XLiveInput },
	{ 5002, (DWORD)XLiveRender },
	{ 5003, (DWORD)XLiveUninitialize },
	{ 5005, (DWORD)XLiveOnCreateDevice },
	{ 5007, (DWORD)XLiveOnResetDevice },
	{ 5008, (DWORD)XHVCreateEngine },
	{ 5016, (DWORD)XLivePBufferAllocate },
	{ 5017, (DWORD)XLivePBufferFree },
	{ 5018, (DWORD)XLivePBufferGetByte },
	{ 5019, (DWORD)XLivePBufferSetByte },
	{ 5020, (DWORD)XLivePBufferGetDWORD },
	{ 5021, (DWORD)XLivePBufferSetDWORD },
	{ 5022, (DWORD)XLiveGetUpdateInformation },
	{ 5024, (DWORD)XLiveUpdateSystem },
	{ 5030, (DWORD)XLivePreTranslateMessage },
	{ 5034, (DWORD)XLiveProtectData },
	{ 5035, (DWORD)XLiveUnprotectData },
	{ 5036, (DWORD)XLiveCreateProtectedDataContext },
	{ 5037, (DWORD)XLiveQueryProtectedDataInformation },
	{ 5038, (DWORD)XLiveCloseProtectedDataContext },
	{ 5214, (DWORD)XShowPlayerReviewUI },
	{ 5215, (DWORD)XShowGuideUI },
	{ 5251, (DWORD)XCloseHandle },
	{ 5252, (DWORD)XShowGamerCardUI },
	{ 5254, (DWORD)XCancelOverlapped },
	{ 5256, (DWORD)XEnumerate },
	{ 5260, (DWORD)XShowSigninUI },
	{ 5261, (DWORD)XUserGetXUID },
	{ 5262, (DWORD)XUserGetSigninState },
	{ 5263, (DWORD)XUserGetName },
	{ 5264, (DWORD)XUserAreUsersFriends },
	{ 5265, (DWORD)XUserCheckPrivilege },
	{ 5267, (DWORD)XUserGetSigninInfo },
	{ 5270, (DWORD)XNotifyCreateListener },
	{ 5276, (DWORD)XUserSetProperty },
	{ 5277, (DWORD)XUserSetContext },
	{ 5278, (DWORD)XUserWriteAchievements },
	{ 5280, (DWORD)XUserCreateAchievementEnumerator },
	{ 5281, (DWORD)XUserReadStats },
	{ 5284, (DWORD)XUserCreateStatsEnumeratorByRank },
	{ 5286, (DWORD)XUserCreateStatsEnumeratorByXuid },
	{ 5292, (DWORD)XUserSetContextEx },
	{ 5294, (DWORD)XLivePBufferGetByteArray },
	{ 5295, (DWORD)XLivePBufferSetByteArray },
	{ 5297, (DWORD)XLiveInitializeEx },
	{ 5300, (DWORD)XSessionCreate },
	{ 5303, (DWORD)XStringVerify },
	{ 5305, (DWORD)XStorageUploadFromMemory },
	{ 5310, (DWORD)XOnlineStartup },
	{ 5311, (DWORD)XOnlineCleanup },
	{ 5312, (DWORD)XFriendsCreateEnumerator },
	{ 5314, (DWORD)XUserMuteListQuery },
	{ 5315, (DWORD)XInviteGetAcceptedInfo },
	{ 5316, (DWORD)XInviteSend },
	{ 5317, (DWORD)XSessionWriteStats },
	{ 5318, (DWORD)XSessionStart },
	{ 5319, (DWORD)XSessionSearchEx },
	{ 5322, (DWORD)XSessionModify },
	{ 5323, (DWORD)XSessionMigrateHost },
	{ 5324, (DWORD)XOnlineGetNatType },
	{ 5325, (DWORD)XSessionLeaveLocal },
	{ 5326, (DWORD)XSessionJoinRemote },
	{ 5327, (DWORD)XSessionJoinLocal },
	{ 5328, (DWORD)XSessionGetDetails },
	{ 5329, (DWORD)XSessionFlushStats },
	{ 5330, (DWORD)XSessionDelete },
	{ 5331, (DWORD)XUserReadProfileSettings },
	{ 5332, (DWORD)XSessionEnd },
	{ 5333, (DWORD)XSessionArbitrationRegister },
	{ 5335, (DWORD)XTitleServerCreateEnumerator },
	{ 5336, (DWORD)XSessionLeaveRemote },
	{ 5337, (DWORD)XUserWriteProfileSettings },
	{ 5344, (DWORD)XStorageBuildServerPath },
	{ 5345, (DWORD)XStorageDownloadToMemory },
	{ 5349, (DWORD)XLiveProtectedVerifyFile },
	{ 5350, (DWORD)XLiveContentCreateAccessHandle },
	{ 5352, (DWORD)XLiveContentUninstall },
	{ 5355, (DWORD)XLiveContentGetPath },
	{ 5360, (DWORD)XLiveContentCreateEnumerator },
	{ 5361, (DWORD)XLiveContentRetrieveOffersByDate },
	{ 5365, (DWORD)XShowMarketplaceUI },
	{ 5367, (DWORD)xlive_5367 },
	{ 5372, (DWORD)xlive_5372 }
};

void * pTrampolines[XLIVEHOOK_SIZE];

void CXLiveHook::Install()
{
	if(!m_bInstalled)
	{
		for(int i = 0; i < XLIVEHOOK_SIZE; i++)
		{
			pTrampolines[i] = CPatcher::InstallDetourPatch("xlive.dll", xliveHookFunctions[i].uiOrdinal, xliveHookFunctions[i].dwFunction);
		}

		m_bInstalled = true;
	}
}

void CXLiveHook::Uninstall()
{
	if(m_bInstalled)
	{
		for(int i = 0; i < XLIVEHOOK_SIZE; i++)
		{
			CPatcher::UninstallDetourPatch(pTrampolines[i], xliveHookFunctions[i].dwFunction);
		}

		m_bInstalled = false;
	}
}
