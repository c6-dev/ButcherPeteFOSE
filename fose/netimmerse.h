#pragma once
#include "GameTypes.h"
#include <xmmintrin.h>
#include <immintrin.h>
class BSFadeNode;
class NiMultiTargetTransformController;
class NiTextKeyExtraData;
class NiControllerManager;
class NiDefaultAVObjectPalette;
class NiSourceTexture;
class bhkNiCollisionObject;
class hkpWorldObject;
class NiLight;
class BSCubeMapCamera;
class NiFrustumPlanes;
class NiCullingProcess;
class NiTexture;
class NiSkinPartition;
class NiSkinInstance;
class NiSourceCubeMap;
class NiRenderedCubeMap;
class NiDepthStencilBuffer;
class NiRenderTargetGroup;
class NiGeometryData;
class NiRenderedTexture;
class NiTriStrips;
class NiParticles;
class NiLines;
class IDirect3DDevice9;
class NiDX9RenderState;
class NiUnsharedGeometryGroup;
class NiNode;
class TESObjectREFR;
class Tile;
class BSPortalGraph;
class NiTriShape;
struct NiSphere;
class NiProperty;
class NiInterpolator;
class NiControllerSequence;

class NiMemObject {
public:
};

class NiGlobalStringTable {
public:
	static char* AddString(const char* string);
	static void IncRefCount(char* string);
	static void DecRefCount(char* string);
};
class NiFixedString : public NiMemObject {
public:
	NiFixedString() : m_kHandle(nullptr) {};
	NiFixedString(const char* string);
	~NiFixedString();

	char* m_kHandle;

	NiFixedString& operator=(const char* string);
	NiFixedString& operator=(NiFixedString& string);
	bool operator==(const NiFixedString& string);
	bool operator==(const char* string);

	operator char* () { return m_kHandle; }
};

struct alignas(16) AlignedVector4
{
	float	x, y, z, w;

	AlignedVector4() {}
	__forceinline AlignedVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	__forceinline AlignedVector4(const AlignedVector4& rhs) { *this = rhs; }
	__forceinline explicit AlignedVector4(const __m128 rhs) { SetPS(rhs); }

	__forceinline void operator=(AlignedVector4&& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	__forceinline void operator=(const AlignedVector4& rhs) { SetPS(rhs.PS()); }

	__forceinline AlignedVector4& SetPS(const __m128 rhs)
	{
		_mm_store_ps(&x, rhs);
		return *this;
	}

	inline operator float* () { return &x; }

	__forceinline __m128 PS() const { return _mm_load_ps(&x); }
};
typedef AlignedVector4 hkVector4;

STATIC_ASSERT(sizeof(hkVector4) == 0x10);

// 08
struct NiRTTI
{
	const char* name;
	NiRTTI* parent;
};
// 0C
class NiPoint3 {
public:
	float x;
	float y;
	float z;


	NiPoint3() : x(0.f), y(0.f), z(0.f) {};
	NiPoint3(const float x, const float y, const float z) : x(x), y(y), z(z) {};
	NiPoint3(const float f) : x(f), y(f), z(f) {};
	NiPoint3(const NiPoint3& src) : x(src.x), y(src.y), z(src.z) {};
	NiPoint3(const NiPoint3* src) : x(src->x), y(src->y), z(src->z) {};



	inline const float operator[] (UInt32 i) const { return ((float*)&x)[i]; };
	inline float operator[] (UInt32 i) { return ((float*)&x)[i]; };

	operator float* () const { return (float*)this; };

	// Point operations

	NiPoint3& operator= (const NiPoint3& pt) {
		x = pt.x;
		y = pt.y;
		z = pt.z;
		return *this;
	};

	NiPoint3& operator= (const NiPoint3* pt) {
		x = pt->x;
		y = pt->y;
		z = pt->z;
		return *this;
	};

	NiPoint3 operator+ (const NiPoint3& pt) const { return NiPoint3(x + pt.x, y + pt.y, z + pt.z); };
	NiPoint3& operator+= (const NiPoint3& pt) {
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	};

	NiPoint3 operator- (const NiPoint3& pt) const { return NiPoint3(x - pt.x, y - pt.y, z - pt.z); };
	NiPoint3 operator- () const { return NiPoint3(-x, -y, -z); };
	NiPoint3& operator-= (const NiPoint3& pt) {
		x -= pt.x;
		y -= pt.y;
		z -= pt.z;
		return *this;
	};

