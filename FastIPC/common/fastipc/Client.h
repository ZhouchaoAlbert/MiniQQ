//////////////////////////////////////////////////////////////////////////  
/// ����fastipc�Ŀͻ���
/// �ͻ�����ֻ��д���ݸ�����ˡ�����ʹ�òμ�[Server.h]��
///  
/// Դ�룺http://git.oschina.net/washheart/fastipc
/// Դ�룺https://github.com/washheart/fastipc
/// ˵����https://github.com/washheart/fastipc/wiki
/// @version	1.0 
/// @author		washheart@163.com
/// @date       2015-10-22  
////////////////////////////////////////////////////////////////////////// 
#pragma once

#ifndef FastIPC_Client_H
#define FastIPC_Client_H

#include <Windows.h>
#include "FastIPC.h"

namespace fastipc{
	class FASTIPC_API  Client{
	public:
		Client(void);
		~Client();
	private:
		HANDLE		evtWrited;		// ����һ���¼���д��������ɣ����Զ���
		HANDLE		evtReaded;		// ����һ���¼�������������ɣ�����д��
		HANDLE		mapFile;		// �ڴ�ӳ���ļ����
		MemBuff*	memBuf;			// �������ڴ�����ݽṹ
		DWORD		blockSize = 2;	// ����memBuf.data�Ŀռ䳤��

	public:
		/// ����������
		/// @param serverName ����������
		/// @param blockSize ����memBuf.data�Ŀռ䳤�ȣ�Ĭ����2048����1024������
		/// @return 0=�ɹ�������ֵ��ʾʧ�ܣ����峣���μ�FastIPC.h�е�ERR_*����
		int	create(const std::wstring, DWORD blockSize);

		/// �رտͻ���
		void close(void);

		/// д���ݵ������ڴ棬�����ݵĳ��ȴ���MEM_SIZEʱ����Ϊ���MemBuff������
		/// @param pBuff �����͵�����
		/// @param len	 ��Ҫ���͵ĳ���
		/// @return 0=�ɹ�������ֵ��ʾʧ�ܣ����峣���μ�FastIPC.h�е�ERR_*����
		DWORD write(char *pBuff, DWORD len);		// 

		/// �ͻ���״̬�Ƿ���׼����
		/// @return true=׼����
		bool isStable();

	private:
		/// д���ݵ������ڴ�
		/// @param pBuff	�����͵�����
		/// @param len		��Ҫ���͵ĳ��ȣ�len��С�ڵ���MEM_SIZE��
		/// @param packId	�����ʹ�����ʱ�����ݴ�id�����MemBuff�����һ��
		/// @param msgType	���͵����ݵ�����
		/// @return 0=�ɹ�������ֵ��ʾʧ�ܣ����峣���μ�FastIPC.h�е�ERR_*����
		DWORD Client::writeBlock(char *pBuff, DWORD len, char* packId, int msgType);
	};
}
#endif