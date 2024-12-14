#pragma once
#include "GameObjects.h"
typedef bool hkBool;
typedef UInt16 hkHalf;

enum hkpStepResult : int
{
	HK_STEP_RESULT_SUCCESS = 0x0,
	HK_STEP_RESULT_MEMORY_FAILURE_BEFORE_INTEGRATION = 0x1,
	HK_STEP_RESULT_MEMORY_FAILURE_DURING_COLLIDE = 0x2,
	HK_STEP_RESULT_MEMORY_FAILURE_DURING_TOI_SOLVE = 0x3,
};

class hkpRigidBody;

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
	/*08*/virtual void		calcContentStatistics(void* collector);

	UInt16			sizeAndFlags;		// 04
	UInt16			refCount;			// 06
};
class bhkShape;

// 10
class hkpShape : public hkReferencedObject
{
public:

	bhkShape*		shape;		// 08
	UInt32			m_type;		// 0C
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
	UInt8				m_maxLinearVelocity;
	UInt8				m_maxAngularVelocity;
	UInt8				m_deactivationClass;
};
static_assert(sizeof(hkMotionState) == 0xB0);

// 10
struct hkpCdBody
{
	hkpShape*		shape;			// 00
	UInt32			shapeKey;		// 04
	hkMotionState*	motion;		// 08
	hkpCdBody*		parent;		// 0C

	hkpWorldObject* GetWorldObj() const { return (hkpWorldObject*)((UInt8*)this - 0x10); }
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
		void*	m_childShapeAabbs;
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
public:
	UInt32 m_key;
	UInt32 m_alignmentPadding;
	hkpPropertyValue m_value;
};
STATIC_ASSERT(sizeof(hkpProperty) == 0x10);

struct hkStepInfo {
	float m_startTime;
	float m_endTime;
	float m_deltaTime;
	float m_invDeltaTime;
};

class hkpWorld;
class hkpEntity;
class hkpSimulation : public hkReferencedObject {
public:
	virtual hkpStepResult stepDeltaTime(float);
	virtual hkpStepResult integrate(float);
	virtual hkpStepResult collide();
	virtual hkpStepResult advanceTime();
	virtual void collideEntitiesDiscrete(hkpEntity**, int, hkpWorld*, const hkStepInfo*, int);
	virtual void resetCollisionInformationForEntities(hkpEntity**, int, hkpWorld*, hkBool);
	virtual void assertThereIsNoCollisionInformationForEntities(hkpEntity**, int, hkpWorld*);
	virtual void removeCollisionInformationForAgent(void*);
	virtual void assertThereIsNoCollisionInformationForAgent(void*);
	virtual void reintegrateAndRecollideEntities(hkpEntity**, int, hkpWorld*, int);
	virtual void collideInternal(const hkStepInfo*);
	virtual void warpTime(float);

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
	/*0C*/virtual void				SetShape(hkpShape* shape);
	/*10*/virtual hkMotionState*	GetMotionState();

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
STATIC_ASSERT(offsetof(hkpWorldObject, m_properties) == 0x74);

class hkpConstraintAtom
{
public:
	UInt16 m_type;
};
static_assert(sizeof(hkpConstraintAtom) == 0x2);

class hkpConstraintInstance;
class alignas(16) hkConstraintInternal
{
public:
	hkpConstraintInstance* m_constraint;
	hkpEntity* m_entities[2];
	hkpConstraintAtom* m_atoms;
	UInt16 m_atomsSize;
	UInt8 m_callbackRequest;
	UInt8 m_priority;
	UInt16 m_sizeOfSchemas;
	UInt16 m_numSolverResults;
	UInt16 m_numSolverElemTemps;
	UInt8 m_whoIsMaster;
	hkBool m_isNormalType;
	void* m_runtime;
	UInt16 m_runtimeSize;
	UInt16 m_slaveIndex;
};

static_assert(sizeof(hkConstraintInternal) == 0x30);

struct hkpConstraintInfo
{
	int m_maxSizeOfSchema;
	int m_sizeOfSchemas;
	int m_numSolverResults;
	int m_numSolverElemTemps;
};

class hkpConstraintOwner : hkReferencedObject
{
public:
	virtual void addConstraintToCriticalLockedIsland(hkpConstraintInstance*);
	virtual void removeConstraintFromCriticalLockedIsland(hkpConstraintInstance*);
	virtual void addCallbackRequest(hkpConstraintInstance*, int);
	virtual void checkAccessRw();

