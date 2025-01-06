#include "netimmerse.h"
#include "GameForms.h"
void NiAVObject::SetLocalTranslate(const NiPoint3& pos) {
	m_kLocal.translate.x = pos.x;
	m_kLocal.translate.y = pos.y;
	m_kLocal.translate.z = pos.z;
}

TESObjectREFR* NiAVObject::GetParentRef() const
{
	if (IS_TYPE(this, BSFadeNode)) {
		TESObjectREFR* obj = ((BSFadeNode*)this)->linkedObj;
		if (obj) return obj;
	}
	if (this->m_parent) {
		return this->m_parent->GetParentRef();
	}
	return nullptr;
}

bool NiMatrix3::operator==(const NiMatrix3& mat) const {
	return
		(m_pEntry[0][0] == mat.m_pEntry[0][0]) &&
		(m_pEntry[0][1] == mat.m_pEntry[0][1]) &&
		(m_pEntry[0][2] == mat.m_pEntry[0][2]) &&
		(m_pEntry[1][0] == mat.m_pEntry[1][0]) &&
		(m_pEntry[1][1] == mat.m_pEntry[1][1]) &&
		(m_pEntry[1][2] == mat.m_pEntry[1][2]) &&
		(m_pEntry[2][0] == mat.m_pEntry[2][0]) &&
		(m_pEntry[2][1] == mat.m_pEntry[2][1]) &&
		(m_pEntry[2][2] == mat.m_pEntry[2][2]);

}

NiMatrix3 NiMatrix3::operator+ (const NiMatrix3& mat) const {

	NiMatrix3 result = *this;
	result.m_pEntry[0][0] += mat.m_pEntry[0][0];
	result.m_pEntry[0][1] += mat.m_pEntry[0][1];
	result.m_pEntry[0][2] += mat.m_pEntry[0][2];
	result.m_pEntry[1][0] += mat.m_pEntry[1][0];
	result.m_pEntry[1][1] += mat.m_pEntry[1][1];
	result.m_pEntry[1][2] += mat.m_pEntry[1][2];
	result.m_pEntry[2][0] += mat.m_pEntry[2][0];
	result.m_pEntry[2][1] += mat.m_pEntry[2][1];
	result.m_pEntry[2][2] += mat.m_pEntry[2][2];

	return result;
}

NiMatrix3 NiMatrix3::operator- (const NiMatrix3& mat) const {
	NiMatrix3 result = *this;

	result.m_pEntry[0][0] -= mat.m_pEntry[0][0];
	result.m_pEntry[0][1] -= mat.m_pEntry[0][1];
	result.m_pEntry[0][2] -= mat.m_pEntry[0][2];
	result.m_pEntry[1][0] -= mat.m_pEntry[1][0];
	result.m_pEntry[1][1] -= mat.m_pEntry[1][1];
	result.m_pEntry[1][2] -= mat.m_pEntry[1][2];
	result.m_pEntry[2][0] -= mat.m_pEntry[2][0];
	result.m_pEntry[2][1] -= mat.m_pEntry[2][1];
	result.m_pEntry[2][2] -= mat.m_pEntry[2][2];

	return result;
}

NiMatrix3 NiMatrix3::operator* (const NiMatrix3& mat) const {
	NiMatrix3 result;

	result.m_pEntry[0][0] =
		m_pEntry[0][0] * mat.m_pEntry[0][0] +
		m_pEntry[0][1] * mat.m_pEntry[1][0] +
		m_pEntry[0][2] * mat.m_pEntry[2][0];
	result.m_pEntry[1][0] =
		m_pEntry[1][0] * mat.m_pEntry[0][0] +
		m_pEntry[1][1] * mat.m_pEntry[1][0] +
		m_pEntry[1][2] * mat.m_pEntry[2][0];
	result.m_pEntry[2][0] =
		m_pEntry[2][0] * mat.m_pEntry[0][0] +
		m_pEntry[2][1] * mat.m_pEntry[1][0] +
		m_pEntry[2][2] * mat.m_pEntry[2][0];
	result.m_pEntry[0][1] =
		m_pEntry[0][0] * mat.m_pEntry[0][1] +
		m_pEntry[0][1] * mat.m_pEntry[1][1] +
		m_pEntry[0][2] * mat.m_pEntry[2][1];
	result.m_pEntry[1][1] =
		m_pEntry[1][0] * mat.m_pEntry[0][1] +
		m_pEntry[1][1] * mat.m_pEntry[1][1] +
		m_pEntry[1][2] * mat.m_pEntry[2][1];
	result.m_pEntry[2][1] =
		m_pEntry[2][0] * mat.m_pEntry[0][1] +
		m_pEntry[2][1] * mat.m_pEntry[1][1] +
		m_pEntry[2][2] * mat.m_pEntry[2][1];
	result.m_pEntry[0][2] =
		m_pEntry[0][0] * mat.m_pEntry[0][2] +
		m_pEntry[0][1] * mat.m_pEntry[1][2] +
		m_pEntry[0][2] * mat.m_pEntry[2][2];
	result.m_pEntry[1][2] =
		m_pEntry[1][0] * mat.m_pEntry[0][2] +
		m_pEntry[1][1] * mat.m_pEntry[1][2] +
		m_pEntry[1][2] * mat.m_pEntry[2][2];
	result.m_pEntry[2][2] =
		m_pEntry[2][0] * mat.m_pEntry[0][2] +
		m_pEntry[2][1] * mat.m_pEntry[1][2] +
		m_pEntry[2][2] * mat.m_pEntry[2][2];

	return result;
}

