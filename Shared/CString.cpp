//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CString.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#include <StdInc.h>

#define BUFFER_SIZE 8192
#ifdef WIN32
#define stricmp _stricmp
#endif

const unsigned int String::nPos = (unsigned int)-1;

String::String()
{
	Init();
}

String::String(const char * szFormat, ...)
{
	Init();

	if(szFormat)
	{
		char szString[BUFFER_SIZE];
		va_list vaArgs;
		va_start(vaArgs, szFormat);
		vsprintf(szString, szFormat, vaArgs);
		va_end(vaArgs);
		Set(szString);
	}
}

String::~String()
{

}

String::operator const char * () const
{
	return Get();
}

String& String::operator = (const char * szString)
{
	Set(szString);
	return *this;
}

String& String::operator = (const String strString)
{
	Set(strString.C_String());
	return *this;
}

String& String::operator += (const char * szString)
{
	Append(szString);
	return *this;
}

String& String::operator += (const String strString)
{
	Append(strString.Get());
	return *this;
}

String& String::operator += (const unsigned char ucChar)
{
	Append(ucChar);
	return *this;
}

unsigned char String::operator [] (size_t sOffset) const
{
	return GetChar(sOffset);
}

bool String::operator == (const char * szString) const
{
	return (Compare(szString) == 0);
}

bool String::operator == (const String strString) const
{
	return (Compare(strString.Get()) == 0);
}

bool String::operator != (const char * szString) const
{
	return (Compare(szString) != 0);
}

bool String::operator != (const String strString) const
{
	return (Compare(strString.Get()) != 0);
}

bool String::operator > (const char * szString) const
{
	return (Compare(szString) > 0);
}

bool String::operator > (const String strString) const
{
	return (Compare(strString.Get()) > 0);
}

bool String::operator >= (const char * szString) const
{
	return (Compare(szString) >= 0);
}

bool String::operator >= (const String strString) const
{
	return (Compare(strString.Get()) >= 0);
}

bool String::operator < (const char * szString) const
{
	return (Compare(szString) < 0);
}

bool String::operator < (const String strString) const
{
	return (Compare(strString.Get()) < 0);
}

bool String::operator <= (const char * szString) const
{
	return (Compare(szString) <= 0);
}

bool String::operator <= (const String strString) const
{
	return (Compare(strString.Get()) <= 0);
}

void String::Init()
{
	// TODO: Change no limit to 0?
	m_sLimit = nPos;
}

const char * String::Get() const
{
	return m_strString.c_str();
}

const char * String::C_String() const
{
	return m_strString.c_str();
}

void String::Set(const char * szString)
{
	// Make sure the string is valid
	if(szString)
	{
		// Set the string
		m_strString.assign(szString);

		// Ensure we haven't passed the string limit
		LimitTruncate();
	}
}

void String::Set(const String strString)
{
	return Set(strString.C_String());
}

void String::Format(const char * szFormat, ...)
{
	char szString[BUFFER_SIZE];
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsprintf(szString, szFormat, vaArgs);
	va_end(vaArgs);
	Set(szString);
}

size_t String::GetLength() const
{
	return m_strString.size();
}

bool String::IsEmpty() const
{
	return m_strString.empty();
}

void String::ToLower()
{
	for(size_t i = 0; i < GetLength(); i++)
		m_strString[i] = tolower(m_strString[i]);
}

bool String::IsLower() const
{
	for(size_t i = 0; i < GetLength(); i++)
	{
		unsigned char cChar = m_strString[i];

		if(((cChar >= 'a' && cChar <= 'Z') || (cChar >= 'A' && cChar <= 'Z')) && !islower(cChar))
			return false;
	}

	return true;
}

void String::ToUpper()
{
	for(size_t i = 0; i < GetLength(); i++)
		m_strString[i] = toupper(m_strString[i]);
}

bool String::IsUpper() const
{
	for(size_t i = 0; i < GetLength(); i++)
	{
		unsigned char cChar = m_strString[i];

		if(((cChar >= 'a' && cChar <= 'Z') || (cChar >= 'A' && cChar <= 'Z')) && !isupper(cChar))
			return false;
	}

	return true;
}

void String::Truncate(size_t sOffset)
{
	if(sOffset < GetLength())
		m_strString.resize(sOffset);
}

void String::Erase(size_t sOffset, size_t sCount)
{
	m_strString.erase(sOffset, sCount);
}

void String::Clear()
{
	m_strString.clear();
}

int String::Compare(const char * szString) const
{
	return strcmp(Get(), szString);
}

int String::Compare(const String strString) const
{
	return strcmp(Get(), strString.Get());
}