	hkpConstraintInfo m_constraintInfo;
};
class hkpSolverResults
{
public:
	float m_impulseApplied;
	float m_internalSolverData;
};

class hkpConstraintData : hkReferencedObject
{
public:
	struct hkpConstraintData::RuntimeInfo
	{
		int m_sizeOfExternalRuntime;
		int m_numSolverResults;
	};

	class hkpConstraintData::ConstraintInfo : hkpConstraintInfo
	{
	public:
		hkpConstraintAtom* m_atoms;
		UInt32 m_sizeOfAllAtoms;
	};

	virtual hkBool* isValid(hkBool* result);
	virtual int getType();
	virtual void getRuntimeInfo(hkBool, hkpConstraintData::RuntimeInfo*);
	virtual hkpSolverResults* getSolverResults(void*);
	virtual void addInstance(hkpConstraintInstance*, void*, int);
	virtual void buildJacobian(const void*, void*);
	virtual hkBool*  isBuildJacobianCallbackRequired(hkBool* result);
	virtual void buildJacobianCallback(const void*);
	virtual void getConstraintInfo(hkpConstraintData::ConstraintInfo*);

	UInt32 m_userData;
};
static_assert(sizeof(hkpConstraintData) == 0xC);

class hkpModifierConstraintAtom : hkpConstraintAtom
{
public:
	__declspec(align(16)) UInt16 m_modifierAtomSize;
	UInt16 m_childSize;
	hkpConstraintAtom* m_child;
	unsigned int m_pad[2];
};

static_assert(sizeof(hkpModifierConstraintAtom) == 0x20);

// 2C
class hkpConstraintInstance : public hkReferencedObject
{
public:
	virtual void entityAddedCallback(hkpEntity*);
	virtual void entityRemovedCallback(hkpEntity*);
	virtual void entityDeletedCallback(hkpEntity*);
	virtual int getType();

	hkpConstraintOwner* m_owner;
	hkpConstraintData* m_data;
	hkpModifierConstraintAtom* m_constraintModifiers;
	hkpEntity* m_entities[2];
	UInt8 m_priority;
	hkBool m_wantRuntime;
	const char* m_name;
	UInt32 m_userData;
	hkConstraintInternal* m_internal;
};
static_assert(sizeof(hkpConstraintInstance) == 0x2C);

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

	virtual void deallocateInternalArrays();

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
	NiNode* GetParentNode();
	TESObjectREFR* GetParentRef();

};
STATIC_ASSERT(sizeof(hkpEntity::ExtendedListeners) == 0x10);
STATIC_ASSERT(sizeof(hkpEntity::SpuCollisionCallback) == 0x8);
STATIC_ASSERT(sizeof(hkpEntity) == 0x200);


// 220
class hkpRigidBody : public hkpEntity
{
public:
	virtual hkpRigidBody* clone();
};
STATIC_ASSERT(offsetof(hkpRigidBody, m_collidable.m_broadPhaseHandle.m_type) == 0x28);
STATIC_ASSERT(sizeof(hkpRigidBody) == 0x200);


// 04
class hkpEntityListener
{
public:
	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual void		Unk_01(UInt32 arg);
	/*08*/virtual void		Unk_02(hkpRigidBody* rigidBody);
	/*0C*/virtual void		Unk_03(UInt32 arg);
	/*10*/virtual void		Unk_04(UInt32 arg);
	/*14*/virtual void		Unk_05(UInt32 arg);
};


// 04
class hkpPhantomListener
{
public:
	/*00*/virtual void		Unk_00(void);
	/*04*/virtual void		Unk_01(void);
	/*08*/virtual void		Unk_02(void);
	/*0C*/virtual void		Unk_03(void);
	/*10*/virtual void		Unk_04(void);
};


// D0
class hkpCharacterProxy : public hkReferencedObject
{
public:
	/*0C*/virtual void		Unk_03(void);
	/*10*/virtual void		Unk_04(void);

	hkpEntityListener		entityListener;		// 08
	hkpPhantomListener		phantomListener;	// 0C

	hkVector4				velocity;			// 10
	hkVector4				vector20;			// 20
	void* shapePhantom;		// 30
	UInt32					unk34[16];			// 34
	hkArray<UInt32>			cdPoints;			// 74
	hkArray<UInt32>			arr80;				// 80
	hkArray<UInt32>			arr8C;				// 8C
	hkArray<UInt32>			arr98;				// 98
	UInt32					unkA4[11];			// A4
};
static_assert(sizeof(hkpCharacterProxy) == 0xD0);

class hkpCdPointCollector {
public:
	hkpCdPointCollector();
	virtual ~hkpCdPointCollector();
	virtual void addCdPoint(void* point);

	float m_earlyOutDistance;
};

