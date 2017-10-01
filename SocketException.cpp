#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <exception>
#include "SocketException.h"

using namespace std;

SocketException::SocketException(const SocketException &tmp)
{
	msg = NULL;
	nbrErr = 0;
	setMsg(tmp.getMsg());
	setNbrErr(tmp.getNbrErr());
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
