	float operator* (const NiPoint3& pt) const { return x * pt.x + y * pt.y + z * pt.z; };

	// Scalar operations

	NiPoint3 operator* (float fScalar) const { return NiPoint3(fScalar * x, fScalar * y, fScalar * z); };
	friend NiPoint3 operator* (float fScalar, const NiPoint3& pt) { return NiPoint3(fScalar * pt.x, fScalar * pt.y, fScalar * pt.z); };
	NiPoint3& operator*= (float fScalar) {
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		return *this;
	};



	NiPoint3 operator/ (float fScalar) const {
		float fInvScalar = 1.0f / fScalar;
		return NiPoint3(fInvScalar * x, fInvScalar * y, fInvScalar * z);
	};

	NiPoint3& operator/= (float fScalar) {
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;
		return *this;
	};

	// 0x457990
	__forceinline float Length() const {
		[[msvc::flatten]]
		return std::sqrt(x * x + y * y + z * z);
	}

	static float __fastcall Length_Hook(NiPoint3* apThis) {
		return apThis->Length();
	};

	__forceinline float SqrLength() const {
		return x * x + y * y + z * z;
	}

	__forceinline float Dot(const NiPoint3& pt) const {
		return x * pt.x + y * pt.y + z * pt.z;
	}

	__forceinline NiPoint3 Cross(const NiPoint3& pt) const {
		return NiPoint3(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
	}

	__forceinline float Unitize() {
		float fLength = Length();

		if (fLength > 1e-06f) {
			float fRecip = 1.0f / fLength;
			x *= fRecip;
			y *= fRecip;
			z *= fRecip;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			fLength = 0.0f;
		}
		return fLength;
	}

	inline NiPoint3 UnitCross(const NiPoint3& pt) const {
		NiPoint3 cross(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
		float fLength = cross.Length();
		if (fLength > 1e-06f)
			return cross / fLength;
		else
			return NiPoint3(0.0f, 0.0f, 0.0f);
	}

	inline float Distance(const NiPoint3& pt) const {
		return (*this - pt).Length();
	}

	static void PointsPlusEqualFloatTimesPoints(NiPoint3* pkDst, float f, const NiPoint3* pkSrc, unsigned int uiVerts) {
		for (UInt32 i = 0; i < uiVerts; i++) {
			pkDst[i] += f * pkSrc[i];
		}
	}

	void Lerp(const NiPoint3& to, const NiPoint3& from, const double by) {
		x = to.x * (1 - by) + from.x * by;
		y = to.y * (1 - by) + from.y * by;
		z = to.z * (1 - by) + from.z * by;
	}

	NiPoint3& Lerp(const NiPoint3& to, const NiPoint3& from, const NiPoint3& by) {
		x = to.x * (1 - by.x) + from.x * by.x;
		y = to.y * (1 - by.y) + from.y * by.y;
		z = to.z * (1 - by.z) + from.z * by.z;
		return *this;
	}

	float GetLargest() const {
		float largest = x;
		if (y > largest)
			largest = y;
		if (z > largest)
			largest = z;
		return largest;
	}

	static float Sign(NiPoint3 p1, NiPoint3 p2, NiPoint3 p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	static bool PointInTriangle(NiPoint3 pt, NiPoint3 v1, NiPoint3 v2, NiPoint3 v3) {
		bool b1 = Sign(pt, v1, v2) < 0.0;
		bool b2 = Sign(pt, v2, v3) < 0.0;
		bool b3 = Sign(pt, v3, v1) < 0.0;

		return (b1 == b2) && (b2 == b3);
	}

	static NiPoint3 GetTriangleCenter(NiPoint3 v1, NiPoint3 v2, NiPoint3 v3) {
		return NiPoint3((v1.x + v2.x + v3.x) / 3.0f, (v1.y + v2.y + v3.y) / 3.0f, (v1.z + v2.z + v3.z) / 3.0f);
	}

	static const NiPoint3 UNIT_X;
	static const NiPoint3 UNIT_Y;
	static const NiPoint3 UNIT_Z;
	static const NiPoint3 UNIT_ALL;
	static const NiPoint3 ZERO;
};

STATIC_ASSERT(sizeof(NiPoint3) == 0xC);
typedef NiPoint3 NiVector3;
// 24
class NiMatrix3 {
public:
	float	m_pEntry[3][3];

	NiMatrix3() { memset(m_pEntry, 0, sizeof(m_pEntry)); }
	NiMatrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		m_pEntry[0][0] = m00;
		m_pEntry[0][1] = m10;
		m_pEntry[0][2] = m20;
		m_pEntry[1][0] = m01;
		m_pEntry[1][1] = m11;
		m_pEntry[1][2] = m21;
		m_pEntry[2][0] = m02;
		m_pEntry[2][1] = m12;
		m_pEntry[2][2] = m22;
	}
	bool operator==(const NiMatrix3& mat) const;
	NiMatrix3 operator+(const NiMatrix3& mat) const;
	NiMatrix3 operator-(const NiMatrix3& mat) const;
	NiMatrix3 operator*(const NiMatrix3& mat) const;
	NiMatrix3 operator*(float fScalar) const;
	NiPoint3 operator*(const NiPoint3& pt) const;
	friend NiPoint3 operator*(const NiPoint3& pt, const NiMatrix3& mat);

	NiMatrix3 operator/(float fScalar) const;

	void MakeXRotation(float fAngle);

	void MakeYRotation(float fAngle);

	void MakeZRotation(float fAngle);

	void MakeRotation(float fAngle, float x, float y, float z);
	void MakeRotation(float angle, const NiPoint3& axis);

	void FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle);
	bool ToEulerAnglesXYZ(float& arfXAngle, float& arfYAngle, float& arfZAngle) const;

	__forceinline void SetCol(UInt32 uiCol, const NiPoint3& col) {
		m_pEntry[0][uiCol] = col.x;
		m_pEntry[1][uiCol] = col.y;
		m_pEntry[2][uiCol] = col.z;
	}
	__forceinline void GetCol(UInt32 uiCol, float& f0, float& f1, float& f2) const {

		f0 = m_pEntry[0][uiCol];
		f1 = m_pEntry[1][uiCol];
		f2 = m_pEntry[2][uiCol];

	}

	__forceinline void GetCol(UInt32 uiCol, NiPoint3& col) const {

		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];

	}

