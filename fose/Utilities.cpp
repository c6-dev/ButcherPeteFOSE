#include <string>
#include "Utilities.h"
#include <map>
#include "SafeWrite.h"

void DumpClass(void * theClassPtr, UInt32 nIntsToDump)
{
	_MESSAGE("DumpClass:");
	UInt32* basePtr = (UInt32*)theClassPtr;

	gLog.Indent();

	if (!theClassPtr) return;
	for (UInt32 ix = 0; ix < nIntsToDump; ix++ ) {
		UInt32* curPtr = basePtr+ix;
		const char* curPtrName = NULL;
		UInt32 otherPtr = 0;
		float otherFloat = 0.0;
		const char* otherPtrName = NULL;
		if (curPtr) {
			curPtrName = GetObjectClassName((void*)curPtr);

			__try
			{
				otherPtr = *curPtr;
				otherFloat = *(float*)(curPtr);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//
			}

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		_MESSAGE("%3d +%03X ptr: 0x%08X: %32s *ptr: 0x%08x | %f: %32s", ix, ix*4, curPtr, curPtrName, otherPtr, otherFloat, otherPtrName);
	}

	gLog.Outdent();
}

#pragma warning (push)
#pragma warning (disable : 4200)
struct RTTIType
{
	void	* typeInfo;
	UInt32	pad;
	char	name[0];
};

struct RTTILocator
{
	UInt32		sig, offset, cdOffset;
	RTTIType	* type;
};
#pragma warning (pop)

// use the RTTI information to return an object's class name
const char * GetObjectClassName(void * objBase)
{
	const char	* result = "<no rtti>";

	__try
	{
		void		** obj = (void **)objBase;
		RTTILocator	** vtbl = (RTTILocator **)obj[0];
		RTTILocator	* rtti = vtbl[-1];
		RTTIType	* type = rtti->type;

		// starts with ,?
		if((type->name[0] == '.') && (type->name[1] == '?'))
		{
			// is at most 100 chars long
			for(UInt32 i = 0; i < 100; i++)
			{
				if(type->name[i] == 0)
				{
					// remove the .?AV
					result = type->name + 4;
					break;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// return the default
	}

	return result;
}

/******************
 Tokenizer methods
*******************/

Tokenizer::Tokenizer(const char* src, const char* delims)
	: m_offset(0), m_delims(delims), m_data(src)
{
	//
}

Tokenizer::~Tokenizer()
{
	//
}

UInt32 Tokenizer::NextToken(std::string& outStr)
{
	if (m_offset == m_data.length())
		return -1;

	size_t start = m_data.find_first_not_of(m_delims, m_offset);
	if (start != -1)
	{
		size_t end = m_data.find_first_of(m_delims, start);
		if (end == -1)
			end = m_data.length();

		m_offset = end;
		outStr = m_data.substr(start, end - start);
		return start;
	}

	return -1;
}

UInt32 Tokenizer::PrevToken(std::string& outStr)
{
	if (m_offset == 0)
		return -1;

	size_t searchStart = m_data.find_last_of(m_delims, m_offset - 1);
	if (searchStart == -1)
		return -1;

	size_t end = m_data.find_last_not_of(m_delims, searchStart);
	if (end == -1)
		return -1;

	size_t start = m_data.find_last_of(m_delims, end);	// okay if start == -1 here

	m_offset = end + 1;
	outStr = m_data.substr(start + 1, end - start);
	return start + 1;
}

/*******************
  Temp hook routines
********************/

struct SavedInstruction {
	UInt8	data[5];
};

static std::map<UInt32, SavedInstruction> s_tempHookSavedData;

void CreateTempHook(UInt32 hookAddr, UInt32 jmpAddr)
{
	SavedInstruction instr;
	memcpy(instr.data, (void*)hookAddr, 5);
	s_tempHookSavedData[hookAddr] = instr;
	WriteRelJump(hookAddr, jmpAddr);
}

bool DeleteTempHook(UInt32 hookAddr)
{
	if (s_tempHookSavedData.find(hookAddr) == s_tempHookSavedData.end())
		return false;
	
	SavedInstruction instr = s_tempHookSavedData[hookAddr];
	SafeWrite8(hookAddr, (UInt32)instr.data);
	SafeWrite32(hookAddr + 1, (UInt32)(instr.data + 1));
	s_tempHookSavedData.erase(hookAddr);
	return true;
}

const std::string & GetFalloutDirectory(void)
{
	static std::string s_falloutDirectory;

	if(s_falloutDirectory.empty())
	{
		// can't determine how many bytes we'll need, hope it's not more than MAX_PATH
		char	falloutPathBuf[MAX_PATH];
		UInt32	falloutPathLength = GetModuleFileName(GetModuleHandle(NULL), falloutPathBuf, sizeof(falloutPathBuf));

		if(falloutPathLength && (falloutPathLength < sizeof(falloutPathBuf)))
		{
			std::string	falloutPath(falloutPathBuf, falloutPathLength);

			// truncate at last slash
			std::string::size_type	lastSlash = falloutPath.rfind('\\');
			if(lastSlash != std::string::npos)	// if we don't find a slash something is VERY WRONG
			{
				s_falloutDirectory = falloutPath.substr(0, lastSlash + 1);

				_DMESSAGE("fallout root = %s", s_falloutDirectory.c_str());
			}
			else
			{
				_WARNING("no slash in fallout path? (%s)", falloutPath.c_str());
			}
		}
		else
		{
			_WARNING("couldn't find fallout path (len = %d, err = %08X)", falloutPathLength, GetLastError());
		}
	}

	return s_falloutDirectory;
}

static const std::string & GetFOSEConfigPath(void)
{
	static std::string s_configPath;

	if(s_configPath.empty())
	{
		std::string	falloutPath = GetFalloutDirectory();
		if(!falloutPath.empty())
		{
			s_configPath = falloutPath + "Data\\FOSE\\fose_config.ini";

			_MESSAGE("config path = %s", s_configPath.c_str());
		}
	}

	return s_configPath;
}

std::string GetFOSEConfigOption(const char * section, const char * key)
{
	std::string	result;

	const std::string & configPath = GetFOSEConfigPath();
	if(!configPath.empty())
	{
		char	resultBuf[256];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileString(section, key, NULL, resultBuf, 255, configPath.c_str());

		result = resultBuf;
	}

	return result;
}

bool GetFOSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut)
{
	std::string	data = GetFOSEConfigOption(section, key);
	if(data.empty())
		return false;

	return (sscanf(data.c_str(), "%lu", dataOut) == 1);
}


__declspec(naked) char* __fastcall GetNextToken(char* str, char delim)
{
	__asm
	{
		mov		dh, dl
		movd	xmm7, edx
		pshuflw	xmm7, xmm7, 0
		unpcklpd	xmm7, xmm7
		ALIGN 16
		lookup:
		movups	xmm6, [ecx]
			add		ecx, 0x10
			xorps	xmm5, xmm5
			pcmpeqb	xmm5, xmm6
			pcmpeqb	xmm6, xmm7
			por		xmm5, xmm6
			pmovmskb	eax, xmm5
			bsf		eax, eax
			jz		lookup
			lea		eax, [eax + ecx - 0x10]
			ALIGN 16
			nextNon:
		cmp[eax], dl
			jnz		done
			mov[eax], 0
			inc		eax
			jmp		nextNon
			done :
		retn
	}
}