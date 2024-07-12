#include "netimmerse.h"

void NiAVObject::SetLocalTranslate(const NiPoint3& pos) {
	m_kLocal.translate.x = pos.x;
	m_kLocal.translate.y = pos.y;
	m_kLocal.translate.z = pos.z;
}