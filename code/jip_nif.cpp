#include "jip_nif.h"
#include "GameObjects.h"
#include "CommandTable.h"
#include "GameInterface.h"

// --------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------

static constexpr float kDegToRad = 0.01745329238f; // pi / 180
static constexpr float kRadToDeg = 57.29578018f; // 180 / pi

// --------------------------------------------------------------------------
// Helpers
// --------------------------------------------------------------------------

static inline float Clamp(float v, float lo, float hi)
{
	return (v < lo) ? lo : (v > hi) ? hi : v;
}

// --------------------------------------------------------------------------
// FromEulerPRY  –  R = Rz(yaw) * Ry(roll) * Rx(pitch)
//
// Resulting matrix (display form, column-major storage):
//   m[0]= cr*cy             m[3]= sp*sr*cy - cp*sy   m[6]= cp*sr*cy + sp*sy
//   m[1]= cr*sy             m[4]= sp*sr*sy + cp*cy   m[7]= cp*sr*sy - sp*cy
//   m[2]= -sr               m[5]= cr*sp              m[8]= cr*cp
// --------------------------------------------------------------------------

static void FromEulerPRY(NiMatrix33& out, float pitch, float roll, float yaw)
{
	const float sp = sinf(pitch), cp = cosf(pitch);
	const float sr = sinf(roll), cr = cosf(roll);
	const float sy = sinf(yaw), cy = cosf(yaw);

	float* o = out.flat();
	o[0] = cr * cy;
	o[1] = cr * sy;
	o[2] = -sr;
	o[3] = sp * sr * cy - cp * sy;
	o[4] = sp * sr * sy + cp * cy;
	o[5] = cr * sp;
	o[6] = cp * sr * cy + sp * sy;
	o[7] = cp * sr * sy - sp * cy;
	o[8] = cr * cp;
}

// --------------------------------------------------------------------------
// FromEulerPRYInv  –  transpose of FromEulerPRY (= inverse rotation)
//
//   m[0]= cr*cy                m[3]= cr*sy                m[6]= -sr
//   m[1]= sp*sr*cy - cp*sy     m[4]= sp*sr*sy + cp*cy     m[7]= cr*sp
//   m[2]= cp*sr*cy + sp*sy     m[5]= cp*sr*sy - sp*cy     m[8]= cr*cp
// --------------------------------------------------------------------------

static void FromEulerPRYInv(NiMatrix33& out, float pitch, float roll, float yaw)
{
	const float sp = sinf(pitch), cp = cosf(pitch);
	const float sr = sinf(roll), cr = cosf(roll);
	const float sy = sinf(yaw), cy = cosf(yaw);

	float* o = out.flat();
	o[0] = cr * cy;
	o[1] = sp * sr * cy - cp * sy;
	o[2] = cp * sr * cy + sp * sy;
	o[3] = cr * sy;
	o[4] = sp * sr * sy + cp * cy;
	o[5] = cp * sr * sy - sp * cy;
	o[6] = -sr;
	o[7] = cr * sp;
	o[8] = cr * cp;
}

// --------------------------------------------------------------------------
// ToEulerPRYInv  –  extract Euler angles from a PRYInv-convention matrix
//
//   yaw   = atan2(m[3], m[0])
//   roll  = asin(-m[6])           (clamped to [-1,1])
//   pitch = atan2(m[7], m[8])
// --------------------------------------------------------------------------

static void ToEulerPRYInv(const NiMatrix33& in, float& pitch, float& roll, float& yaw)
{
	const float* o = in.flat();
	yaw = atan2f(o[3], o[0]);
	roll = asinf(Clamp(-o[6], -1.0f, 1.0f));
	pitch = atan2f(o[7], o[8]);
}

// --------------------------------------------------------------------------
// ToEulerPRY  –  extract Euler angles from a PRY-convention matrix
//
//   yaw   = atan2(m[1], m[0])
//   roll  = asin(-m[2])           (clamped to [-1,1])
//   pitch = atan2(m[5], m[8])
// --------------------------------------------------------------------------

static void ToEulerPRY(const NiMatrix33& in, float& pitch, float& roll, float& yaw)
{
	const float* o = in.flat();
	yaw = atan2f(o[1], o[0]);
	roll = asinf(Clamp(-o[2], -1.0f, 1.0f));
	pitch = atan2f(o[5], o[8]);
}

// --------------------------------------------------------------------------
// MultiplyMatrices  –  this = this * B   (standard matrix multiply)
// --------------------------------------------------------------------------

