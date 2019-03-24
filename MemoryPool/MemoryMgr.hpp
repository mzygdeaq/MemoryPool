#ifndef _MemoryMgr_hpp_
#define _MemoryMgr_hpp_

#include <stdlib.h>
#include <assert.h>

class MemoryAlloc;

// 内存块 最小单元
class MemoryBlock {
public:
	// 所属的内存池
	MemoryAlloc* pAlloc;
	// 下一块位置
	MemoryBlock* pNext;
	// 内存块编号
	int nID;
	// 引用次数
	int nRef;
	// 是否受到池管理
	bool bPool;
private:
	char padding1;
	char padding2;
	char padding3;
};

// int ss = sizeof(MemoryBlock);

// 内存池
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

	// 分配内存
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
		// 指针+1是加整个结构的大小 所以必须转成char *
		return ((char*)pReturn + sizeof(MemoryBlock));
	}
	
	// 释放内存
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
	// 初始化
	void initMemory() {
		assert(nullptr == _pBuf);
		if (!_pBuf)
			return;
		size_t bufSize = _nSize * _nBlockSize;
		_pBuf = (char*)malloc(bufSize);

		// 初始化所有的内存单元
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
	// 内存池地址
	char* _pBuf;
	// 头部内存单元
	MemoryBlock* _pHeader;
	// 内存单元的大小
	size_t _nSize;
	// 内存单元的数量
	size_t _nBlockSize;

};

// 内存管理器
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
