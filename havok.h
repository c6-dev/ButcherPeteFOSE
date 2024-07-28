#pragma once

typedef bool hkBool;
typedef UInt16 hkHalf;

template <class T_Data>
class hkSmallArray {
public:
	T_Data* m_data;
	UInt16	m_size;
	UInt16	m_capacityAndFlags;
};

STATIC_ASSERT(sizeof(hkSmallArray<void>) == 0x8);

// 30
struct alignas(16) hkMatrix3x4
{
	float	cr[3][4];

	hkMatrix3x4() {}
	__forceinline hkMatrix3x4(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		cr[0][0] = m00; cr[0][1] = m10; cr[0][2] = m20;
		cr[0][4] = m01; cr[0][5] = m11; cr[0][6] = m21;
		cr[0][8] = m02; cr[0][9] = m12; cr[0][10] = m22;
	}

	__forceinline void operator=(hkMatrix3x4&& rhs)
	{
		cr[0][0] = rhs.cr[0][0]; cr[0][1] = rhs.cr[0][1]; cr[0][2] = rhs.cr[0][2];
		cr[0][4] = rhs.cr[0][4]; cr[0][5] = rhs.cr[0][5]; cr[0][6] = rhs.cr[0][6];
		cr[0][8] = rhs.cr[0][8]; cr[0][9] = rhs.cr[0][9]; cr[0][10] = rhs.cr[0][10];
	}
	__forceinline void operator=(const hkMatrix3x4& from)
	{
		_mm_store_ps(&cr[0][0], _mm_load_ps(&from.cr[0][0]));
		_mm_store_ps(&cr[1][0], _mm_load_ps(&from.cr[1][0]));
		_mm_store_ps(&cr[2][0], _mm_load_ps(&from.cr[2][0]));
	}
	inline operator float* () const { return (float*)this; }


};

typedef AlignedVector4 hkVector4;

struct alignas(16) hkQuaternion
{
	float	x, y, z, w;

	hkQuaternion() {}
	__forceinline hkQuaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	__forceinline hkQuaternion(const hkQuaternion& from) { *this = from; }
	__forceinline hkQuaternion(const hkVector4& from) { *this = from; }
	__forceinline hkQuaternion(const NiQuaternion& niQt) { *this = niQt; }
	__forceinline explicit hkQuaternion(const __m128 rhs) { SetPS(rhs); }

	__forceinline void operator=(hkQuaternion&& from)
	{
		x = from.x;
		y = from.y;
		z = from.z;
		w = from.w;
	}
	__forceinline void operator=(const hkQuaternion& from) { SetPS(from.PS()); }
	__forceinline void operator=(const hkVector4& from) { SetPS(from.PS()); }

	__forceinline hkQuaternion& SetPS(const __m128 rhs)
	{
		_mm_store_ps(&x, rhs);
		return *this;
	}

	__forceinline __m128 operator+(const hkQuaternion& rhs) const { return PS() + rhs.PS(); }
	__forceinline __m128 operator-(const hkQuaternion& rhs) const { return PS() - rhs.PS(); }

	__forceinline __m128 operator*(float s) const { return PS() * _mm_set_ps1(s); }

	__forceinline hkQuaternion& operator+=(const hkQuaternion& rhs) { return SetPS(*this + rhs); }

	__forceinline hkQuaternion& operator-=(const hkQuaternion& rhs) { return SetPS(*this - rhs); }

	__forceinline hkQuaternion& operator*=(float s) { return SetPS(*this * s); }


	inline operator float* () { return &x; }
	__forceinline __m128 PS() const { return _mm_load_ps(&x); }
	

	__forceinline __m128 Invert() const { return PS() ^ _mm_load_ps((const float*)0x10C4C00); }

