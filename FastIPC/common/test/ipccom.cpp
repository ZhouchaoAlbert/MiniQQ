/// ����fastipc��ͨ�ŵ���ش���
#include"stdafx.h"
#include "ipccom.h"
#include "../fastipc/Server.h"
#include "../fastipc/Client.h"
#include "../util/StrUtil.h"
#include <thread>
#include <string>
#include <sstream>

std::wstring serverName;	// ���������
fastipc::Server server;     // IPC�����
fastipc::Client client;		// IPC�ͻ���
std::stringstream blocks;	// ��һ�������ݱ��ֳɶ��ʱ������������ʱ����ÿ�������
DWORD blockSize = 10;		// һ�δ���ʱ�����ݴ�С
DWORD sendMsg(std::wstring msg){
	if (!client.isStable()){// ���client�Ƿ��Ѿ���ʼ��
		client.create(fastipc::genServerName(serverName), blockSize);// ��ʼ��client
	}
	std::string s = jw::w2s(msg);
	return client.write(LPSTR(s.c_str()), s.size());// ������Ϣ�������
}

class ServerReadListener :public fastipc::RebuildedBlockListener{
	void onRebuildedRead(fastipc::MemBlock* readed) override{
		int len = readed->dataLen;
		char * tmp;
		tmp = (char *)malloc(len + 1);
		memcpy(tmp, readed->data, len);
		tmp[len] = '\0';
		//���ܵ�����
		jw::s2w(tmp);
		delete tmp;
	}
	//void onRead(fastipc::MemBlock* readed) override{
	//	if (readed->dataLen > 0){
	//		int len = readed->dataLen;
	//		char * tmp;
	//		tmp = (char *)malloc(len + 1);
	//		memcpy(tmp, readed->data, len);
	//		tmp[len] = '\0';
	//		if (readed->msgType != MSG_TYPE_NORMAL){
	//			blocks << tmp;// FIXME����������֯���ݶ�ʱû�и���packId���ж��Ƿ���ͬһ�����ݣ�������ڶ���ͻ��˻�ͻ��˶��̷߳���ʱ��Ҫ�����������
	//			if (readed->msgType == MSG_TYPE_END){
	//				std::wstring ws = jw::s2w(blocks.str());
	//				ws.append(L" UID=");
	//				char * uid;
	//				uid = (char *)malloc(PACK_ID_LEN + 1);
	//				memcpy(uid, readed->packId, PACK_ID_LEN);
	//				uid[PACK_ID_LEN] = '\0';
	//				ws.append(jw::s2w(uid));
	//				appendReceied(ws);
	//				blocks.str("");
	//			}
	//		}
	//		else{
	//			appendReceied(jw::s2w(tmp));
	//		}
	//		delete tmp;
	//	}
	//}
};

// ����IPC�����
void startServer(){
	server.startRead();// ��ʼ�������ݣ��˷�����������ǰ�߳�
}

// ����˴����Ѵ���
void windowCreated(HWND hWnd){
	server.setListener(new ServerReadListener());// �������������Ա���յ������ݽ��д���
	std::thread newThread(startServer);			 // �ڶ����Ľ���������IPC������
	newThread.detach();							 // ������̣���������
}