static void MultiplyMatrices(NiMatrix33& A, const NiMatrix33& B)
{
	//  A_display[i][j] = sum_k  A_old_display[i][k] * B_display[k][j]
	//
	//  With column-major flat[col*3 + row]:
	//    result[col][row] = sum_k A_old[k][row] * B[col][k]

	float tmp[9];
	const float* a = A.flat();
	const float* b = B.flat();

	for (int col = 0; col < 3; ++col)
		for (int row = 0; row < 3; ++row)
		{
			float sum = 0;
			for (int k = 0; k < 3; ++k) sum += a[k * 3 + row] * b[col * 3 + k];
			tmp[col * 3 + row] = sum;
		}

	for (int i = 0; i < 9; ++i) A.flat()[i] = tmp[i];
}

// --------------------------------------------------------------------------
// MultiplyMatricesInv  –  this = this * B^T
// --------------------------------------------------------------------------

static void MultiplyMatricesInv(NiMatrix33& A, const NiMatrix33& B)
{
	//  Equivalent to multiplying A by the transpose of B.
	//  (B^T)_display[k][j] = B_display[j][k],  so
	//    result_display[i][j] = sum_k A_display[i][k] * B_display[j][k]
	//
	//  In flat col-major:
	//    result[col][row] = sum_k A[k][row] * B[k][col]

	float tmp[9];
	const float* a = A.flat();
	const float* b = B.flat();

	for (int col = 0; col < 3; ++col)
		for (int row = 0; row < 3; ++row)
		{
			float sum = 0;
			for (int k = 0; k < 3; ++k) sum += a[k * 3 + row] * b[k * 3 + col];
			tmp[col * 3 + row] = sum;
		}

	for (int i = 0; i < 9; ++i) A.flat()[i] = tmp[i];
}

// --------------------------------------------------------------------------
// MultiplyVector  –  return mat * vec
// --------------------------------------------------------------------------

static NiVector3 MultiplyVector(const NiMatrix33& mat, const NiVector3& v)
{
	//  Display row i  dot  vec
	const float* m = mat.flat();
	NiVector3 r;
	r.x = m[0] * v.x + m[3] * v.y + m[6] * v.z;
	r.y = m[1] * v.x + m[4] * v.y + m[7] * v.z;
	r.z = m[2] * v.x + m[5] * v.y + m[8] * v.z;
	return r;
}

// --------------------------------------------------------------------------
// Rotate  –  this *= FromEulerPRYInv(rot)
//
// Equivalent to:
//   NiMatrix33 tmp;
//   FromEulerPRYInv(tmp, p, r, y);
//   MultiplyMatrices(this, tmp);
// --------------------------------------------------------------------------

static void Rotate(NiMatrix33& mat, float pitch, float roll, float yaw)
{
	NiMatrix33 tmp;
	FromEulerPRYInv(tmp, pitch, roll, yaw);
	MultiplyMatrices(mat, tmp);
}

// --------------------------------------------------------------------------
// From2Points  –  build a "look-at" rotation from pt1 toward pt2.
//
// Computes direction  dir = pt1 - pt2,  then derives:
//   pitch = atan2(-dz, sqrt(dx^2 + dy^2))
//   yaw   = atan2(dx, -dy)          (general case when dx != 0)
//         = atan2(dx,  dy)          (when dy == 0)
//
// The resulting matrix (display form):
//   | sinY               cosY               0     |
//   | -cosY*cosP         sinY*cosP          sinP  |
//   |  cosY*sinP        -sinY*sinP          cosP  |
//
// where sinP, cosP, cosY, sinY are computed from the direction.
// Returns the Euclidean distance |dir|.
// --------------------------------------------------------------------------

static float From2Points(NiMatrix33& out, const NiVector3& pt1, const NiVector3& pt2)
{
	float dx = pt1.x - pt2.x;
	float dy = pt1.y - pt2.y;
	float dz = pt1.z - pt2.z;

	float lenSq = dx * dx + dy * dy + dz * dz;
	float xyLenSq = dx * dx + dy * dy;

	float* o = out.flat();

	// ---------- degenerate: zero direction ----------
	if (lenSq <= 0.0f)
	{
		// Identity
		o[0] = 1;
		o[1] = 0;
		o[2] = 0;
		o[3] = 0;
		o[4] = 1;
		o[5] = 0;
		o[6] = 0;
		o[7] = 0;
		o[8] = 1;
		return 0.0f;
	}

	float len = sqrtf(lenSq);
	float sinP = -dz / len;
	float cosP = sqrtf(xyLenSq) / len;

	float cosY, sinY;

	// ---------- degenerate: purely vertical direction ----------
	if (xyLenSq <= 0.0f)
	{
		cosY = 0.0f;
		sinY = 1.0f;
		// sinP is ±1, cosP is 0 – straight up or down
		if (dz > 0.0f) sinP = -1.0f; // looking down
		else sinP = 1.0f; // looking up
		cosP = 0.0f;
	}
	else
	{
		float invXYLen = 1.0f / sqrtf(xyLenSq);
		cosY = dx * invXYLen;
		// The original assembly negates sinY when dx != 0 (i.e. dy != 0 path)
		if (dy == 0.0f) sinY = dy * invXYLen; // == 0
		else sinY = -dy * invXYLen;
	}

	// Build the rotation matrix
	o[0] = sinY;
	o[1] = -cosY * cosP;
	o[2] = cosY * sinP;
	o[3] = cosY;
	o[4] = sinY * cosP;
	o[5] = -sinY * sinP;
	o[6] = 0.0f;
	o[7] = sinP;
	o[8] = cosP;

	return len;
}


