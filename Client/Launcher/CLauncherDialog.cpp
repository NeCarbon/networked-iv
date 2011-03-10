// LauncherDlg.cpp : implementation file
//

#include <StdInc.h>
#include "CLauncherApp.h"
#include "CLauncherDialog.h"

int ShowMessageBox(char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK))
{
	return MessageBox(NULL, szText, MOD_NAME, uType);
}

bool GetProcessIdFromProcessName(char * szProcessName, DWORD * dwProcessId)
{
	bool bReturn = false;

	// Create a tool help 32 process snapshot
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(!hProcessSnapShot)
		return false;

	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);

	if(Process32First(hProcessSnapShot, &ProcessEntry))
	{
		// Enumerate through all processes
		while(Process32Next(hProcessSnapShot, &ProcessEntry))
		{
			// Check the process name to see if it matches szProcessName
			if(!strcmp(ProcessEntry.szExeFile, szProcessName))
			{
				// It matches, set the process id (if required) and return true
				if(dwProcessId)
					*dwProcessId = ProcessEntry.th32ProcessID;

				bReturn = true;
				break;
			}
		}
	}

	// Close the snapshot handle
	CloseHandle(hProcessSnapShot); 

	return bReturn;
}

bool IsProcessRunning(char * szProcessName)
{
	// Simply return the value of GetProcessIdFromProcessName
	return GetProcessIdFromProcessName(szProcessName, NULL);
}

// CLauncherDialog dialog

CLauncherDialog::CLauncherDialog(CWnd* pParent) : CDialog(CLauncherDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ipAddressEditBox);
	DDX_Control(pDX, IDC_EDIT2, m_port);
	DDX_Control(pDX, IDC_EDIT3, m_password);
	DDX_Control(pDX, IDC_EDIT4, m_nick);
}

BEGIN_MESSAGE_MAP(CLauncherDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CLauncherDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CLauncherDialog::OnBnClickedOk)
END_MESSAGE_MAP()

void CLauncherDialog::LoadInfo()
{
	char szIp[128];
	char szPort[16];
	char szNick[32];

	// Get the values from the registry
	SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "ip", "127.0.0.1", szIp, sizeof(szIp));
	SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "port", "9999", szPort, sizeof(szPort));
	SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "nick", "Player", szNick, sizeof(szNick));

	// Set the edit boxes values
	SetDlgItemText(IDC_EDIT1, szIp);
	SetDlgItemText(IDC_EDIT2, szPort);
	SetDlgItemText(IDC_EDIT4, szNick);
}

void CLauncherDialog::SaveInfo()
{
	char szIp[128];
	char szPort[16];
	char szNick[32];

	// Get the values from the edit boxes
	GetDlgItemText(IDC_EDIT1, szIp, sizeof(szIp));
	GetDlgItemText(IDC_EDIT2, szPort, sizeof(szPort));
	GetDlgItemText(IDC_EDIT4, szNick, sizeof(szNick));

	// Save the values to the registry
	SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "ip", szIp, strlen(szIp));
	SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "port", szPort, strlen(szPort));
	SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "nick", szNick, strlen(szNick));
}

// CLauncherDialog message handlers

BOOL CLauncherDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set window text
	SetWindowText(MOD_NAME " v" MOD_VERSION_STRING " Launcher");

	// Load the edit box values
	LoadInfo();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLauncherDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CLauncherDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLauncherDialog::OnBnClickedCancel()
{
	// Save the edit box values
	SaveInfo();
	OnCancel();
}