STATIC_ASSERT(sizeof(hkpCdPointCollector) == 0x8);

// 10
struct hkCdBody
{
	hkpShape* shape;			// 00
	UInt32			shapeKey;		// 04
	hkMotionState* motion;		// 08
	hkCdBody* parent;		// 0C

	hkpWorldObject* GetWorldObj() const { return (hkpWorldObject*)((UInt8*)this - 0x10); }
};

struct hkContactPoint
{
	NiVector4 m_position;
	NiVector4 m_separatingNormal;
};


template <typename T_Data, unsigned T_Count>
class hkInplaceArray : public hkArray<T_Data> {
public:
	T_Data m_storage[T_Count];
};

class hkpAllCdPointCollector : public hkpCdPointCollector {
public:
	virtual void addCdPointAlt(void* event);

	struct hkpRootCdPoint {
		hkContactPoint	m_contact;
		hkpCollidable* m_rootCollidableA;
		UInt32 m_shapeKeyA;
		hkpCollidable* m_rootCollidableB;
		UInt32 unk30[5];
	};

	UInt32					unk008[2];		// 008
	hkArray<hkpRootCdPoint>		cdPoints;		// 010
	UInt32					unk01C;			// 01C
	hkpRootCdPoint				cdPointsLoc[8];	// 020
};


STATIC_ASSERT(sizeof(hkpAllCdPointCollector) == 0x220);
STATIC_ASSERT(sizeof(hkpAllCdPointCollector::hkpRootCdPoint) == 0x40);


class hkpPhantom : public hkpWorldObject {
public:
	hkpPhantom();
	virtual ~hkpPhantom();

	virtual UInt32			getType() const;
	virtual void			calcAabb(void* aabb);
	virtual void			addOverlappingCollidable(void* collidable);
	virtual bool			isOverlappingCollidableAdded(const void* collidable);
	virtual void			removeOverlappingCollidable(void* collidable);
	virtual hkpPhantom* clone() const;
	virtual void			updateShapeCollectionFilter();
	virtual void			deallocateInternalArrays();

	hkArray<void*>							m_overlapListeners;
	hkArray<hkpPhantomListener*>			m_phantomListeners;
};

STATIC_ASSERT(sizeof(hkpPhantom) == 0x98);

class hkpShapePhantom : public hkpPhantom {
public:
	hkpShapePhantom();
	virtual ~hkpShapePhantom();

	virtual void setPositionAndLinearCast(const hkVector4& position, const void* input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector);
	virtual void getClosestPoints(hkpCdPointCollector& collector, const void* input = nullptr);
	virtual void getPenetrations(void* collector, const void* input = nullptr);

	hkMotionState m_motionState;
};

STATIC_ASSERT(sizeof(hkpShapePhantom) == 0x150);


// 170
class hkpSimpleShapePhantom : public hkpShapePhantom
{
public:
	hkpSimpleShapePhantom();
	virtual ~hkpSimpleShapePhantom();
	struct CollisionDetail {
		  hkpCollidable* m_collidable;
	};
	hkArray<CollisionDetail>		m_collisionDetails;	// 160
};
STATIC_ASSERT(sizeof(hkpSimpleShapePhantom) == 0x160);

// bhk

class bhkRefObject : public NiObject
{
public:
	bhkRefObject();
	virtual ~bhkRefObject();
	/*8C*/virtual void		SetObject(hkReferencedObject* object);
	/*90*/virtual void		Link(bool attach);	// inc/dec ref, depending on arg

	hkReferencedObject* phkObject;		// 08
};
STATIC_ASSERT(sizeof(bhkRefObject) == 0xC);

// 10
class bhkSerializable : public bhkRefObject
{
public:
	bhkSerializable();
	virtual ~bhkSerializable();
	virtual hkpWorld*	GethkWorld();					// 37 | Returns the hkpWorld
	virtual void*	GetbhkWorld();					// 38 | Same as above but null checks this
	virtual bool		Add(void* apWorld);			// 39
	virtual bool		Remove();						// 40
	virtual void		KillCInfo(bool abCreated);		// 41
	virtual UInt32		GetCinfoSize();					// 42
	virtual UInt32		LoadCInfo(void* arStream);	// 43
	virtual void		Init(void*);					// 44
	virtual void*		CreateCInfo(bool*);				// 45
	virtual void*		Kill();							// 46
	virtual void*		KillCInfo2(UInt32);				// 47 | Figure out the name

	void* pInfo;	// 0C - stores hkConstraintData (descriptor used to build hkObj)
};

STATIC_ASSERT(sizeof(bhkSerializable) == 0x10);