	__forceinline float __vectorcall DotProduct(const hkQuaternion& rhs) const
	{
		__m128 k = _mm_setzero_ps();
		return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(PS() * rhs.PS(), k), k));
	}

};
STATIC_ASSERT(sizeof(hkQuaternion) == 0x10);
// 0C
template <typename T_Data> class hkArray
{
public:
	enum
	{
		kFlag_NoDealloc = 0x80000000,	// data not owned by us
		kFlag_Locked = 0x40000000,	// will never be destroyed
	};

	T_Data* data;				// 00
	UInt32		size;				// 04
	UInt32		capacityAndFlags;	// 08

	UInt32 Size() const { return size; }
	bool Empty() const { return !size; }
	T_Data* Data() const { return const_cast<T_Data*>(data); }

	T_Data& operator[](UInt32 index) { return data[index]; }

	class Iterator
	{
		T_Data* pData;
		UInt32		count;

	public:
		explicit operator bool() const { return count != 0; }
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const { return *pData; }
		T_Data& operator->() const { return *pData; }
		T_Data& Get() const { return *pData; }

		Iterator(hkArray& source) : pData(source.data), count(source.size) {}
	};

	Iterator Begin() { return Iterator(*this); }
};
STATIC_ASSERT(sizeof(hkArray<void*>) == 0xC);
// 04
class hkBaseObject
{
public:
	/*00*/virtual void		Destroy(bool doFree);
};

// 08
class hkReferencedObject : public hkBaseObject
{
public:
	/*04*/virtual void		CalcStatistics(void* collector);
	/*08*/virtual void		Unk_02(void);

	UInt16			sizeAndFlags;		// 04
	UInt16			refCount;			// 06
};
// 0C
class hkpRigidBody;
class bhkRefObject : public NiObject
{
public:
	/*8C*/virtual void		SetObject(hkReferencedObject* object);
	/*90*/virtual void		UpdateRefCount(bool incRef);	// inc/dec ref, depending on arg
	hkpRigidBody*	phkObject;		// 08
};

// 10
class bhkSerializable : public bhkRefObject
{
public:
	/*94*/virtual void		Unk_25(UInt32 arg);
	/*98*/virtual void* GetWorld(void);
	/*9C*/virtual bool		SetWorld(void* inWorld);
	/*A0*/virtual bool		Unk_28(void);
	/*A4*/virtual void		FreeData(bool del);	// free hkData
	/*A8*/virtual UInt32	Unk_2A(void);
	/*AC*/virtual void		LoadHavokData(void* stream);	// called from bhkSerializable::Load after primary load is done
	/*B0*/virtual void		Unk_2C(void);	// create object
	/*B4*/virtual void* CreateHavokData(UInt8* arg);	// create Cinfo, return hkData
	/*B8*/virtual void		Unk_2E(void);		// destroy object
	/*BC*/virtual void		Unk_2F(void);
	/*C0*/virtual void		Unk_30(void);

	void* hkData;	// 0C - stores hkConstraintData (descriptor used to build hkObj)
};
// 14
class bhkWorldObject : public bhkSerializable
{
public:
	/*C4*/virtual void		UpdateCollisionFilter();
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);

	UInt32				bodyFlags;		// 10

};
// 14
class bhkShape : public bhkSerializable
{
public:
	/*C4*/virtual void		Unk_31(void);
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);
	/*D4*/virtual void		Unk_35(void);
	/*D8*/virtual void		Unk_36(void);
	/*DC*/virtual void		Unk_37(void);
	/*E0*/virtual void		Unk_38(void);

	UInt32			materialType;	// 10
};


// 10
class hkpShape : public hkReferencedObject
{
public:
	/*0C*/virtual void		Unk_03(hkVector4* arg1);
	/*10*/virtual UInt32	Unk_04();
	/*14*/virtual bool		IsConvex();
	/*18*/virtual UInt32	Unk_06(UInt32 arg1, UInt32 arg2);
	/*1C*/virtual void		GetSpaceDiagonal(void* arg1, float arg2, void* resPtr);
	/*20*/virtual void* Unk_08(void* arg1, hkVector4* arg2, void* arg3);
	/*24*/virtual void		Unk_09(void* arg1, void* arg2, void* arg3);
	/*28*/virtual void		Unk_0A(hkVector4* arg1, hkVector4* arg2, void* arg3, hkVector4* arg4);
	/*2C*/virtual UInt32	Unk_0B();

