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
struct hkVector4;
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

// 08
struct NiRTTI
{
	const char* name;
	NiRTTI* parent;
};

// 24
struct NiMatrix33
{
	float	cr[3][3];

	void ExtractAngles(float& rotX, float& rotY, float& rotZ);
	void RotationMatrix(float rotX, float rotY, float rotZ);
	void Rotate(float rotX, float rotY, float rotZ);
	void MultiplyMatrices(NiMatrix33& matA, NiMatrix33& matB);
	void Dump(const char* title = NULL);
};

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
// 0C
struct NiVector3
{
	float	x, y, z;

	NiVector3() {}
	__forceinline NiVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	__forceinline NiVector3(const NiVector3& rhs) { *this = rhs; }
	__forceinline explicit NiVector3(const __m128 rhs) { SetPS(rhs); }

	__forceinline void operator=(NiVector3&& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	__forceinline void operator=(const NiVector3& rhs)
	{
		_mm_storeu_si64(this, _mm_loadu_si64(&rhs));
		z = rhs.z;
	}


	__forceinline NiVector3& SetPS(const __m128 rhs)
	{
		_mm_storeu_si64(this, _mm_castps_si128(rhs));
		_mm_store_ss(&z, _mm_unpackhi_ps(rhs, rhs));
		return *this;
	}

	__forceinline __m128 operator+(__m128 packedPS) const { return PS() + packedPS; }
	__forceinline __m128 operator-(__m128 packedPS) const { return PS() - packedPS; }
	__forceinline __m128 operator*(float s) const { return PS() * _mm_set_ps1(s); }
	__forceinline __m128 operator*(__m128 packedPS) const { return PS() * packedPS; }

	__forceinline NiVector3& operator+=(__m128 packedPS) { return SetPS(*this + packedPS); }
	__forceinline NiVector3& operator-=(__m128 packedPS) { return SetPS(*this - packedPS); }
	__forceinline NiVector3& operator*=(float s) { return SetPS(*this * s); }
	__forceinline NiVector3& operator*=(__m128 packedPS) { return SetPS(*this * packedPS); }

	inline operator float* () { return &x; }

	__forceinline __m128 PS() const { return _mm_loadu_ps(&x); }
	__forceinline __m128 PS2() const { return _mm_castsi128_ps(_mm_loadu_si64(this)); }

};
typedef NiVector3 NiPoint3;

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


class NiMemObject
{
	NiMemObject();
	~NiMemObject();
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
	~NiObject();

	virtual NiRTTI* GetType();
	virtual NiNode* GetNiNode();
	virtual BSFadeNode* GetFadeNode();
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(UInt32 arg);
	virtual void	Unk_13(UInt32 arg);
	virtual void	Unk_14(UInt32 arg);
	virtual void	Unk_15(UInt32 arg);
	virtual void	Unk_16(UInt32 arg);
	virtual void	Unk_17(UInt32 arg);
	virtual void	Unk_18(UInt32 arg);
	virtual void	Unk_19(UInt32 arg);
	virtual void	Unk_1A(UInt32 arg);
	virtual void	Unk_1B(UInt32 arg);
	virtual void	Unk_1C(void);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(UInt32 arg);
	virtual UInt32	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual void	Unk_21(UInt32 arg);
	virtual void	Unk_22(void);
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

static struct NiUpdateData
{
	float fTime;
	UInt8 isUpdateControllers;
	UInt8 bIsMultiThreaded;
	UInt8 byte06;
	UInt8 bUpdateGeomorphs;
	UInt8 bUpdateShadowSceneNode;
	UInt8 gap09[3];
} DefaultNodeUpdateParams;

// 9C
class NiAVObject : public NiObjectNET
{
public:
	NiAVObject();
	~NiAVObject();

	virtual void	Unk_23(UInt32 arg1);
	virtual void	Unk_24(NiMatrix33* arg1, NiVector3* arg2, bool arg3);
	virtual void	Unk_25(UInt32 arg1);
	virtual void	Unk_26(UInt32 arg1);
	virtual void	Unk_27(UInt32 arg1);
	virtual void	Unk_28(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void	Unk_29(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_2A(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_2B(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_2C(UInt32 arg1);
	virtual void	Unk_2D(UInt32 arg1);
	virtual void	UpdateTransform(UInt32 arg1);
	virtual void	Unk_2F(void);
	virtual void	UpdateBounds(UInt32 arg1);
	virtual void	Unk_31(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_32(UInt32 arg1);
	virtual void	Unk_33(UInt32 arg1);
	virtual void	Unk_34(void);
	virtual void	OnVisible(NiCullingProcess* cullingProcess);
	virtual void	Unk_36(UInt32 arg1);

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
		kNiFlag_UnkBit27 = 0x8000000,
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
	NiTArray<NiAVObject*>	m_children;		// 9C
};
STATIC_ASSERT(sizeof(NiNode) == 0xAC);

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