// Parameters
// ----------
//   localRot   : (in/out) the block's local rotation matrix
//   worldRot   : (in)     the block's world rotation matrix  (read-only)
//   parentRot  : (in)     the parent node's world rotation   (read-only)
//   worldPos   : (in)     the block's world translation      (read-only)
//   rotDeg     : (in)     user-supplied rotation (degrees)
//   transform  : (in)     transform mode 0..5
//
// Transform modes
// ---------------
//   0  Set local rotation from Euler PRY angles.
//   1  Multiply (rotate) the current local rotation by the given angles.
//   2  Set local rotation from Euler PRYInv angles.
//   3  (same as 2)
//   4  Add rotDeg to the current world Euler PRYInv, rebuild PRYInv,
//      then convert to local space via parentRot^T.
//   5  Build a look-at rotation from the block's world position toward
//      the point given in rotDeg, then convert to local space.
// ==========================================================================

void SetNifBlockRotation(NiMatrix33& localRot, const NiMatrix33& worldRot, const NiMatrix33& parentRot, const NiVector3& worldPos,
                         const NiVector3& rotDeg, unsigned int transform)
{
	// Convert degrees -> radians
	const float p = rotDeg.x * kDegToRad;
	const float r = rotDeg.y * kDegToRad;
	const float y = rotDeg.z * kDegToRad;

	switch (transform)
	{
	default: case 0:
		// Set rotation from Euler PRY (radians)
		FromEulerPRY(localRot, p, r, y);
		break;

	case 1:
		// Rotate: localRot *= FromEulerPRYInv(rot)
		Rotate(localRot, p, r, y);
		break;

	case 2:
	case 3:
		// Set rotation from Euler PRYInv (radians)
		FromEulerPRYInv(localRot, p, r, y);
		break;

	case 4:
		{
			// Extract current world Euler PRYInv, add user angles, rebuild,
			// then transform from world space to local space:
			//   localRot = FromEulerPRYInv(current + user) * parentRot^T
			float wp, wr, wy;
			ToEulerPRYInv(worldRot, wp, wr, wy);

			FromEulerPRYInv(localRot, p + wp, r + wr, y + wy);
			MultiplyMatricesInv(localRot, parentRot);
			break;
		}

	case 5:
		{
			// Build a look-at rotation (world space), then convert to local:
			//   localRot = From2Points(worldPos, rotPoint) * parentRot^T
			// Note: rotDeg is treated as a world-space point here (not degrees).
			NiVector3 rotPoint = {rotDeg.x, rotDeg.y, rotDeg.z};
			From2Points(localRot, worldPos, rotPoint);
			MultiplyMatricesInv(localRot, parentRot);
			break;
		}
	}
}

static NiNode* GetNiNode(TESObjectREFR* thisObj)
{
	auto* rs = thisObj->loadedData;
	if (!rs) return nullptr;

	NiNode* rootNode = rs->rootNode;

	if (thisObj->refID == 0x14)
	{
		auto* pc = PlayerCharacter::GetSingleton();
		if (!pc->bIs3rdPerson) return pc->node1stPerson;
	}

	return rootNode;
}

static NiAVObject* GetNifBlock(TESObjectREFR* thisObj, UInt32 pcNode, const char* blockName)
{
	NiNode* rootNode = nullptr;

	if (pcNode && thisObj->refID == 0x14)
	{
		auto* pc = PlayerCharacter::GetSingleton();
		if (pcNode & 1)
		{
			// 3rd-person root
			auto* rs = pc->loadedData;
			if (!rs) return nullptr;
			rootNode = rs->rootNode;
		}
		else
		{
			// 1st-person root
			rootNode = pc->node1stPerson;
		}
	}
	else
	{
		rootNode = GetNiNode(thisObj);
	}

	if (!rootNode) return nullptr;

	if (blockName[0] != '\0') return rootNode->GetBlock(blockName);

	return rootNode;
}


