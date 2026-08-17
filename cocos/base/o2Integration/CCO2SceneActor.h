//@CODETOOL_NON_EXCLUDE
#pragma once

#include "o2/Scene/Actor.h"

namespace cocos2d
{
	class Director;
	class Scene;
}

// ---------------------------------------------------------------------------
// Service actor: the entry point of the cocos2d integration inside an o2
// scene. Manages the cocos side lifecycle, draws the whole cocos scene as a
// single scene drawable — so it supports o2 drawing order (depth) and layers —
// and exposes the cocos node hierarchy to the editor through editable children
// ---------------------------------------------------------------------------
class O2CocosSceneActor : public o2::Actor
{
public:
	// Constructor. Registers itself as the integration instance
	O2CocosSceneActor(o2::RefCounter* refCounter);

	// Destructor
	~O2CocosSceneActor();

	// Returns the integration instance, if created
	static O2CocosSceneActor* Instance();

	// Returns cocos director — the interaction point with the cocos side
	cocos2d::Director* GetDirector() const;

	// Pumps the cocos scheduler; called by the o2 scene only in play mode, so the
	// hosted cocos world advances exactly when the editor plays (and respects the
	// editor game speed through dt)
	void Update(float dt) override;

	// Returns the actor children plus the running cocos scene root
	o2::Vector<o2::Ref<o2::SceneEditableObject>> GetEditableChildren() const override;

	// Returns the basis mapping the cocos design-resolution box (in points) into
	// o2 world space; refreshed on every draw, used for editor node transforms
	static const o2::Basis& GetCocosToWorldBasis();

	// Returns true once the cocos→world mapping has been computed (after first draw)
	static bool IsMappingReady();

	// Touch conversion phase for input forwarded from o2 listeners
	enum class TouchPhase { Began, Moved, Ended };

	// Converts an o2 game-world point into cocos view coordinates and injects a
	// touch into the cocos GLView; entry point for node cursor listeners
	static void ForwardTouchToCocos(const o2::Vec2F& worldPoint, TouchPhase phase);

	SERIALIZABLE(O2CocosSceneActor);

protected:
	// Draws the whole cocos scene in one pass at this actor's layer/depth slot
	void OnDraw() override;

	// Registers interactive cocos nodes as cursor areas in the game camera's
	// listeners layer; game view pass only, and only while the editor plays
	void RegisterInteractiveNodes();

private:
	static O2CocosSceneActor* sInstance;
	static o2::Basis sCocosToWorld; // design box (unit) → o2 world, refreshed in OnDraw
	static bool sMappingReady;      // sCocosToWorld has been computed at least once

	cocos2d::Director* mDirector = nullptr; // @IGNORE — raw cocos pointer, invisible to o2 reflection
	bool mFrameInitialized = false;         // @IGNORE — actor frame set to the design box once
};
// --- META ---

PRE_ENUM_META(O2CocosSceneActor::TouchPhase);

CLASS_BASES_META(O2CocosSceneActor)
{
    BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(O2CocosSceneActor)
{
}
END_META;
CLASS_METHODS_META(O2CocosSceneActor)
{

    FUNCTION().PUBLIC().SIGNATURE_STATIC(O2CocosSceneActor*, Instance);
    FUNCTION().PUBLIC().SIGNATURE(cocos2d::Director*, GetDirector);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(o2::Vector<o2::Ref<o2::SceneEditableObject>>, GetEditableChildren);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const o2::Basis&, GetCocosToWorldBasis);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsMappingReady);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(void, ForwardTouchToCocos, const o2::Vec2F&, TouchPhase);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDraw);
    FUNCTION().PROTECTED().SIGNATURE(void, RegisterInteractiveNodes);
}
END_META;
// --- END META ---
