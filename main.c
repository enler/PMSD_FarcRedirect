#include "3ds/types.h"

static const u16 title01path[] = L"patch:/title01.img";
static const u16 title02path[] = L"patch:/title02.img";

static const u16 testPath[] = L"image_2d.bin";

int wcscmp(const u16 * s_in0, const u16 * s_in1);
void * memcpy(void* dst, void* src, size_t len);

int readFileFromBuffer(const u16 * path, void * buffer, s64 pos, int size) {
	int (*UserFileSystem$$TryOpenFile)(void**, const u16*, u32) = (void*)0x288E60;
	int (*UserFileSystem$$TryReadFile)(int *, void*, s64, void*, u32) = (void*)0x1AF7A0;
	int (*UserFileSystem$$CloseFile)(void *) = (void*)0x240524;
	void * fileObj = 0;
	if (!UserFileSystem$$TryOpenFile(&fileObj, path, 1)) {
		int rSize = 0;
		UserFileSystem$$TryReadFile(&rSize, fileObj, pos, buffer, size);
		UserFileSystem$$CloseFile(fileObj);
		return rSize;
	}
	return 0;
}

int hook_farcLoadAll2(void* this, const u16 * path, void* buffer, u32 size);

int orig_farcOpen(void*, const u16*, int, int);

int hook_farcOpen(void* this, const u16 * path, int a2, int a3) {
	if (wcscmp(path, testPath) != 0) {
		return orig_farcOpen(this, path, a2, a3);
	}
	static void * vtable[7];
	memcpy(vtable, *(void**)this, sizeof(vtable));
	*(void**)this = vtable;
	vtable[5] = hook_farcLoadAll2;
	return orig_farcOpen(this, path, a2, a3);
}

int orig_farcLoadAll(void* this, const u16 * path, void* buffer, u32 size);

int hook_farcLoadAll2(void* this, const u16 * path, void* buffer, u32 size) {
	if (wcscmp(path, title01path + 7) == 0) {
		return readFileFromBuffer(title01path, buffer ,0, size);
	}
	else if (wcscmp(path, title02path + 7) == 0) {
		return readFileFromBuffer(title02path, buffer ,0, size);
	}
	return orig_farcLoadAll(this, path, buffer, size);
}

int hook_farcLoadAll(void* this, u16 * path, void* buffer, u32 size) {
	int (*hook_farcLoadAll2_proxy)(void*, u16*, void*, u32) = *(*(void***)this + 5);
	if (hook_farcLoadAll2_proxy) return hook_farcLoadAll2_proxy(this, path, buffer, size);
	return orig_farcLoadAll(this, path, buffer, size);
}