bool Cmd_SetNifBlockRotation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector3 rot;
	UInt32 transform = 0, pcNode = 0;

	if (!ExtractArgs(EXTRACT_ARGS, &blockName, &rot.x, &rot.y, &rot.z, &transform, &pcNode) || !blockName[0]) return true;

	NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName);
	if (!niBlock) return true;

	auto& localRot = niBlock->m_localRotate;
	auto& worldRot = niBlock->m_worldRotate;
	auto& parentRot = niBlock->m_parent->m_worldRotate;

	auto& worldPos = niBlock->m_worldTranslate;

	NiVector3 rotDeg = {rot.x, rot.y, rot.z};

	SetNifBlockRotation(localRot, worldRot, parentRot, worldPos, rotDeg, transform);

	if (niBlock->IsNiNode() && !thisObj->IsActor()) static_cast<NiNode*>(niBlock)->ResetCollision();
	NiUpdateData data;
	niBlock->UpdateDownwardPass(data, 0);

	return true;
}

bool Cmd_GetNifBlockFlag_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 flagID, pcNode = 0;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &flagID, &pcNode) && (flagID <= 31))
	{
		if (NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName); niBlock && (niBlock->m_flags & (1 << flagID)))
		{
			*result = 1;
		}
	}

	if (IsConsoleMode()) Console_Print("GetNifBlockFlag >> %.f", *result);
	return true;
}

bool Cmd_SetNifBlockFlag_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 flagID, doSet, pcNode = 0;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &flagID, &doSet, &pcNode) && (flagID <= 26))
	{
		if (NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			if (doSet) niBlock->m_flags |= (1 << flagID);
			else niBlock->m_flags &= ~(1 << flagID);
		}
	}
	return true;
}

bool Cmd_GetNifBlockScale_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 pcNode = 0;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &pcNode))
	{
		if (NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName)) *result = niBlock->m_localScale;
	}
	if (IsConsoleMode()) Console_Print("GetNifBlockScale >> %.3f", *result);
	return true;
}

bool Cmd_SetNifBlockScale_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	float newScale;
	UInt32 pcNode = 0;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &newScale, &pcNode) && blockName[0])
	{
		if (NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			niBlock->m_localScale = newScale;
			if (niBlock->IsNiNode() && !thisObj->IsActor()) static_cast<NiNode*>(niBlock)->ResetCollision();
			NiUpdateData data;
			niBlock->UpdateDownwardPass(data, 0);
		}
	}
	return true;
}

// Parameters
// ----------
//   localPos   : (in/out) the block's local translation
//   worldRot   : (in)     the block's world rotation matrix  (read-only)
//   transltn   : (in)     user-supplied translation vector
//   transform  : (in)     transform mode 0..2
//
// Transform modes
// ---------------
//   0  Set local translation directly.
//   1  Add the translation vector to the current local translation.
//   2  Rotate the translation vector by the world rotation matrix,
//      then add the result to the current local translation.
// ==========================================================================

void SetNifBlockTranslation(NiVector3& localPos, const NiMatrix33& worldRot, const NiVector3& transltn, unsigned int transform)
{
	switch (transform)
	{
	default: case 0:
		// Set local translation directly.
		localPos = transltn;
		break;

	case 1:
		// Add translation to current local position.
		localPos.x += transltn.x;
		localPos.y += transltn.y;
		localPos.z += transltn.z;
		break;

	case 2:
		{
			// Rotate the translation by the world rotation, then add.
			NiVector3 rotated = MultiplyVector(worldRot, transltn);
			localPos.x += rotated.x;
			localPos.y += rotated.y;
			localPos.z += rotated.z;
			break;
		}
	}
}

bool Cmd_SetNifBlockTranslation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector3 transltn;
	UInt32 pcNode = 0, transform = 0;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &transltn.x, &transltn.y, &transltn.z, &pcNode, &transform) && blockName[0])
	{
		if (NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			auto& localPos = niBlock->m_localTranslate;
			auto& worldRot = niBlock->m_worldRotate;

			NiVector3 trans = {transltn.x, transltn.y, transltn.z};

			SetNifBlockTranslation(localPos, worldRot, trans, transform);

			if (niBlock->IsNiNode())
			{
				if (!thisObj->IsActor()) static_cast<NiNode*>(niBlock)->ResetCollision();
			}
			NiUpdateData data;
			niBlock->UpdateDownwardPass(data, 0);

			return true;
		}
	}
	return true;
}
