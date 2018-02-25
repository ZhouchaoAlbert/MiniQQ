//////////////////////////////////////////////////////////////////////////  
/// ����fastipc�ķ�����
/// �������ֻ�������Ŀͻ��˵�д��Ϣ��
/// �����Ҫ˫��ͨ�ţ���ô�����(S1)��Ӧ�Ŀͻ���(C1)��Ҫ����һ������ˣ�S2����Ȼ������(S1)����һ���ͻ���(C2)����д��
/// ��S1����C1��д�¼���S2����C2��д�¼���
/// ������÷����������£�create��serListerner��startRead��close��
///  
/// Դ�룺http://git.oschina.net/washheart/fastipc
/// Դ�룺https://github.com/washheart/fastipc
/// ˵����https://github.com/washheart/fastipc/wiki
/// @version	1.0 
/// @author		washheart@163.com
/// @date       2015-10-22  
////////////////////////////////////////////////////////////////////////// 
#pragma once
#ifndef FastIPC_Server_H
#define FastIPC_Server_H

#include <Windows.h>
#include <map>
#include "FastIPC.h"

namespace fastipc{
	class FASTIPC_API ReadListener{
	public:
		// ���з���˶�ȡ�����ݺ󣬻���ô˷���֪ͨ�ȴ��߽��д���
		// memBlock�ڷַ�����ɷ��������٣��ⲿ�����������������
		virtual void onRead(MemBlock* memBlock){}
	};

	template  class FASTIPC_API std::map < std::string, MemBlock* >;
	// �Ѿ������ݽ�������װ�����ÿ�����������
	class FASTIPC_API RebuildedBlockListener :public ReadListener{
	private:
		typedef std::map<std::string, MemBlock*> RebuildedBlockMap;// ����һ���洢BrowserWindowInfo��Map
		RebuildedBlockMap rebuildBlocks;
		MemBlock * getRebuildedBlock(std::string packId){
			RebuildedBlockMap::iterator it = rebuildBlocks.find(packId);
			if (rebuildBlocks.end() != it) {
				return it->second;
			}
			return NULL;
		}
	public:
		virtual void onRebuildedRead(MemBlock* memBlock){}
	//private:
		// ���з���˶�ȡ�����ݺ󣬻���ô˷���֪ͨ�ȴ��߽��д���
		// memBlock�ڷַ�����ɷ��������٣��ⲿ�����������������
		void onRead(MemBlock* readed){
			if (readed->msgType == MSG_TYPE_NORMAL){
				onRebuildedRead(readed);// ��ͨ��Ϣ��ֱ��ת��
			}
			else{// ��ȡ���������õ�uuid�����ڷ��Ͷ˿����Ƕ��߳̽����ͣ�����������map����id�����ݿ�Ĺ�ϵ��
				char * uid;
				uid = (char *)malloc(PACK_ID_LEN + 1);
				memcpy(uid, readed->packId, PACK_ID_LEN);
				uid[PACK_ID_LEN] = '\0';
				std::string packId(uid);
				MemBlock * tmpBlock = getRebuildedBlock(packId);
				try{
					if (!tmpBlock){
						tmpBlock = new MemBlock();
						tmpBlock->msgType = MSG_TYPE_NORMAL;
						tmpBlock->dataLen = 0;
						rebuildBlocks.insert(std::pair<std::string, MemBlock*>(packId, tmpBlock));
					}
					int len = readed->dataLen;
					tmpBlock->data = (char*)realloc(tmpBlock->data, tmpBlock->dataLen+len);
					memcpy((tmpBlock->data + tmpBlock->dataLen), readed->data, len);// ׷�ӿ�������
					tmpBlock->dataLen = tmpBlock->dataLen + len;
					if (readed->msgType == MSG_TYPE_END){
						rebuildBlocks.erase(packId);// ��map���Ƴ�
						onRebuildedRead(tmpBlock);// ������ɣ�ת��
						delete tmpBlock;// ������
					}
				}
				catch (...){
					delete tmpBlock;// ������
				}
			}
		}

	};

	class  FASTIPC_API Server{
	public:
		Server();
		~Server();

	private:
		HANDLE			evtWrited;	// ����һ���¼���д��������ɣ����Զ���
		HANDLE			evtReaded;	// ����һ���¼�������������ɣ�����д��
		HANDLE			mapFile;	// �ڴ�ӳ���ļ����
		MemBuff*		memBuf;		// �������ڴ�����ݽṹ
		//DWORD			blockSize;	// ����memBuf.data�Ŀռ䳤��
		ReadListener*	listener;	// �������¼��������д���
	public:
		/// ����������
		/// @param serverName ����������
		/// @param blockSize ����memBuf.data�Ŀռ䳤�ȣ�Ĭ����2048����1024������
		/// @return 0=�ɹ�������ֵ��ʾʧ�ܣ����峣���μ�FastIPC.h�е�ERR_*����
		int	create(const std::wstring serverName, DWORD blockSize);

		/// �رշ�����
		void close(void);

		/// Ϊ����������һ�����������Ա��ȡ���ݲ����д���
		/// @param listener һ��������
		void setListener(ReadListener* listener);

		/// ��ʼ�������ݳɹ���ȡ�󣬻�ص�listener.onRead��
		/// ע��˷�����������ǰ�̡߳�
		void startRead();

		/// ������״̬�Ƿ���׼����
		/// @return true=׼����
		bool isStable();
	};
}
#endif