// 14
class bhkWorldObject : public bhkSerializable
{
public:
	bhkWorldObject();
	virtual ~bhkWorldObject();

	virtual void	UpdateCollisionFilter();
	virtual void	ForceAdd(void* apWorld);			// 50
	virtual NiNode* CreateDisplayGeometry(NiNode* apNode);	// 51

	UInt32				bodyFlags;		// 10

};
STATIC_ASSERT(sizeof(bhkWorldObject) == 0x14);

class bhkPhantom : public bhkWorldObject {
public:
	bool unk14;
};

STATIC_ASSERT(sizeof(bhkPhantom) == 0x18);
// 14
class bhkShape : public bhkSerializable
{
public:
	virtual void CopyMembers(bhkSerializable* apDestination, void* apCloneProc);							// 49
	virtual bool CalcMass(void* apMassProperty);																		// 50
	virtual void* FindShapeCollection(); 																	// 51
	virtual bool CanShare(void* apCloneProc);																// 52
	virtual void BuildDisplayGeometry(NiNode* apNode, NiColor* apColor);												// 53
	virtual void DestroyDisplayGeometry();																				// 54
	virtual void BuildDisplayFromhkGeometry(NiNode* apNode, void* apGeometry, NiColor* apColor, const char* apName);	// 55

	UInt32			materialType;	// 10
};
STATIC_ASSERT(sizeof(bhkShape) == 0x14);

// 14
class bhkNiCollisionObject : public NiCollisionObject
{
public:
	bhkNiCollisionObject();
	virtual ~bhkNiCollisionObject();

	virtual NiPoint3*	GetVelocity(NiPoint3* outVel);
	virtual void        hktoNiTransform();
	virtual void        NitohkTransform();
	virtual void        ClearVelocities();
	virtual void        SetMotionType(UInt32 aeType, void* apBody, bool abUpdateMotion);
	virtual bool        GetKeyFrame();
	virtual void        SetTransform(NiTransform& akTransform);
	virtual void        SetDebugDisplay(bool abToggle);

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
	UInt16			word0E;			// 0E // NiPointer
	bhkWorldObject* worldObj;		// 10

};

STATIC_ASSERT(sizeof(bhkNiCollisionObject) == 0x14);
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


// 3C8
class bhkCharacterPointCollector : public hkpAllCdPointCollector
{
public:
	UInt32						unk3A0;			// 3A0 
	hkArray<hkpWorldObject*>	contactBodies;	// 3A4
	hkArray<UInt32>				arr3B0;			// 3B0
	hkArray<float>				arr3BC;			// 3BC
};
STATIC_ASSERT(sizeof(bhkCharacterPointCollector) == 0x248);

// 3E0
class bhkCharacterProxy : public bhkSerializable
{
public:
	bhkCharacterPointCollector	pointCollector;		// 10
	UInt32						unk3D8[2];
};
STATIC_ASSERT(sizeof(bhkCharacterProxy) == 0x260);

// 30 TODO
class hkpCharacterContext : public hkReferencedObject
{
public:
	UInt32						unk08;			// 08
	void						*stateMngr;		// 0C
	UInt32						hkState;		// 10
	UInt32						unk14[7];		// 14
};

// 0x4F0 TODO
class bhkCharacterController : public bhkCharacterProxy
{
public:
	hkpCharacterContext	chrContext;
	UInt32 unkC[(0x34C - 0x290) >> 2];
	UInt32 wantState;
	UInt32 unk354[0x38 >> 2];
	hkVector4 velocity;
	hkVector4 kVelocityMod;
	float velocityTime;
	float rotMod;
	float rotModTime;
	float calculatePitchTimer;
	float acrobatics;
	float center;
	float waterHeight;
	float jumpHeight;
	float fallStartHeight;
	float fallTime;
	float gravity;
	float pitchAngle;
	float rollAngle;
	float pitchMult;
	float scale;
	float swimFloatHeight;
	float actorHeight;
	float speedPct;
	UInt32 unk3F8[(0x498 - 0x3F8) >> 2];
	UInt32 unk498;
	hkpRigidBody* spSupportBody; 
	UInt32 unk4A0[(0x4F0 - 0x4A0) >> 2];
};
STATIC_ASSERT(sizeof(bhkCharacterController) == 0x4F0);
STATIC_ASSERT(offsetof(bhkCharacterController, velocity) == 0x390);
STATIC_ASSERT(offsetof(bhkCharacterController, calculatePitchTimer) == 0x3BC);
//STATIC_ASSERT(offsetof(bhkCharacterController, wantState) == 0x350);