	bhkShape* shape;		// 08
	UInt32			unk0C;		// 0C
};
static_assert(sizeof(hkpShape) == 0x10);
// 40
struct hkTransform
{
	hkMatrix3x4	rotation;		// 00
	hkVector4	translation;	// 30
};

// 50
struct hkSweptTransform
{
	hkVector4		centerOfMass0;	// 00 (40)
	hkVector4		centerOfMass1;	// 10 (50)
	hkQuaternion	rotation0;		// 20 (60)
	hkQuaternion	rotation1;		// 30 (70)
	hkVector4		centerOfMassLoc;// 40 (80)
};
// B0
struct hkMotionState
{
	enum SolverDeactivation
	{
		kSolver_Max,
		kSolver_Off,
		kSolver_Low,
		kSolver_Medium,
		kSolver_High
	};

	hkTransform			transform;			// 00
	hkSweptTransform	sweptTransform;		// 40
	hkVector4			deltaAngle;			// 90
	float				objectRadius;		// A0	For spheres: actual radius * tan(PI/3)
	float				linearDamping;		// A4
	float				angularDamping;		// A8
	UInt8				byteAC;				// AC
	UInt8				byteAD;				// AD
	UInt8				solverDeactivation;	// AE
	UInt8				byteAF;				// AF
};
static_assert(sizeof(hkMotionState) == 0xB0);

// 10
struct hkpCdBody
{
	hkpShape* shape;			// 00
	UInt32			shapeKey;		// 04
	hkMotionState* motion;		// 08
	hkpCdBody* parent;		// 0C

	hkpWorldObject* GetWorldObj() const { return (hkpWorldObject*)((UInt8*)this - 0x10); }
};
// 80
struct ContactData
{
	UInt16							unk00;
	UInt16							flags;
	UInt32							key;
	void*							contactMgr;
	UInt32							unk0C;
	hkpCdBody*						cdBody1;
	hkpCdBody*						cdBody2;
	UInt32							unk18[26];
};

// 08
struct CdBodyLinker
{
	ContactData* data;
	hkpCdBody* cdBody;
};

// 10
struct CdParentObj
{
	NiRTTI* type;		// 00	0x1267B70 (bhkNiCollisionObject) / 0x11F4280 (NiAVObject) / 0x11F4A20 (NiTriStrips)
	UInt32		unk04;		// 04
	NiObject* object;	// 08	bhkNiCollisionObject or NiAVObject
	UInt32		unk0C;		// 0C
};

class hkpBroadPhaseHandle
{
public:
	UInt32 m_id;
};


class hkpTypedBroadPhaseHandle : public hkpBroadPhaseHandle {
public:
	SInt8	m_type;
	SInt8	m_ownerOffset;
	SInt8	m_objectQualityType;
	UInt32	m_collisionFilterInfo;
};
static_assert(sizeof(hkpTypedBroadPhaseHandle) == 0xC);
STATIC_ASSERT(offsetof(hkpTypedBroadPhaseHandle, m_type) == 0x4);

class hkpCollidable : public hkpCdBody {
public:
	struct BoundingVolumeData {
		UInt32	m_min[3];
		UInt8	m_expansionMin[3];
		UInt8	m_expansionShift;
		UInt32	m_max[3];
		UInt8	m_expansionMax[3];
		UInt8	m_padding;
		UInt16	m_numChildShapeAabbs;
		UInt16	m_capacityChildShapeAabbs;
		void* m_childShapeAabbs;
	};