	__forceinline NiPoint3 GetCol(UInt32 uiCol) const {
		NiPoint3 col;

		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];
		col.z = m_pEntry[2][uiCol];

		return col;
	}

	NiMatrix3(const NiPoint3& col0, const NiPoint3& col1, const NiPoint3& col2) {
		SetCol(0, col0);
		SetCol(1, col1);
		SetCol(2, col2);
	}

	__forceinline void GetRow(UInt32 uiRow, NiPoint3& row) const {

		row.x = m_pEntry[uiRow][0];
		row.y = m_pEntry[uiRow][1];
		row.z = m_pEntry[uiRow][2];

	}

	float* GetRow(UInt32 uiRow) {
		NiPoint3 row;
		return &m_pEntry[uiRow][0];

	}

	__forceinline void SetRow(UInt32 uiRow, float f0, float f1, float f2) {
		m_pEntry[uiRow][0] = f0;
		m_pEntry[uiRow][1] = f1;
		m_pEntry[uiRow][2] = f2;
	}

	__forceinline float GetEntry(UInt32 uiRow, UInt32 uiCol) const {

		return m_pEntry[uiRow][uiCol];

	}

	__forceinline void SetEntry(UInt32 uiRow, UInt32 uiCol, float fValue) {

		m_pEntry[uiRow][uiCol] = fValue;

	}

	__forceinline NiMatrix3 Transpose() {
		NiPoint3 row[3];

		GetRow(0, row[0]);
		GetRow(1, row[1]);
		GetRow(2, row[2]);

		return NiMatrix3(row[0], row[1], row[2]);
	}

	NiMatrix3 TransposeTimes(const NiMatrix3& mat) const;
	bool Inverse(NiMatrix3& inv) const;


};


STATIC_ASSERT(sizeof(NiMatrix3) == 0x24);

typedef NiMatrix3 NiMatrix33;

struct NiQuaternion;



