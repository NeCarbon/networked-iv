//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CString.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>
#include <string>

class String
{
private:
	std::string m_strString;
	size_t m_sLimit;

	void          Init();

public:
	// Undefined position value
	static const unsigned int nPos;

	String();
	String(const char * szFormat, ...);
	~String();

	operator const char * () const;

	// Assignment operator
	String& operator = (const char * szString);
	String& operator = (const String strString);

	// Addition operator
	String& operator += (const char * szString);
	String& operator += (const String strString);
	String& operator += (const unsigned char ucChar);

	unsigned char operator [] (size_t sOffset) const;

	bool operator == (const char * szString) const;
	bool operator == (const String strString) const;

	bool operator != (const char * szString) const;
	bool operator != (const String strString) const;

	// More than operator
	bool operator > (const char * szString) const;
	bool operator > (const String strString) const;

	// More than or equal to operator
	bool operator >= (const char * szString) const;
	bool operator >= (const String strString) const;

	// Less than operator
	bool operator < (const char * szString) const;
	bool operator < (const String strString) const;

	// Less than or equal to operator
	bool operator <= (const char * szString) const;
	bool operator <= (const String strString) const;

	// Return the non editable string
	const char *  Get() const;
	const char *  C_String() const;

	// Set the string
	void          Set(const char * szString);
	void          Set(const String strString);

	// Format the string
	void          Format(const char * szFormat, ...);

	// Return the string length
	size_t        GetLength() const;

	// Return true if the string length is 0,
	// false if not
	bool          IsEmpty() const;

	// Convert all a-z chars in the string to
	// lowercase
	void          ToLower();

	// Return true if all a-z chars in the string
	// are lowercase, false if not
	bool          IsLower() const;

	// Convert all a-z chars in the string to
	// uppercase
	void          ToUpper();

	// Return true if all a-z chars in the string
	// are uppercase, false if not
	bool          IsUpper() const;

	// Truncate (null terminate) the string at sOffset
	void          Truncate(size_t sOffset);

	// Erase sCount amount of bytes at sOffset in the
	// string
	void          Erase(size_t sOffset, size_t sCount);

	// Clear the string contents and free the allocated
	// memory
	void          Clear();

	// Compare the string with sz/strString (case sensitive)
	int           Compare(const char * szString) const;
	int           Compare(const String strString) const;
	int           StrCmp(const char * szString) const { return Compare(szString); }
	int           StrCmp(const String strString) const { return Compare(strString); }

	// Compare the string with sz/strString (case insensitive)
	int           ICompare(const char * szString) const;
	int           ICompare(const String strString) const;
	int           StrICmp(const char * szString) const { return ICompare(szString); }
	int           StrICmp(const String strString) const { return ICompare(strString); }

	// Return the value of the string as an integer
	int           ToInteger() const;

	// Return the value of the string as a boolean
	bool          ToBoolean() const;

	// Set the char at sOffset to cChar
	void          SetChar(size_t sOffset, unsigned char cChar);

	// Return the char at sOffset
	unsigned char GetChar(size_t sOffset) const;

	// Return a substituted string from the string 
	// at sOffset with a size of sCount
	String        SubStr(size_t sOffset, size_t sCount) const;

	// Replace the string at sOffset with sz/strString
	void          Replace(size_t sOffset, const char * szString);
	void          Replace(size_t sOffset, String strString);

	// Append sz/strString to the string
	void          Append(const char * szString);
	void          Append(const String strString);

	// Append szFormat and variable arguments to the string
	void          AppendF(const char * szFormat, ...);

	// Append ucChar to the string
	void          Append(const unsigned char ucChar);

	// Insert szString into the string at sOffset
	void          Insert(size_t sOffset, const char * szString);

	// Attempt to find sz/strString in the string after sPos, 
	// if found return its index, if not return nPos
	size_t        Find(const char * szString, size_t sPos = 0) const;
	size_t        Find(const unsigned char ucChar, size_t sPos = 0) const;
	size_t        Find(const String strString, size_t sPos = 0) const;

	// Return true if the string contains sz/strString after sPos, 
	// false if not
	bool          Contains(const char * szString, size_t sPos = 0) const;
	bool          Contains(const unsigned char ucChar, size_t sPos = 0) const;
	bool          Contains(const String strString, size_t sPos = 0) const;

	// Starting at the end, attempt to find sz/strString 
	// in the string after sPos, if found return its index, if 
	// not return nPos
	size_t        ReverseFind(const char * szString, size_t sPos = 0) const;
	size_t        ReverseFind(const unsigned char ucChar, size_t sPos = 0) const;
	size_t        ReverseFind(const String strString, size_t sPos = 0) const;

	// Set the string limit
	bool          SetLimit(size_t sLimit);

	// Return the string limit
	size_t        GetLimit() const;

	// Truncate (null terminate) if the string is
	// longer than the set limit
	bool          LimitTruncate();

	// Ensure we have sSize amount of memory
	// allocated
	void          Allocate(size_t sSize);

	// Resize the string to sSize and pad any
	// extra bytes with 0
	void          Resize(size_t sSize);

	// Free any allocated memory
	void          Free();

	// Return the amount of memory allocated
	size_t        GetSize() const;

	// Return true if we have any memory allocated,
	// false if not
	bool          IsAllocated() const;

	// Return the data string
	char *        GetData() const;
};

inline String operator + (const String&strString, const String& strString2) { String strNewString(strString); strNewString.Append(strString2); return strNewString; }