	char						m_ownerOffset;
	UInt8						m_forceCollideOntoPpu;
	UInt16						m_shapeSizeOnSpu;
	hkpTypedBroadPhaseHandle	m_broadPhaseHandle;
	BoundingVolumeData			m_boundingVolumeData;
	float						m_allowedPenetrationDepth;
};
STATIC_ASSERT(offsetof(hkpCollidable, m_broadPhaseHandle) == 0x14);
STATIC_ASSERT(sizeof(hkpCollidable) == 0x4C);

class hkpLinkedCollidable : public hkpCollidable {

	struct CollisionEntry
	{
		void* m_agentEntry;
		hkpLinkedCollidable* m_partner;
	};
	hkArray<CollisionEntry> m_collisionEntries;
};

STATIC_ASSERT(sizeof(hkpLinkedCollidable) == 0x58);

class hkMultiThreadCheck
{
public:
	UInt32 m_threadId;
	UInt16 m_markCount;
	UInt16 m_markBitStack;
};
STATIC_ASSERT(sizeof(hkMultiThreadCheck) == 0x8);

class hkpPropertyValue
{
public:
	UInt64 m_data;
};
STATIC_ASSERT(sizeof(hkpPropertyValue) == 0x8);

class hkpProperty
{
	UInt32 m_key;
	UInt32 m_alignmentPadding;
	hkpPropertyValue m_value;
};
STATIC_ASSERT(sizeof(hkpProperty) == 0x10);
class hkpWorld;
class hkpSimulation : public hkReferencedObject {
public:

	UInt32		m_determinismCheckFrameCounter;
	hkpWorld*	m_world;
	UInt8		m_lastProcessingStep;
	float		m_currentTime;
	float		m_currentPsiTime;
	float		m_physicsDeltaTime;
	float		m_simulateUntilTime;
	float		m_frameMarkerPsiSnap;
	UInt32		m_previousStepResult;
};

STATIC_ASSERT(sizeof(hkpSimulation) == 0x2C);

struct hkStepInfo {
	float m_startTime;
	float m_endTime;
	float m_deltaTime;
	float m_invDeltaTime;
};

class alignas(16) hkpSolverInfo
{
public:
	struct DeactivationInfo
	{
		float m_linearVelocityThresholdInv;
		float m_angularVelocityThresholdInv;
		float m_slowObjectVelocityMultiplier;
		float m_relativeSleepVelocityThreshold;
		float m_maxDistSqrd[2];
		hkHalf m_maxRotSqrd[2];
	};
	float m_one;
	float m_tau;
	float m_damping;
	float m_frictionTau;
	hkVector4 m_globalAccelerationPerSubStep;
	hkVector4 m_globalAccelerationPerStep;
	hkVector4 m_integrateVelocityFactor;
	hkVector4 m_invIntegrateVelocityFactor;
	float m_dampDivTau;
	float m_tauDivDamp;
	float m_dampDivFrictionTau;
	float m_frictionTauDivDamp;
	float m_contactRestingVelocity;
	DeactivationInfo m_deactivationInfo[6];
	float m_deltaTime;
	float m_invDeltaTime;
	int m_numSteps;
	int m_numMicroSteps;
	float m_invNumMicroSteps;
	float m_invNumSteps;
	hkBool m_forceCoherentConstraintOrderingInSolver;
	UInt8 m_deactivationNumInactiveFramesSelectFlag[2];
	UInt8 m_deactivationIntegrateCounter;
};
STATIC_ASSERT(sizeof(hkpSolverInfo) == 0x130);

class hkpWorldDynamicsStepInfo
{
public:
	hkStepInfo m_stepInfo;
	hkpSolverInfo m_solverInfo;
};

STATIC_ASSERT(sizeof(hkpWorldDynamicsStepInfo) == 0x140);

