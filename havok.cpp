#include "havok.h"
NiNode* hkpEntity::GetParentNode()
{
    for (int i = 0; i < this->m_properties.size; i++)
    {
        hkpProperty p = this->m_properties[i];
        if (IS_TYPE(p.m_value.m_data, bhkNiCollisionObject)) {
            bhkNiCollisionObject* collObj = (bhkNiCollisionObject*)p.m_value.m_data;
            return collObj->m_pkSceneObject ? (NiNode*)collObj->m_pkSceneObject : nullptr;
        }
        else if (IS_TYPE(p.m_value.m_data, NiAVObject)) {
            return (NiNode*)p.m_value.m_data;
        }
    }
    return nullptr;
}
TESObjectREFR* hkpEntity::GetParentRef()
{
    NiNode* node = this->GetParentNode();
    TESObjectREFR* ref = nullptr;
    while (node)
    {
        if (IS_TYPE(node, BSFadeNode))
        {
            BSFadeNode* fadeNode = (BSFadeNode*)node;
            ref = fadeNode->linkedObj;
            if (ref) return ref;
        }
        node = (NiNode*)node->m_parent;
    }
    return ref;
}