void CLauncherDialog::OnBnClickedOk()
{
	CString ipAddress;
	CString port;
	CString password;
	CString nick;
	GetDlgItemText(IDC_EDIT1, ipAddress);

	if(ipAddress.IsEmpty())
	{
		MessageBox("No ip address entered.");
		return;
	}

	GetDlgItemText(IDC_EDIT2, port);

	if(port.IsEmpty())
	{
		MessageBox("No port entered.");
		return;
	}

	GetDlgItemText(IDC_EDIT3, password);
	GetDlgItemText(IDC_EDIT4, nick);

	if(nick.IsEmpty())
	{
		MessageBox("No nick entered.");
		return;
	}

	// Get the GTA IV install directory from the registry
	char szInstallDirectory[MAX_PATH];
	bool bFoundCustomDirectory = false;

	if(!SharedUtility::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\Rockstar Games\\Grand Theft Auto IV", 
										  "InstallFolder", NULL, szInstallDirectory, sizeof(szInstallDirectory)) || 
		!SharedUtility::Exists(szInstallDirectory))
	{
		if(!SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "gtaivdir", NULL, 
											  szInstallDirectory, sizeof(szInstallDirectory)) || 
			!SharedUtility::Exists(szInstallDirectory))
		{
			if(ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Specify your GTA IV path now?", 
				(MB_ICONEXCLAMATION | MB_OKCANCEL)) == IDOK)
			{
				// Taken from http://vcfaq.mvps.org/sdk/20.htm
				BROWSEINFO browseInfo = { 0 };
				browseInfo.lpszTitle = "Pick a Directory";
				ITEMIDLIST * pItemIdList = SHBrowseForFolder(&browseInfo);

				if(pItemIdList != NULL)
				{
					// Get the name of the selected folder
					if(SHGetPathFromIDList(pItemIdList, szInstallDirectory))
						bFoundCustomDirectory = true;

					// Free any memory used
					IMalloc * pIMalloc = 0;
					if(SUCCEEDED(SHGetMalloc(&pIMalloc)))
					{
						pIMalloc->Free(pItemIdList);
						pIMalloc->Release();
					}
				}
			}

			if(!bFoundCustomDirectory)
			{
				ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Cannot launch Networked: IV.");
				return;
			}
		}
	}

	// Get the full path to LaunchGTAIV.exe
	char szApplicationPath[MAX_PATH];
	sprintf(szApplicationPath, "%s\\LaunchGTAIV.exe", szInstallDirectory);

	// Check if LaunchGTAIV.exe exists
	if(!SharedUtility::Exists(szApplicationPath))
	{
		ShowMessageBox("Failed to find LaunchGTAIV.exe. Cannot launch Networked: IV.");
		return;
	}

	// If we have a custom directory save it
	if(bFoundCustomDirectory)
		SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\NIV", "gtaivdir", szInstallDirectory, strlen(szInstallDirectory));

	// Format the command line params
	char szParams[256];
	sprintf(szParams, "\"%s\" -ip %s -port %s -nick %s", szApplicationPath, ipAddress, port, nick);

	// Do we have a password?
	if(!password.IsEmpty())
	{
		// Append it to the command line params
		strcat(szParams, " -password ");
		strcat(szParams, password);
	}

	// Save the edit box values
	SaveInfo();

	// Get the full path of the client core
	char szLibraryPath[MAX_PATH];
	sprintf(szLibraryPath, "%s" CLIENT_CORE_NAME DEBUG_SUFFIX ".dll", SharedUtility::GetAppPath());

	// Check if the client core exists
	if(!SharedUtility::Exists(szLibraryPath))
	{
		ShowMessageBox("Failed to find " CLIENT_CORE_NAME DEBUG_SUFFIX ".dll. Cannot launch Networked: IV.");
		return;
	}

	// Get the full path of the launcher library dll
	sprintf(szLibraryPath, "%s" CLIENT_LAUNCH_HELPER DEBUG_SUFFIX ".dll", SharedUtility::GetAppPath());

	// Check if the launch helper exists
	if(!SharedUtility::Exists(szLibraryPath))
	{
		ShowMessageBox("Failed to find " CLIENT_LAUNCH_HELPER DEBUG_SUFFIX ".dll. Cannot launch Networked: IV.");
		return;
	}

	// Check if LaunchGTAIV.exe is already running
	if(IsProcessRunning("LaunchGTAIV.exe"))
	{
		ShowMessageBox("LaunchGTAIV.exe is already running. Cannot launch Networked: IV.");
		return;
	}

	// Check if GTAIV.exe is already running
	if(IsProcessRunning("GTAIV.exe"))
	{
		ShowMessageBox("GTAIV.exe is already running. Cannot launch Networked: IV.");
		return;
	}

	// Start LaunchGTAIV.exe
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);
	if(!CreateProcess(szApplicationPath, szParams, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, 
		SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo))
	{
		ShowMessageBox("Failed to start LaunchGTAIV.exe. Cannot launch Networked: IV.");
		return;
	}

	// Inject LauncherLibrary.dll into LaunchGTAIV.exe
	int iReturn = SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, szLibraryPath);

	// Did the injection fail?
	if(iReturn > 0)
	{
		// Terminate the process
		TerminateProcess(piProcessInfo.hProcess, 0);

		if(iReturn == 1)
			ShowMessageBox("Failed to write library path into remote process. Cannot launch Networked: IV.");
		else if(iReturn == 2)
			ShowMessageBox("Failed to create remote thread in remote process. Cannot launch Networked: IV.");
		else if(iReturn == 3)
			ShowMessageBox("Failed to open the remote process, Cannot launch Networked: IV.");

		return;
	}

	// Resume the LaunchGTAIV.exe thread
	ResumeThread(piProcessInfo.hThread);
}
