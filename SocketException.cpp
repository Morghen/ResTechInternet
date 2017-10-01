#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <exception>
#include "SocketException.h"

using namespace std;

/*
ERRORINIT			=		1;
ERRORCONNECT		=		2;
ERRORLISTEN			=		3;
ERRORACCEPT			=		4;
ERRORMSGRCV			=		5;
ERRORMSGSEND		=		6;
*/

SocketException::SocketException(const SocketException &tmp)
{
	msg = NULL;
	nbrErr = 0;
	setMsg(tmp.getMsg());
	setNbrErr(tmp.getNbrErr());
}

SocketException::SocketException(int n)
{
	msg = NULL;
	nbrErr = 0;
	setNbrErr(n);
	switch(n)
	{
		case ERRORINIT	:
			setMsg("Error init socket");
			break;
		case ERRORCONNECT:
			setMsg("Error connect client");
			break;
		case ERRORLISTEN:
			setMsg("Error listen server");
			break;
		case ERRORACCEPT:
			setMsg("Error accept server");
			break;
		case ERRORMSGRCV:
			setMsg("Error message receive");
			break;
		case ERRORMSGSEND:
			setMsg("Error message send");
			break;
		default:
			setMsg("Error not defined");
	}
}


SocketException::~SocketException() throw()
{
	if(msg != NULL)delete msg;
}




int SocketException::setMsg(const char *tmp)
{
	if(getMsg() != NULL) delete msg;
	if(tmp != NULL)
	{
		msg = new char[strlen(tmp)+1];
		strcpy(msg, tmp);
	}
	return 1;
}
