class alignas(16) hkpWorld : hkReferencedObject
{
	hkpSimulation* m_simulation;
	__declspec(align(8)) hkVector4 m_gravity;
	void* m_fixedIsland;
	hkpRigidBody* m_fixedRigidBody;
	hkArray<void*> m_activeSimulationIslands;
	hkArray<void*> m_inactiveSimulationIslands;
	hkArray<void*> m_dirtySimulationIslands;
	void* m_maintenanceMgr;
	void* m_memoryWatchDog;
	void* m_broadPhase;
	void* m_broadPhaseDispatcher;
	void* m_phantomBroadPhaseListener;
	void* m_entityEntityBroadPhaseListener;
	void* m_broadPhaseBorderListener;
	void* m_multithreadedSimulationJobData;
	void* m_collisionInput;
	void* m_collisionFilter;
	void* m_collisionDispatcher;
	void* m_convexListFilter;
	void* m_pendingOperations;
	int m_pendingOperationsCount;
	int m_criticalOperationsLockCount;
	int m_criticalOperationsLockCountForPhantoms;
	hkBool m_blockExecutingPendingOperations;
	hkBool m_criticalOperationsAllowed;
	void* m_pendingOperationQueues;
	int m_pendingOperationQueueCount;
	hkMultiThreadCheck m_multiThreadCheck;
	hkBool m_processActionsInSingleThread;
	unsigned int m_minDesiredIslandSize;
	void* m_modifyConstraintCriticalSection;
	void* m_worldLock;
	void* m_islandDirtyListCriticalSection;
	void* m_propertyMasterLock;
	hkBool m_wantSimulationIslands;
	float m_snapCollisionToConvexEdgeThreshold;
	float m_snapCollisionToConcaveEdgeThreshold;
	hkBool m_enableToiWeldRejection;
	hkBool m_wantDeactivation;
	hkBool m_shouldActivateOnRigidBodyTransformChange;
	float m_deactivationReferenceDistance;
	float m_toiCollisionResponseRotateNormal;
	hkBool m_processToisMultithreaded;
	int m_maxSectorsPerCollideTask;
	int m_maxEntriesPerToiCollideTask;
	UInt8 m_simulationType;
	unsigned int m_lastEntityUid;
	hkArray<void*> m_phantoms;
	hkArray<void*> m_actionListeners;
	hkArray<void*> m_entityListeners;
	hkArray<void*> m_phantomListeners;
	hkArray<void*> m_constraintListeners;
	hkArray<void*> m_worldDeletionListeners;
	hkArray<void*> m_islandActivationListeners;
	hkArray<void*> m_worldPostSimulationListeners;
	hkArray<void*> m_worldPostIntegrateListeners;
	hkArray<void*> m_worldPostCollideListeners;
	hkArray<void*> m_islandPostIntegrateListeners;
	hkArray<void*> m_islandPostCollideListeners;
	hkArray<void*> m_collisionListeners;
	hkArray<void*> m_contactImpulseLimitBreachedListeners;
	void* m_broadPhaseBorder;
	hkpWorldDynamicsStepInfo m_dynamicsStepInfo;
	hkVector4 m_broadPhaseExtents[2];
	int m_broadPhaseNumMarkers;
	int m_sizeOfToiEventQueue;
	int m_broadPhaseQuerySize;
	int m_broadPhaseUpdateSize;
	SInt8 m_contactPointGeneration;
};
STATIC_ASSERT(sizeof(hkpWorld) == 0x310);
// 80
class hkpWorldObject : public hkReferencedObject
{
public:
	/*0C*/virtual void		SetShape(hkpShape* shape);
	/*10*/virtual hkMotionState* GetMotionState();

	enum FilterFlags
	{
		kFilterFlag_Scaled = 0x20,
		kFilterFlag_NoCollision = 0x40,
		kFilterFlag_Linked = 0x80
	};

