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
		SocketException()
		{msg = NULL; nbrErr=0;};
		SocketException(const char *tmp)
		{msg = NULL; nbrErr=0; setMsg(tmp);};
		
		SocketException(const char *tmp, int n)
		{msg = NULL; nbrErr=0; setMsg(tmp); setNbrErr(n);};
		
		SocketException(int n);
		
		SocketException(const SocketException &tmp);
		
		~SocketException() throw();
		
		char *getMsg() const {return msg;};
		int getNbrErr() const {return nbrErr;};
		
		static const int ERRORINIT				=		1;
		static const int ERRORCONNECT			=		2;
		static const int ERRORLISTEN			=		3;
		static const int ERRORACCEPT			=		4;
		static const int ERRORMSGRCV			=		5;
		static const int ERRORMSGSEND			=		6;
};









#endif
