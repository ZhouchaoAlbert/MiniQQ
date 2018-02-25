#include "StdAfx.h"
#include "Server.h"

namespace fastipc{
	Server::Server(){
		evtWrited = NULL;
		evtReaded = NULL;
		mapFile = NULL;
		memBuf = NULL;
	};

	Server::~Server(void)	{
		close();
	};

	void Server::close(void){
		listener = NULL;
		if (evtWrited) {// �ر��¼����
			HANDLE handle = evtWrited;
			evtWrited = NULL;
			SetEvent(handle);
			CloseHandle(handle);
		}
		if (evtReaded) {// �ر��¼����
			HANDLE handle = evtReaded;
			evtReaded = NULL;
			CloseHandle(handle);
		}
		if (memBuf) {// ȡ���ڴ�ӳ��
			MemBuff *pBuff = memBuf;
			memBuf = NULL;
			UnmapViewOfFile(pBuff);
		}
		if (mapFile) {// �ر�ӳ���ļ�
			HANDLE handle = mapFile;
			mapFile = NULL;
			CloseHandle(handle);
		}
	};
	void Server::setListener(ReadListener* listener){
		this->listener = listener;
	};
	bool Server::isStable(){
		return memBuf != NULL;
	};
	int Server::create(const std::wstring serverName, DWORD blockSize){
		// ���������¼��ֱ�����֪ͨ�ɶ�����д
		evtWrited = CreateEvent(NULL, FALSE, FALSE, LPTSTR(genWritedEventName(serverName).c_str()));
		if (evtWrited == NULL || evtWrited == INVALID_HANDLE_VALUE)  return ERR_EventOpen_W;
		evtReaded = CreateEvent(NULL, FALSE, FALSE, LPTSTR(genReadedEventName(serverName).c_str()));
		if (evtReaded == NULL || evtReaded == INVALID_HANDLE_VALUE) return ERR_EventOpen_R;

		// �����ڴ�ӳ���ļ�
		DWORD size = sizeof(MemBuff)+sizeof(char)*blockSize;// ��̬�ӳ�blockSize���ֽ�
		mapFile = CreateFileMapping(INVALID_HANDLE_VALUE,	// ����һ���������ļ��޹ص�ӳ���ļ�
			NULL,											// ��ȫ����
			PAGE_READWRITE,									// �򿪷�ʽ
			0,												// �ļ�ӳ�����󳤶ȵĸ�32λ��
			size,											// �ļ�ӳ�����󳤶ȵĵ�32λ��
			LPTSTR(genMappingFileName(serverName).c_str()));// ӳ���ļ�������
		if (mapFile == NULL || mapFile == INVALID_HANDLE_VALUE)  return ERR_MappingCreate;
		// ӳ���ļ����ڴ�
		memBuf = (MemBuff*)MapViewOfFile(mapFile,			// ӳ���ļ����
			FILE_MAP_ALL_ACCESS,							// ��дȨ��
			0, 0,											// ӳ����ʼƫ�Ƶĸߡ���32λ
			size);											// ָ��ӳ���ļ����ֽ���
		if (memBuf == NULL)return ERR_MappingMap;			// ӳ���ļ�ʧ��
		ZeroMemory(memBuf, size);							// ��ջ�����
		return 0;
	};

	void Server::startRead(){
		while (evtWrited && (WaitForSingleObject(evtWrited, INFINITE) == WAIT_OBJECT_0)){// �ȴ�д���¼�
			if (!evtWrited)break;
			// ��ȡΪ���̶߳�ȡ�����Բ���Ҫ������state��ǰ�����״̬λ�ļ��
			//if (memBuf->state != MEM_CAN_READ) continue;  // ��ǰ���������ǿɶ�״̬����������ʹ�ã������ȴ�
			//if (memBuf->state != MEM_IS_BUSY)continue;  // ������ú��ǿ���״̬����ô�����Ǳ��������̰����ݶ����ˣ���ʱ�����ȴ� 
			InterlockedCompareExchange(&memBuf->state, MEM_IS_BUSY, MEM_CAN_READ);// ͨ��ԭ�Ӳ��������ù�������״̬Ϊ��״̬
			MemBlock * rtn = NULL;
			try{
				rtn = new MemBlock();
				rtn->dataLen = memBuf->dataLen;
				rtn->msgType = memBuf->msgType;
				rtn->data = (char *)malloc(memBuf->dataLen);
				memcpy(rtn->data, memBuf->data, rtn->dataLen);
				if (memBuf->msgType > MSG_TYPE_NORMAL){
					ZeroMemory(rtn->packId, PACK_ID_LEN);
					memcpy(rtn->packId, memBuf->packId, PACK_ID_LEN);
				}
				listener->onRead(rtn);
				delete rtn;// ������
			} catch (...){
				delete rtn;// ������
			}
			// ���߳�����£����ܻ����server.close�����ú󣬻����߳���onRead����������Ӹ���ָ����
			if (memBuf){
				InterlockedExchange(&memBuf->state, MEM_CAN_WRITE);// ���ݶ�ȡ֮������Ϊ��д
			}else{
				close();// ������Ƿ���δ�رյ���Դ
			}
			if (evtReaded){
				SetEvent(evtReaded);
			}else{
				close();// ������Ƿ���δ�رյ���Դ
			}
		}
	};
}