	hkpWorld* m_world;
	UInt32 m_userData;
	hkpLinkedCollidable m_collidable;
	hkMultiThreadCheck m_multiThreadCheck;
	const char* m_name;
	hkArray<hkpProperty> m_properties;
};
static_assert(sizeof(hkpWorldObject) == 0x80);
STATIC_ASSERT(offsetof(hkpWorldObject, m_collidable) == 0x10);
// 38
class hkpConstraintInstance : public hkReferencedObject
{
public:
	/*0C*/virtual void		Unk_03(void);
	/*10*/virtual void		Unk_04(void);
	/*14*/virtual void		Unk_05(void);
	/*18*/virtual void		Unk_06(void);
	/*1C*/virtual void		Unk_07(void);

	void* simIsle;			// 08
	void* constraintData;	// 0C
	UInt32					unk10;				// 10
	void* contactBody;		// 14
	void* parentBody;		// 18
	UInt32					unk1C[5];			// 1C
	void* ptr30;				// 30
	UInt32					unk34;				// 34
};

class hkpMaterial
{
public:
	SInt8 m_responseType;
	float m_friction;
	float m_restitution;
};
STATIC_ASSERT(sizeof(hkpMaterial) == 0xC);

// 120
class hkpMotion : public hkReferencedObject
{
public:
	/*0C*/virtual void		SetBodyMass(float _bodyMass);
	/*10*/virtual void		SetBodyMassInv(float _bodyMassInv);
	/*14*/virtual void		GetInertiaLocal(hkMatrix3x4* outInertia);
	/*18*/virtual void		GetInertiaWorld(hkMatrix3x4* outInertia);
	/*1C*/virtual void		SetInertiaLocal(hkMatrix3x4* inInertia);
	/*20*/virtual void		SetInertiaInvLocal(hkMatrix3x4* inInertia);
	/*24*/virtual void		GetInertiaInvLocal(hkMatrix3x4* outInertia);
	/*28*/virtual void		GetInertiaInvWorld(hkMatrix3x4* outInertia);
	/*2C*/virtual void		SetCenterOfMassInLocal(hkVector4* centerOfMass);
	/*30*/virtual void		SetPosition(hkVector4* position);
	/*34*/virtual void		SetRotation(hkQuaternion* rotation);
	/*38*/virtual void		SetPositionAndRotation(hkVector4* position, hkQuaternion* rotation);
	/*3C*/virtual void		SetTransform(hkTransform* transform);
	/*40*/virtual void		SetLinearVelocity(hkVector4* velocity);
	/*44*/virtual void		SetAngularVelocity(hkVector4* velocity);
	/*48*/virtual void		GetProjectedPointVelocity(hkVector4* point, hkVector4* normal, float* outVel, float* outInvVirtMass);
	/*4C*/virtual void		ApplyLinearImpulse(hkVector4* impulse);
	/*50*/virtual void		ApplyPointImpulse(hkVector4* impulse, hkVector4* point);
	/*54*/virtual void		ApplyAngularImpulse(hkVector4* impulse);
	/*58*/virtual void		ApplyPointForce(float deltaTime, hkVector4* force, hkVector4* point);
	/*5C*/virtual void		ApplyForce(float deltaTime, hkVector4* force);
	/*60*/virtual void		ApplyTorque(float deltaTime, hkVector4* torque);
	/*64*/virtual void		GetMotionStateAndVelocitiesAndDeactivationType(hkpMotion* outMotion);

	enum MotionType
	{
		kMotionType_Sphere = 2,
		kMotionType_Box,
		kMotionType_KeyframedRigid,
		kMotionType_FixedRigid,
		kMotionType_ThinBox,
		kMotionType_Character,
	};

	UInt8				m_type;
	UInt8				m_deactivationIntegrateCounter;
	UInt16				m_deactivationNumInactiveFrames[2];
	hkMotionState		m_motionState;
	hkVector4			m_inertiaAndMassInv;
	hkVector4			m_linearVelocity;
	hkVector4			m_angularVelocity;
	hkVector4			m_deactivationRefPosition[2];
	UInt32				m_deactivationRefOrientation[2];
	void* m_savedMotion;
	UInt16				m_savedQualityTypeIndex;
	UInt16				m_gravityFactor;

};
static_assert(sizeof(hkpMotion) == 0x120);