__forceinline __m128 __vectorcall operator+(__m128 a, __m128 b)
{
	return _mm_add_ps(a, b);
}
__forceinline __m128 __vectorcall operator-(__m128 a, __m128 b)
{
	return _mm_sub_ps(a, b);
}
__forceinline __m128 __vectorcall operator*(__m128 a, __m128 b)
{
	return _mm_mul_ps(a, b);
}
__forceinline __m128 __vectorcall operator&(__m128 a, __m128 b)
{
	return _mm_and_ps(a, b);
}
__forceinline __m128 __vectorcall operator|(__m128 a, __m128 b)
{
	return _mm_or_ps(a, b);
}
__forceinline __m128 __vectorcall operator^(__m128 a, __m128 b)
{
	return _mm_xor_ps(a, b);
}


// 10 - always aligned?
struct NiVector4
{
	float	x, y, z, w;

	NiVector4() {}
	NiVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

// 10 - always aligned?
struct NiQuaternion
{
	float	w, x, y, z;

	NiQuaternion() {}
	NiQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

	void EulerYPR(NiVector3& ypr);
	void RotationMatrix(NiMatrix33& rotMatrix);
	void Dump();
};

// 34
struct NiTransform
{
	NiMatrix33	rotate;		// 00
	NiVector3	translate;	// 24
	float		scale;		// 30
};

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 1C
struct NiFrustum
{
	float	l;			// 00
	float	r;			// 04
	float	t;			// 08
	float	b;			// 0C
	float	n;			// 10
	float	f;			// 14
	UInt8	o;			// 18
	UInt8	pad19[3];	// 19
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// C
struct NiColor
{
	float	r;
	float	g;
	float	b;
};

// 10
struct NiColorAlpha
{
	float	r;
	float	g;
	float	b;
	float	a;
};

// 10
struct NiPlane
{
	NiVector3	nrm;
	float		offset;
};



// 08
class NiRefObject : public NiMemObject
{
public:
	NiRefObject();
	~NiRefObject();

	virtual void	Destructor(bool freeThis);
	virtual void	Free(void);

	UInt32		m_uiRefCount;	// 04
};

// 08
class NiObject : public NiRefObject
{
public:
	NiObject();
	virtual ~NiObject();

	virtual const NiRTTI* GetRTTI() const;												// 02 | Returns NiRTTI of the object
	virtual	NiNode* IsNiNode() const;												// 03 | Returns this if it's a NiNode, otherwise nullptr
	virtual	BSFadeNode* IsFadeNode() const;												// 04 | Returns this if it's a BSFadeNode, otherwise nullptr
	virtual void* IsMultiBoundNode() const;										// 05 | Returns this if it's a BSMultiBoundNode, otherwise nullptr
	virtual void* IsGeometry() const;												// 06 | Returns this if it's a NiGeometry, otherwise nullptr
	virtual void* IsTriBasedGeometry() const;										// 07 | Returns this if it's a NiTriBasedGeom, otherwise nullptr
	virtual NiTriStrips* IsTriStrips() const;											// 08 | Returns this if it's a NiTriStrips, otherwise nullptr
	virtual NiTriShape* IsTriShape() const;												// 09 | Returns this if it's a NiTriShape, otherwise nullptr
	virtual void* IsSegmentedTriShape() const;									// 10 | Returns this if it's a BSSegmentedTriShape, otherwise nullptr
	virtual void* IsResizableTriShape() const;									// 11 | Returns this if it's a BSResizableTriShape, otherwise nullptr
	virtual NiParticles* IsParticlesGeom() const;										// 12 | Returns this if it's a NiParticles, otherwise nullptr
	virtual NiLines* IsLinesGeom() const;											// 13 | Returns this if it's a NiLines, otherwise nullptr
	virtual bhkNiCollisionObject* IsBhkNiCollisionObject() const;									// 14 | Returns this if it's a bhkNiCollisionObject, otherwise nullptr
	virtual void* IsBhkBlendCollisionObject() const;								// 15 | Returns this if it's a bhkBlendCollisionObject, otherwise nullptr
	virtual void* IsBhkRigidBody() const;											// 16 | Returns this if it's a bhkRigidBody, otherwise nullptr
	virtual void* IsBhkLimitedHingeConstraint() const;							// 17 | Returns this if it's a bhkLimitedHingeConstraint, otherwise nullptr
	virtual NiObject* CreateClone(void* apCloning);						// 18 | Creates a clone of this object
	virtual void LoadBinary(void* apStream);									// 19 | Loads objects from disk
	virtual void LinkObject(void* apStream);									// 20 | Called by the streaming system to resolve links to other objects once it can be guaranteed that all objects have been loaded
	virtual void RegisterStreamables(void* apStream);						// 21 | When an object is inserted into a stream, it calls register streamables to make sure that any contained objects or objects linked in a scene graph are streamed as well
	virtual void SaveBinary(void* apStream);									// 22 | Saves objects to disk
	virtual bool IsEqual(NiObject* apObject) const;								// 23 | Compares this object with another
	virtual void GetViewerStrings(void* apStrings);				// 24 | Gets strings containing information about the object
	virtual void AddViewerStrings(void* apStrings);				// 25 | Adds additional strings containing information about the object
	virtual void ProcessClone(void* apCloning);						// 26 | Post process for CreateClone
	virtual void PostLinkObject(void* apStream);								// 27 | Called by the streaming system to resolve any tasks that require other objects to be correctly linked. It is called by the streaming system after LinkObject has been called on all streamed objects
	virtual bool StreamCanSkip();												// 28
	virtual const NiRTTI* GetStreamableRTTI();											// 29
	virtual void SetBound(void* apNewBound);									// 30 | Replaces the bound of the object
	virtual void GetBlockAllocationSize();										// 31 | Used by geometry data
	virtual void* GetGroup();														// 32 | Used by geometry data
	virtual void SetGroup(void* apGroup);								// 33 | Used by geometry data
	virtual NiControllerManager* IsControllerManager() const;									// 34 | Returns this if it's a NiControllerManager, otherwise nullptr

};

static struct NiUpdateData
{
	float fTime;
	UInt8 isUpdateControllers;
	UInt8 bIsMultiThreaded;
	UInt8 byte06;
	UInt8 bUpdateGeomorphs;
	UInt8 bUpdateShadowSceneNode;
	UInt8 gap09[3];

