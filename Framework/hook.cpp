#include "hook.h"
#include <dbghelp.h>
#include "CommonDefine.h"

#pragma comment(lib, "dbghelp.lib")

void hook::hook_api(HINSTANCE dll_inst, char *api_name, FARPROC call_proc)
{
	api_proc = GetProcAddress(dll_inst, api_name);
	hook_code(api_proc, call_proc);
}

//HOOK code
void hook::hook_code(FARPROC proc, FARPROC call_proc)
{
	DWORD old_protect(0);
	api_proc = proc;
	BYTE * buff = &hook_hex[0];
	*(BYTE *)(buff) = 0xE9;
	buff++;
	*(DWORD *)(buff) = (DWORD)call_proc - (DWORD)api_proc - 5;
	VirtualProtect(api_proc, 8, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy(src_hex, api_proc, 5);
	memcpy(api_proc, hook_hex, 5);
}

void hook::set()		//钩
{
	memcpy(api_proc, hook_hex, 5);
}

void hook::revert()	//还原
{
	memcpy(api_proc, src_hex, 5);
}

//*****************************************************************************
//	函数说明：	IAT Hook
//	函数原型：	int replace_iat(xxx)
//  参数：      hProcess进程句柄、pDllName为Dll名、pApiName需要修复IAT函数名、CallCode修改地址、HookCode调用地址
//  返回值：    0
//  用法：		
//*****************************************************************************
int hook::replace_iat(HANDLE hProcess, const char *pDllName, const char *pApiName, DWORD &CallCode, DWORD HookCode)
{
	//HANDLE hProcess = GetModuleHandle(0);
	DWORD dwSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImageImport = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hProcess, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwSize);
	if (!pImageImport) return 1;
	PIMAGE_IMPORT_BY_NAME pImageImportByName = 0;
	PIMAGE_THUNK_DATA pImageThunkOriginal = 0;
	PIMAGE_THUNK_DATA pImageThunkReal = 0;

	while (true)
	{
		if (!pImageImport->Name) return 2;
		else if (!strcmpi((char*)((PBYTE)hProcess + pImageImport->Name), pDllName)) break;
		++pImageImport;
	}

	pImageThunkOriginal = (PIMAGE_THUNK_DATA)((PBYTE)hProcess + pImageImport->OriginalFirstThunk);
	pImageThunkReal = (PIMAGE_THUNK_DATA)((PBYTE)hProcess + pImageImport->FirstThunk);
	while (pImageThunkOriginal->u1.Function)
	{
		if ((pImageThunkOriginal->u1.Ordinal & IMAGE_ORDINAL_FLAG) != IMAGE_ORDINAL_FLAG)
		{
			pImageImportByName = (PIMAGE_IMPORT_BY_NAME)((PBYTE)hProcess + pImageThunkOriginal->u1.AddressOfData);
			if (!strcmpi(pApiName, (char*)pImageImportByName->Name))
			{
				MEMORY_BASIC_INFORMATION mbi_thunk;
				VirtualQuery(pImageThunkReal, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
				VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect);
				if (HookCode)
				{
					CallCode = pImageThunkReal->u1.Function;
					pImageThunkReal->u1.Function = HookCode;
				}
				else {
					pImageThunkReal->u1.Function = CallCode;
				}
				DWORD dwOldProtect;
				VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect);
				break;
			}
		}
		++pImageThunkOriginal;
		++pImageThunkReal;
	}
	return 0;
}