class hkpEntity : public hkpWorldObject
{
public:

	struct SpuCollisionCallback {
		void*	m_util;
		UInt16	m_capacity;
		UInt8	m_eventFilter;
		UInt8	m_userFilter;
	};
	struct ExtendedListeners {
		hkSmallArray<void*>			m_activationListeners;
		hkSmallArray<void*>			m_entityListeners;
	};
	hkpMaterial							m_material;
	void*								m_breakOffPartsUtil;
	UInt32								m_solverData;
	UInt16								m_storageIndex;
	UInt16								m_contactPointCallbackDelay;
	hkSmallArray<void>					m_constraintsMaster;
	hkArray<hkpConstraintInstance*>		m_constraintsSlave;
	hkArray<UInt8>						m_constraintRuntime;
	void*								m_simulationIsland;
	char								m_autoRemoveLevel;
	UInt8								m_numShapeKeysInContactPointProperties;
	UInt32								m_uid;
	SpuCollisionCallback				m_spuCollisionCallback;
	ExtendedListeners*					m_extendedListeners;
	hkpMotion							m_motion;
	hkSmallArray<void*>					m_contactListeners;
	hkSmallArray<void*>					m_actions;

	bool IsMobile() const { return (m_motion.m_type & 2) != 0; }

};
STATIC_ASSERT(sizeof(hkpEntity::ExtendedListeners) == 0x10);
STATIC_ASSERT(sizeof(hkpEntity::SpuCollisionCallback) == 0x8);
STATIC_ASSERT(sizeof(hkpEntity) == 0x200);


// 220
class hkpRigidBody : public hkpEntity
{
public:

};
STATIC_ASSERT(offsetof(hkpRigidBody, m_collidable.m_broadPhaseHandle.m_type) == 0x28);
STATIC_ASSERT(sizeof(hkpRigidBody) == 0x200);
// 0C
class NiCollisionObject : public NiObject
{
public:
	/*8C*/virtual void		Attach(NiAVObject* obj);
	/*90*/virtual void		Unk_24(UInt32 arg);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(UInt32 arg);
	/*9C*/virtual void		Unk_27(UInt32 version, UInt32 arg1);

	NiNode* linkedNode;	// 08
};

// 14
class bhkNiCollisionObject : public NiCollisionObject
{
public:
	/*A0*/virtual NiVector3* GetLinearVelocityInGameUnits(NiVector3* outVel);
	/*A4*/virtual void		CollisionObjTransformToTargetNode();
	/*A8*/virtual void		TargetNodeTransformToCollisionObj();
	/*AC*/virtual void		UpdateVelocity();
	/*B0*/virtual void		SetMotionType(UInt32 moType, void* body, bool updateMotion);
	/*B4*/virtual bool		IsFixedMotion();
	/*B8*/virtual void		SetTargetNodeTransform(NiTransform* transform);
	/*BC*/virtual bool		Unk_2F();
	/*C0*/virtual void		ToggleDebugDisplay(bool toggle);	//	TCG

	enum Flags
	{
		kFlag_Active = 1,
		kFlag_Notify = 4,
		kFlag_SetLocal = 8,
		kFlag_DebugDisplay = 0x10,	//	TCG
		kFlag_UseVelocity = 0x20,
		kFlag_Reset = 0x40,
		kFlag_SyncOnUpdate = 0x80,
		kFlag_AnimTargeted = 0x400,
		kFlag_DismemberLimb = 0x800
	};

	UInt16			flags;			// 0C
	UInt16			word0E;			// 0E
	bhkWorldObject* worldObj;		// 10

	//	Callbacks array @ 0x11AFE88
};

// 14
class bhkCollisionObject : public bhkNiCollisionObject
{
public:
};

// 14
class bhkPCollisionObject : public bhkNiCollisionObject
{
public:
};

// 14
class bhkSPCollisionObject : public bhkPCollisionObject
{
public:
};