	NiUpdateData()
		: fTime(0.0f),
		isUpdateControllers(0),
		bIsMultiThreaded(0),
		byte06(0),
		bUpdateGeomorphs(0),
		bUpdateShadowSceneNode(0)
	{
		// gap09 is intentionally not initialized
	}
} DefaultNodeUpdateParams;

class NiAVObject;
// 0C
class NiCollisionObject : public NiObject
{
public:
	NiCollisionObject();
	virtual ~NiCollisionObject();

	virtual void		SetSceneGraphObject(NiAVObject* apSceneObject);
	virtual void		UpdateWorldData(NiUpdateData& arData);
	virtual void		RecreateWorldData();
	virtual void		LoadBoundingVolume(void* apData);
	virtual void		Convert(UInt32 uiVersion, void* apData);

	NiAVObject* m_pkSceneObject;
};


class NiTimeController;
class NiExtraData;
// 18
class NiObjectNET : public NiObject
{
public:
	NiObjectNET();
	~NiObjectNET();

	const char* m_blockName;				// 08
	NiTimeController* m_controller;				// 0C
	NiExtraData** m_extraDataList;			// 10
	UInt16				m_extraDataListLen;			// 14
	UInt16				m_extraDataListCapacity;	// 16

	void DumpExtraData();
};


// 9C
class NiAVObject : public NiObjectNET
{
public:
	NiAVObject();
	~NiAVObject();

	virtual void			UpdateControllers(NiUpdateData& arData);
	virtual void			ApplyTransform(NiMatrix3& arMat, NiPoint3& arTrn, bool abOnLeft);
	virtual void			Unk_39();
	virtual NiAVObject*		GetObject_(const NiFixedString& arName);
	virtual NiAVObject*		GetObjectByName(const NiFixedString& arName);
	virtual void			SetSelectiveUpdateFlags(bool& arSelectiveUpdate, BOOL abSelectiveUpdateTransforms, bool& arRigid);
	virtual void			UpdateDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateSelectedDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateRigidDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdatePropertiesDownward(void* apParentState);
	virtual void			Unk_47();
	virtual void			UpdateWorldData(const NiUpdateData& arData);
	virtual void			UpdateWorldBound();
	virtual void			UpdateTransformAndBounds(const NiUpdateData& arData);
	virtual void			PreAttachUpdate(NiNode* apEventualParent, const NiUpdateData& arData);
	virtual void			PreAttachUpdateProperties(NiNode* apEventualParent);
	virtual void			DetachParent();
	virtual void			UpdateUpwardPassParent(void* arg);
	virtual void			OnVisible(NiCullingProcess* apCuller);
	virtual void			PurgeRendererData(void* apRenderer);

