//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CSquirrelArguments.h
// Project: Server
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==========================================================================

#pragma once

#include <StdInc.h>
#include <squirrel/sqstdaux.h>
#include <squirrel/sqstdblob.h>
#include <squirrel/sqstdio.h>
#include <squirrel/sqstdmath.h>
#include <squirrel/sqstdstring.h>
#include <squirrel/sqstdsystem.h>

class CSquirrelArguments;

class CSquirrelArgument
{
private:
	SQObjectType type;

public:
	union {
		int i;
		bool b;
		float f;
		String* str;
		CEntity* pEntity;
		CSquirrelArguments* pArray;
	} data;

	CSquirrelArgument(){type=OT_NULL;}
	CSquirrelArgument(int i){type=OT_INTEGER; data.i=i;}
	CSquirrelArgument(bool b){type=OT_BOOL; data.b=b;}
	CSquirrelArgument(float f){type=OT_FLOAT; data.f=f;}
	CSquirrelArgument(String* str){type=OT_STRING; data.str = new String(*str);} // clone the string instead of referencing it
	CSquirrelArgument(CEntity* pEntity){type=OT_FUNCPROTO; data.pEntity=pEntity;}
	CSquirrelArgument(CSquirrelArguments* pArray, bool isArray);

	~CSquirrelArgument();

	SQObjectType GetType(){return type;}

	bool push(SQVM* pVM);
};

class CSquirrelArguments : public std::list<CSquirrelArgument*>
{
public:
	CSquirrelArguments(){}
	~CSquirrelArguments(){for(iterator iter = begin(); iter != end(); ++iter){delete *iter;}}

	void push_to_vm(SQVM* pVM);

	// All kind of push functions
	void push();
	void push(SQObject o);
	void push(int i);
	void push(bool b);
	void push(float f);
	void push(const char* c);
	void push(String str);
	void push(CEntity* pEntity);
	void push(CVector3 vec);
	void push(CSquirrelArguments* pArray, bool isArray);
	bool pushFromStack(SQVM* pVM, int idx);
	void pushFromStack(SQVM* pVM, int startidx, int count);
};
