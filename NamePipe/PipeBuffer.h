#pragma once

#include <ByteIOS.h>
#include <Buffer.h>
#include <memory>
using namespace std;
#include <atlstr.h>
using namespace ATL;

struct CmdMeta 
{
	CmdMeta()
	{
		
	}
	UINT32 uFlag;	
	UINT16 uSize;
	UINT64 uSeq;
	UINT16 uCmd;
};

class CPipeBuffer
{
public:
	CPipeBuffer();
	~CPipeBuffer();
public:
	void Append(UINT8 *buf, UINT16 len);
	BOOL GetPackage(shared_ptr<Util::Buf::CByteIOS> package, CmdMeta &meta);
	void Empty();
protected:
private:
	Util::Buf::CBuffer m_buf;
};