	enum
	{
		kNiFlag_Culled = 0x1,
		kNiFlag_SelectiveUpdate = 0x2,
		kNiFlag_SelUpdTransforms = 0x4,
		kNiFlag_SelUpdController = 0x8,
		kNiFlag_SelUpdRigid = 0x10,
		kNiFlag_DisplayObject = 0x20,
		kNiFlag_DisableSorting = 0x40,
		kNiFlag_SelUpdTransformsOverride = 0x80,
		kNiFlag_UnkBit8 = 0x100,
		kNiFlag_SaveExternalGeomData = 0x200,
		kNiFlag_NoDecals = 0x400,
		kNiFlag_AlwaysDraw = 0x800,
		kNiFlag_MeshLOD = 0x1000,
		kNiFlag_FixedBound = 0x2000,
		kNiFlag_TopFadeNode = 0x4000,
		kNiFlag_IgnoreFade = 0x8000,
		kNiFlag_NoAnimSyncX = 0x10000,
		kNiFlag_NoAnimSyncY = 0x20000,
		kNiFlag_NoAnimSyncZ = 0x40000,
		kNiFlag_NoAnimSyncS = 0x80000,
		kNiFlag_NoDismember = 0x100000,
		kNiFlag_NoDismemberValidity = 0x200000,
		kNiFlag_RenderUse = 0x400000,
		kNiFlag_MaterialsApplied = 0x800000,
		kNiFlag_HighDetail = 0x1000000,
		kNiFlag_ForceUpdate = 0x2000000,
		kNiFlag_PreProcessedNode = 0x4000000,
		kNiFlag_PlayerBone = 0x8000000,
		kNiFlag_UnkBit28 = 0x10000000,
		kNiFlag_UnkBit29 = 0x20000000,
		kNiFlag_UnkBit30 = 0x40000000,
		kNiFlag_UnkBit31 = 0x80000000,
	};

	NiAVObject* m_parent;				// 18
	bhkNiCollisionObject* m_collisionObject;		// 1C
	NiSphere* m_kWorldBound;			// 20
	DList<NiProperty>		m_propertyList;			// 24
	UInt32					m_flags;				// 30
	union
	{
		struct
		{
			NiMatrix33				m_localRotate;			// 34
			NiVector3				m_localTranslate;		// 58
			float					m_localScale;			// 64
		};
		NiTransform m_kLocal;
	};

	union
	{
		struct
		{
			NiMatrix33				m_worldRotate;			// 68
			NiVector3				m_worldTranslate;		// 8C
			float					m_worldScale;			// 98
		};
		NiTransform m_kWorld;
	};

	NiProperty* GetProperty(UInt32 propID);

	void DumpProperties();
	void DumpParents();
	void SetVisible(bool visible) { visible ? (m_flags &= ~kNiFlag_Culled) : (m_flags |= kNiFlag_Culled); };
	bool IsVisible() { return !(m_flags & kNiFlag_Culled); }
	void Update(NiUpdateData* params = &DefaultNodeUpdateParams);
	NiPoint3* GetPos() { return (NiPoint3*)&m_worldTranslate; };
	bool IsSelectiveUpdate() { return m_flags & kNiFlag_SelectiveUpdate; };

	void SetLocalTranslate(const NiPoint3& pos);
	void SetLocalTranslate(const NiPoint3* pos);
	const NiPoint3& GetLocalTranslate() const;

	void SetWorldTranslate(const NiPoint3& pos);
	void SetWorldTranslate(const NiPoint3* pos);
	const NiPoint3& GetWorldTranslate() const;
	TESObjectREFR* GetParentRef() const;
};

class NiCamera : NiAVObject
{
	float worldToCam[4][4];
	NiFrustum frustum;
	float minNearPlaneDist;
	float maxFarNearRatio;
	NiViewport viewPort;
	float LODAdjust;
};
STATIC_ASSERT(sizeof(NiCamera) == 0x114);


class NiNode : public NiAVObject {
public:
	NiNode();
	~NiNode();
	NiTArray<NiAVObject*>	m_children;		// 9C