NiMatrix3 NiMatrix3::operator* (float fScalar) const {
	NiMatrix3 result = *this;
	result.m_pEntry[0][0] *= fScalar;
	result.m_pEntry[0][1] *= fScalar;
	result.m_pEntry[0][2] *= fScalar;
	result.m_pEntry[1][0] *= fScalar;
	result.m_pEntry[1][1] *= fScalar;
	result.m_pEntry[1][2] *= fScalar;
	result.m_pEntry[2][0] *= fScalar;
	result.m_pEntry[2][1] *= fScalar;
	result.m_pEntry[2][2] *= fScalar;

	return result;
}

NiPoint3 NiMatrix3::operator*(const NiPoint3& pt) const {

	return NiPoint3(
		m_pEntry[0][0] * pt.x + m_pEntry[0][1] * pt.y + m_pEntry[0][2] * pt.z,
		m_pEntry[1][0] * pt.x + m_pEntry[1][1] * pt.y + m_pEntry[1][2] * pt.z,
		m_pEntry[2][0] * pt.x + m_pEntry[2][1] * pt.y + m_pEntry[2][2] * pt.z);

}

NiPoint3 operator*(const NiPoint3& pt, const NiMatrix3& mat) {

	return NiPoint3(
		mat.m_pEntry[0][0] * pt.x + mat.m_pEntry[1][0] * pt.y + mat.m_pEntry[2][0] * pt.z,
		mat.m_pEntry[0][1] * pt.x + mat.m_pEntry[1][1] * pt.y + mat.m_pEntry[2][1] * pt.z,
		mat.m_pEntry[0][2] * pt.x + mat.m_pEntry[1][2] * pt.y + mat.m_pEntry[2][2] * pt.z);

}

NiMatrix3 NiMatrix3::operator/ (float fScalar) const {
	NiMatrix3 result = *this;
	result.m_pEntry[0][0] /= fScalar;
	result.m_pEntry[0][1] /= fScalar;
	result.m_pEntry[0][2] /= fScalar;
	result.m_pEntry[1][0] /= fScalar;
	result.m_pEntry[1][1] /= fScalar;
	result.m_pEntry[1][2] /= fScalar;
	result.m_pEntry[2][0] /= fScalar;
	result.m_pEntry[2][1] /= fScalar;
	result.m_pEntry[2][2] /= fScalar;

	return result;
}

void NiMatrix3::MakeXRotation(float fAngle) {

	float sn = std::sin(fAngle);
	float cs = std::cos(fAngle);

	m_pEntry[0][0] = 1.0f;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = cs;
	m_pEntry[1][2] = sn;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = -sn;
	m_pEntry[2][2] = cs;

}

void NiMatrix3::MakeYRotation(float fAngle) {

	float sn = std::sin(fAngle);
	float cs = std::cos(fAngle);

	m_pEntry[0][0] = cs;
	m_pEntry[0][1] = 0.0f;
	m_pEntry[0][2] = -sn;
	m_pEntry[1][0] = 0.0f;
	m_pEntry[1][1] = 1.0f;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = sn;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = cs;

}

void NiMatrix3::MakeZRotation(float fAngle) {

	float sn = std::sin(fAngle);
	float cs = std::cos(fAngle);

	m_pEntry[0][0] = cs;
	m_pEntry[0][1] = sn;
	m_pEntry[0][2] = 0.0f;
	m_pEntry[1][0] = -sn;
	m_pEntry[1][1] = cs;
	m_pEntry[1][2] = 0.0f;
	m_pEntry[2][0] = 0.0f;
	m_pEntry[2][1] = 0.0f;
	m_pEntry[2][2] = 1.0f;

}

