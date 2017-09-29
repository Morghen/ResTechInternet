#ifndef __SOCKETEXCEPTION_H__
#define __SOCKETEXCEPTION_H__

#include <exception>

using namespace std;

class SocketException : public exception
{
	protected:
		char *msg;
		int nbrErr;
		int setMsg(const char *tmp);
		int setNbrErr(int n)
		{nbrErr = n; return 1;};
		
	public:
		//init
		explicit SocketException()
		{msg = NULL; nbrErr=0;};
		explicit SocketException(const char *tmp)
		{msg = NULL; nbrErr=0; setMsg(tmp);};
		
		explicit SocketException(const char *tmp, int n)
		{msg = NULL; nbrErr=0; setMsg(tmp); setNbrErr(n);};
		
		explicit SocketException(const SocketException &tmp);
		
		~SocketException() throw();
		
		char *getMsg() const {return msg;};
		int getNbrErr() const {return nbrErr;};
};









#endif