	NiAVObject* GetBlock(const char* blockName);
	NiAVObject* GetBip01();

};
STATIC_ASSERT(sizeof(NiNode) == 0xAC);

class BSFadeNode : public NiNode
{
public:
	BSFadeNode();
	~BSFadeNode();

	enum Type : UInt32
	{
		kObject = 0x1,
		kItem = 0x2,
		kActor = 0x3,
		kLODFadeOutMax = 0xA,
	};

	float fNearDistSqr;
	float fFarDistSqr;
	float fLastFade;
	float fCurrentFade;
	float boundRadius;
	float fTimeSinceUpdate;
	BSFadeNode::Type fadeType;
	UInt32 iFrameCounter;
	TESObjectREFR* linkedObj;
	UInt32 unkD0;
	UInt32 unkD4;
	UInt32 unkD8;
	UInt32 unkDC;
	UInt32 unkE0;
};
STATIC_ASSERT(sizeof(BSFadeNode) == 0xE4);

// 34
class NiTimeController : public NiObject
{
public:
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	SetTarget(NiNode* pTarget);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);

	UInt16								flags;				// 08
	UInt16								unk0A;				// 0A
	float								frequency;			// 0C
	float								phaseTime;			// 10
	float								flt14;				// 14
	float								flt18;				// 18
	float								flt1C;				// 1C
	float								flt20;				// 20
	float								flt24;				// 24
	float								flt28;				// 28
	NiNode* target;			// 2C
	NiMultiTargetTransformController* multiTargetCtrl;	// 30
};

// 7C
class NiControllerManager : public NiTimeController
{
public:
	virtual void	Unk_2D(void);

	NiTArray<NiControllerSequence*>					sequences;		// 34
	void* ptr44;			// 44
	UInt32											unk48;			// 48
	UInt32											unk4C;			// 4C
	NiTMapBase<const char*, NiControllerSequence*>	seqStrMap;		// 50
	UInt32											unk60;			// 60
	NiTArray<void*>* arr64;			// 64
	UInt32											unk68;			// 68
	UInt32											unk6C;			// 6C
	UInt32											unk70;			// 70
	UInt32											unk74;			// 74
	NiDefaultAVObjectPalette* defObjPlt;		// 78
};
STATIC_ASSERT(sizeof(NiControllerManager) == 0x7C);

// 74
class NiControllerSequence : public NiObject
{
public:
	virtual void	Unk_23(void);

	struct ControlledBlock
	{
		NiInterpolator* interpolator;
		NiMultiTargetTransformController* multiTargetCtrl;
		// More
	};

	const char* sequenceName;			// 08
	UInt32				numControlledBlocks;	// 0C
	UInt32				arrayGrowBy;			// 10
	ControlledBlock** controlledBlocks;		// 14
	const char** unkNodeName;			// 18
	float				weight;					// 1C
	NiTextKeyExtraData* textKeyData;			// 20
	UInt32				cycleType;				// 24
	float				frequency;				// 28
	float				beginKeyTime;			// 2C
	float				endKeyTime;				// 30
	float				lastTime;				// 34
	float				weightedLastTime;		// 38
	float				lastScaledTime;			// 3C
	NiControllerManager* owner;					// 40
	UInt32				state;					// 44
	float				offset;					// 48
	float 				startTime;				// 4C
	float 				endTime;				// 50
	float 				destFrame;				// 54
	NiControllerSequence* partnerSequence;		// 58
	const char* accumRootName;			// 5C
	UInt32				unk60[5];				// 60
};
STATIC_ASSERT(sizeof(NiControllerSequence) == 0x74);

// 3C
class TESAnimGroup : public NiRefObject
{
public:
	UInt32			unk08[2];	// 08
	UInt8			index;		// 10
	UInt8			unk11;		// 11
	UInt8			unk12[1];	// 12
	UInt32			unk14[10];	// 14
};

// 78
class BSAnimGroupSequence : public NiControllerSequence
{
public:

	enum eAnimSequence
	{
		kSequence_Idle = 0x0,
		kSequence_Movement = 0x1,
		kSequence_LeftArm = 0x2,
		kSequence_LeftHand = 0x3,
		kSequence_Weapon = 0x4,
		kSequence_WeaponUp = 0x5,
		kSequence_WeaponDown = 0x6,
		kSequence_SpecialIdle = 0x7,
		kSequence_Death = 0x14,
	};

	TESAnimGroup* animGroup;		// 74
};