void NiMatrix3::MakeRotation(float fAngle, float x, float y, float z) {

	float sn = std::sin(fAngle);
	float cs = std::cos(fAngle);

	float omcs = 1.0f - cs;
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xym = x * y * omcs;
	float xzm = x * z * omcs;
	float yzm = y * z * omcs;
	float xsin = x * sn;
	float ysin = y * sn;
	float zsin = z * sn;

	m_pEntry[0][0] = x2 * omcs + cs;
	m_pEntry[0][1] = xym + zsin;
	m_pEntry[0][2] = xzm - ysin;
	m_pEntry[1][0] = xym - zsin;
	m_pEntry[1][1] = y2 * omcs + cs;
	m_pEntry[1][2] = yzm + xsin;
	m_pEntry[2][0] = xzm + ysin;
	m_pEntry[2][1] = yzm - xsin;
	m_pEntry[2][2] = z2 * omcs + cs;

}

void NiMatrix3::MakeRotation(float angle, const NiPoint3& axis) {

	MakeRotation(angle, axis.x, axis.y, axis.z);

} 

void NiMatrix3::FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle) {
	NiMatrix3 kXRot, kYRot, kZRot;
	kXRot.MakeXRotation(fXAngle);
	kYRot.MakeYRotation(fYAngle);
	kZRot.MakeZRotation(fZAngle);
	*this = kXRot * (kYRot * kZRot);
}

const float NI_INFINITY = FLT_MAX;
const float NI_PI = 3.1415927410125732f;
const float NI_HALF_PI = 0.5f * NI_PI;
const float NI_TWO_PI = 2.0f * NI_PI;

bool NiMatrix3::ToEulerAnglesXYZ(float& arfXAngle, float& arfYAngle, float& arfZAngle) const {
	arfYAngle = -asin(m_pEntry[0][2]);
	if (arfYAngle < NI_HALF_PI) {
		if (arfYAngle > -NI_HALF_PI) {
			arfXAngle = -atan2(-m_pEntry[1][2], m_pEntry[2][2]);
			arfZAngle = -atan2(-m_pEntry[0][1], m_pEntry[0][0]);
			return true;
		}
		else {
			// WARNING.  Not a unique solution.
			float fRmY = atan2(m_pEntry[1][0], m_pEntry[1][1]);
			arfZAngle = 0.0f;  // any angle works
			arfXAngle = fRmY - arfZAngle;
			return false;
		}
	}
	else {
		// WARNING.  Not a unique solution.
		float fRpY = atan2(m_pEntry[1][0], m_pEntry[1][1]);
		arfZAngle = 0.0f;  // any angle works
		arfXAngle = arfZAngle - fRpY;
		return false;
	}
}

NiMatrix3 NiMatrix3::TransposeTimes(const NiMatrix3& mat) const {
	NiMatrix3 prd;
	prd.m_pEntry[0][0] = m_pEntry[0][0] * mat.m_pEntry[0][0] + m_pEntry[0][1] * mat.m_pEntry[1][0] + m_pEntry[0][2] * mat.m_pEntry[2][0];
	prd.m_pEntry[1][0] = m_pEntry[1][0] * mat.m_pEntry[0][0] + m_pEntry[1][1] * mat.m_pEntry[1][0] + m_pEntry[1][2] * mat.m_pEntry[2][0];
	prd.m_pEntry[2][0] = m_pEntry[2][0] * mat.m_pEntry[0][0] + m_pEntry[2][1] * mat.m_pEntry[1][0] + m_pEntry[2][2] * mat.m_pEntry[2][0];
	prd.m_pEntry[0][1] = m_pEntry[0][0] * mat.m_pEntry[0][1] + m_pEntry[0][1] * mat.m_pEntry[1][1] + m_pEntry[0][2] * mat.m_pEntry[2][1];
	prd.m_pEntry[1][1] = m_pEntry[1][0] * mat.m_pEntry[0][1] + m_pEntry[1][1] * mat.m_pEntry[1][1] + m_pEntry[1][2] * mat.m_pEntry[2][1];
	prd.m_pEntry[2][1] = m_pEntry[2][0] * mat.m_pEntry[0][1] + m_pEntry[2][1] * mat.m_pEntry[1][1] + m_pEntry[2][2] * mat.m_pEntry[2][1];
	prd.m_pEntry[0][2] = m_pEntry[0][0] * mat.m_pEntry[0][2] + m_pEntry[0][1] * mat.m_pEntry[1][2] + m_pEntry[0][2] * mat.m_pEntry[2][2];
	prd.m_pEntry[1][2] = m_pEntry[1][0] * mat.m_pEntry[0][2] + m_pEntry[1][1] * mat.m_pEntry[1][2] + m_pEntry[1][2] * mat.m_pEntry[2][2];
	prd.m_pEntry[2][2] = m_pEntry[2][0] * mat.m_pEntry[0][2] + m_pEntry[2][1] * mat.m_pEntry[1][2] + m_pEntry[2][2] * mat.m_pEntry[2][2];

	return prd;
}

