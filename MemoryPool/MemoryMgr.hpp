#ifndef _MemoryMgr_hpp_
#define _MemoryMgr_hpp_

#include <stdlib.h>
#include <assert.h>

class MemoryAlloc;

// �ڴ�� ��С��Ԫ
class MemoryBlock {
public:
	// �������ڴ��
	MemoryAlloc* pAlloc;
	// ��һ��λ��
	MemoryBlock* pNext;
	// �ڴ����
	int nID;
	// ���ô���
	int nRef;
	// �Ƿ��ܵ��ع���
	bool bPool;
private:
	char padding1;
	char padding2;
	char padding3;
};

// int ss = sizeof(MemoryBlock);

// �ڴ��
class MemoryAlloc {
public:
	MemoryAlloc() {
		_pBuf = nullptr;
		_pHeader = nullptr;
		_nSize = 0;
		_nBlockSize = 0;
	}
	~MemoryAlloc()
	{
	}

	// �����ڴ�
	void* allocMemory(size_t size)
	{
		if (!_pBuf)
		{
			initMemory();
		}
		MemoryBlock* pReturn = nullptr;
		if (nullptr == _pHeader)
		{
			pReturn = (MemoryBlock*)malloc(size + sizeof(MemoryBlock));
			pReturn->bPool = false;
			pReturn->nID = -1;
			pReturn->nRef = 0;
			pReturn->pAlloc = this;
			pReturn->pNext = nullptr;
		}
		else {
			pReturn = _pHeader;
			assert(0 == pReturn->nRef);
			pReturn->nRef = 1;
			_pHeader = _pHeader->pNext;
		}
		// ָ��+1�Ǽ������ṹ�Ĵ�С ���Ա���ת��char *
		return ((char*)pReturn + sizeof(MemoryBlock));
	}
	
	// �ͷ��ڴ�
	void freeMemory(void* pMem)
	{
		// TODO
		if (pMem == nullptr)
			return;
		MemoryBlock* pBlock = (MemoryBlock *)((char*)pMem - sizeof(MemoryBlock));
		assert(1 == pBlock->nRef);
		if (--pBlock->nRef != 0)
		{
			return;
		}
		if (pBlock->bPool)
		{
			pBlock->pNext = _pHeader;
			_pHeader = pBlock;
		}
		else {
			free(pBlock);
		}
	}
	// ��ʼ��
	void initMemory() {
		assert(nullptr == _pBuf);
		if (!_pBuf)
			return;
		size_t bufSize = _nSize * _nBlockSize;
		_pBuf = (char*)malloc(bufSize);

		// ��ʼ�����е��ڴ浥Ԫ
		_pHeader = (MemoryBlock*)_pBuf;
		_pHeader->bPool = true;
		_pHeader->nID = 0;
		_pHeader->nRef = 0;
		_pHeader->pAlloc = this;
		_pHeader->pNext = nullptr;

		MemoryBlock* PLast = _pHeader;
		for (size_t i = 1; i < _nBlockSize; i++)
		{
			MemoryBlock* pTemp = (MemoryBlock*)(_pBuf + (i * _nSize));
			pTemp->bPool = true;
			pTemp->nID = 0;
			pTemp->nRef = 0;
			pTemp->pAlloc = this;
			pTemp->pNext = nullptr;

			PLast->pNext = pTemp;
			PLast = pTemp;
		}
	}
private:
	// �ڴ�ص�ַ
	char* _pBuf;
	// ͷ���ڴ浥Ԫ
	MemoryBlock* _pHeader;
	// �ڴ浥Ԫ�Ĵ�С
	size_t _nSize;
	// �ڴ浥Ԫ������
	size_t _nBlockSize;

};

// �ڴ������
class MemoryMgr {

public:
	static MemoryMgr& Instance() {
		static MemoryMgr mgr;
		return mgr;
	}
	void* allocMem(size_t size)
	{
		return malloc(size);
	}
	void freeMem(void* p)
	{
		return free(p);
	}
private:
	MemoryMgr()
	{

	}
	~MemoryMgr()
	{

	}
};

#endif // !_MemoryMgr_hpp_
