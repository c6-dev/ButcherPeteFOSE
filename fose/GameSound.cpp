#include "GameSound.h"


bool BSSoundHandle::IsPlaying() const
{
	return ThisCall<bool>(0xBD0160, this);
}

bool BSSoundHandle::Stop()
{
	return ThisCall<bool>(0xBD0140, this);
}


bool BSSoundHandle::Release()
{
	return ThisCall<bool>(0xBD03B0, this);
}