bool NiMatrix3::Inverse(NiMatrix3& inv) const {

	inv.m_pEntry[0][0] = m_pEntry[1][1] * m_pEntry[2][2] - m_pEntry[1][2] * m_pEntry[2][1];
	inv.m_pEntry[0][1] = m_pEntry[0][2] * m_pEntry[2][1] - m_pEntry[0][1] * m_pEntry[2][2];
	inv.m_pEntry[0][2] = m_pEntry[0][1] * m_pEntry[1][2] - m_pEntry[0][2] * m_pEntry[1][1];
	inv.m_pEntry[1][0] = m_pEntry[1][2] * m_pEntry[2][0] - m_pEntry[1][0] * m_pEntry[2][2];
	inv.m_pEntry[1][1] = m_pEntry[0][0] * m_pEntry[2][2] - m_pEntry[0][2] * m_pEntry[2][0];
	inv.m_pEntry[1][2] = m_pEntry[0][2] * m_pEntry[1][0] - m_pEntry[0][0] * m_pEntry[1][2];
	inv.m_pEntry[2][0] = m_pEntry[1][0] * m_pEntry[2][1] - m_pEntry[1][1] * m_pEntry[2][0];
	inv.m_pEntry[2][1] = m_pEntry[0][1] * m_pEntry[2][0] - m_pEntry[0][0] * m_pEntry[2][1];
	inv.m_pEntry[2][2] = m_pEntry[0][0] * m_pEntry[1][1] - m_pEntry[0][1] * m_pEntry[1][0];

	float fDet = m_pEntry[0][0] * inv.m_pEntry[0][0] +
		m_pEntry[0][1] * inv.m_pEntry[1][0] +
		m_pEntry[0][2] * inv.m_pEntry[2][0];

	if (std::abs(fDet) <= 1e-06f)
		return false;

	float fInvdet = 1.0f / fDet;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			inv.m_pEntry[row][col] *= fInvdet;
		}
	}

	return true;

}

 NiAVObject* NiNode::GetBlock(const char* blockName)
{
	 NiFixedString str(blockName);
	 return this->GetObjectByName(str);

}

 NiAVObject* NiNode::GetBip01()
 {
	 if (this->m_children.Length() > 0) {
		 for (int i = 0; i < this->m_children.Length(); i++) {
			 NiAVObject* obj = this->m_children[i];
			 if (obj && obj->m_blockName != nullptr && strcmp(obj->m_blockName, "Bip01") == 0) {
				 return obj;
			 }
		 }
	 }
	 return nullptr;
 }

 char* NiGlobalStringTable::AddString(const char* string) {
	 return CdeclCall<char*>(0x8242A0, string);
 }

 void NiGlobalStringTable::IncRefCount(char* string) {
	 if (*string) InterlockedIncrement((LPLONG)(*string - 8));
 }

 void NiGlobalStringTable::DecRefCount(char* string) {
	 if (*string) InterlockedDecrement((LPLONG)(*string - 8));
 }

 NiFixedString::NiFixedString(const char* string) {
	 if (string)
		 m_kHandle = NiGlobalStringTable::AddString(string);
	 else
		 m_kHandle = nullptr;
 }

 NiFixedString::~NiFixedString() {
	 NiGlobalStringTable::DecRefCount(m_kHandle);
 }

 NiFixedString& NiFixedString::operator=(const char* string) {
	 if (m_kHandle != string) {
		 char* handle = m_kHandle;
		 m_kHandle = NiGlobalStringTable::AddString(string);
		 NiGlobalStringTable::DecRefCount(handle);
	 }
	 return *this;
 }

 NiFixedString& NiFixedString::operator=(NiFixedString& string) {
	 if (m_kHandle != string.m_kHandle) {
		 char* handle = string.m_kHandle;
		 NiGlobalStringTable::IncRefCount(handle);
		 NiGlobalStringTable::DecRefCount(m_kHandle);
		 m_kHandle = handle;
	 }
	 return *this;
 }

 bool NiFixedString::operator==(const NiFixedString& string) {
	 if (m_kHandle == string.m_kHandle)
		 return true;

	 return !strcmp(m_kHandle, string.m_kHandle);
 }

 bool NiFixedString::operator==(const char* string) {
	 if (m_kHandle == string)
		 return true;

	 return !strcmp(m_kHandle, string);
 }