int String::ICompare(const char * szString) const
{
	return stricmp(Get(), szString);
}

int String::ICompare(const String strString) const
{
	return stricmp(Get(), strString.Get());
}

int String::ToInteger() const
{
	return atoi(Get());
}

bool String::ToBoolean() const
{
	return (ToInteger() != 0);
}

void String::SetChar(size_t sOffset, unsigned char cChar)
{
	// TODO: Use allocated size instead of/aswell as the string length?
	if(sOffset < GetLength())
		m_strString[sOffset] = cChar;
}

unsigned char String::GetChar(size_t sOffset) const
{
	// TODO: Use allocated size instead of/aswell as the string length?
	if(sOffset < GetLength())
		return m_strString[sOffset];

	return 0;
}

String String::SubStr(size_t sOffset, size_t sCount) const
{
	return String(m_strString.substr(sOffset, sCount).c_str());
}

void String::Replace(size_t sOffset, const char * szString)
{
	// TODO:
	//m_strString.replace(sOffset, szString);
}

void String::Replace(size_t sOffset, String strString)
{
	// TODO:
	//m_strString.replace(sOffset, strString.Get());
}

void String::Append(const char * szString)
{
	// Make sure the string is valid
	if(szString)
	{
		// Copy the string to the end of our string
		m_strString.append(szString);

		// Ensure we haven't passed the string limit
		LimitTruncate();
	}
}

void String::Append(const String strString)
{
	return Append(strString.Get());
}

void String::AppendF(const char * szFormat, ...)
{
	// Make sure the format is valid
	if(szFormat)
	{
		char szString[BUFFER_SIZE];
		va_list vaArgs;
		va_start(vaArgs, szFormat);
		vsprintf(szString, szFormat, vaArgs);
		va_end(vaArgs);

		// Copy the string to the end of our string
		m_strString.append(szString);

		// Ensure we haven't passed the string limit
		LimitTruncate();
	}
}

void String::Append(const unsigned char ucChar)
{
	// TODO: Find another way to do this
	// Construct the string to append
	char szString[2];
	szString[0] = ucChar;
	szString[1] = '\0';

	// Copy the string to the end of our string
	m_strString.append(szString);

	// Ensure we haven't passed the string limit
	LimitTruncate();
}

void String::Insert(size_t sOffset, const char * szString)
{
	m_strString.insert(sOffset, szString);

	// Ensure we haven't passed the string limit
	LimitTruncate();
}

size_t String::Find(const char * szString, size_t sPos) const
{
	return m_strString.find(szString, sPos);
}

size_t String::Find(const unsigned char ucChar, size_t sPos) const
{
	return m_strString.find(ucChar, sPos);
}

size_t String::Find(const String strString, size_t sPos) const
{
	return m_strString.find(strString.Get(), sPos);
}

bool String::Contains(const char * szString, size_t sPos) const
{
	return (Find(szString, sPos) != nPos);
}

bool String::Contains(const unsigned char ucChar, size_t sPos) const
{
	return (Find(ucChar, sPos) != nPos);
}

bool String::Contains(const String strString, size_t sPos) const
{
	return (Find(strString.Get(), sPos) != nPos);
}

size_t String::ReverseFind(const char * szString, size_t sPos) const
{
	return m_strString.rfind(szString, sPos);
}

size_t String::ReverseFind(const unsigned char ucChar, size_t sPos) const
{
	return m_strString.rfind(ucChar, sPos);
}

size_t String::ReverseFind(const String strString, size_t sPos) const
{
	return m_strString.rfind(strString.Get(), sPos);
}

bool String::SetLimit(size_t sLimit)
{
	m_sLimit = sLimit;
	return true;
}

size_t String::GetLimit() const
{
	return m_sLimit;
}

bool String::LimitTruncate()
{
	if(m_sLimit > nPos && GetLength() > m_sLimit)
	{
		m_strString[m_sLimit] = '\0';
		return true;
	}

	return false;
}

// NOTE: If size is less than is allocated this will
// free the extra memory, so allocate is bad naming,
// should be something along the lines of "Reallocate"
void String::Allocate(size_t sSize)
{
	// If the size is 0 or less free the buffer
	/*if(sSize == 0)
	{
		Free();
		return;
	}*/

	m_strString.reserve(sSize);
}

void String::Resize(size_t sSize)
{
	m_strString.resize(sSize);
}

void String::Free()
{
	m_strString.clear();
}

size_t String::GetSize() const
{
	return m_strString.capacity();
}

bool String::IsAllocated() const
{
	//return (m_strString->capacity() > 0);
	return true;
}

char * String::GetData() const
{
	// TODO: Better way?
	return (char *)m_strString.data();
}
