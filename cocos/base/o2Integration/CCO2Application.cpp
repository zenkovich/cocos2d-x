// Win32: Ensure stable WinSock2/Windows include order before any other headers pull them in.
// This prevents winsock type redefinition errors and macro collisions in headers included later.
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <WinSock2.h>
#include <Windows.h>
#endif

#include "platform/CCGL.h"
#include "CCO2Application.h"

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventMouse.h"
#include "platform/CCGLView.h"

#include "o2/Application/Input.h"
#include "o2/Utils/System/Time/Time.h"

#include <unordered_map>

namespace
{
	// Map O2 Virtual Key Codes to Cocos2d-x KeyCodes
	cocos2d::EventKeyboard::KeyCode MapO2KeyToCocos(o2::KeyboardKey o2Key)
	{
		using KeyCode = cocos2d::EventKeyboard::KeyCode;
		
		static std::unordered_map<o2::KeyboardKey, KeyCode> keyMap = {
			// Function keys
			{ VK_F1, KeyCode::KEY_F1 },
			{ VK_F2, KeyCode::KEY_F2 },
			{ VK_F3, KeyCode::KEY_F3 },
			{ VK_F4, KeyCode::KEY_F4 },
			{ VK_F5, KeyCode::KEY_F5 },
			{ VK_F6, KeyCode::KEY_F6 },
			{ VK_F7, KeyCode::KEY_F7 },
			{ VK_F8, KeyCode::KEY_F8 },
			{ VK_F9, KeyCode::KEY_F9 },
			{ VK_F10, KeyCode::KEY_F10 },
			{ VK_F11, KeyCode::KEY_F11 },
			{ VK_F12, KeyCode::KEY_F12 },

			// Special keys
			{ VK_ESCAPE, KeyCode::KEY_ESCAPE },
			{ VK_BACK, KeyCode::KEY_BACKSPACE },
			{ VK_TAB, KeyCode::KEY_TAB },
			{ VK_RETURN, KeyCode::KEY_RETURN },
			{ VK_SPACE, KeyCode::KEY_SPACE },
			{ VK_CAPITAL, KeyCode::KEY_CAPS_LOCK },
			{ VK_SHIFT, KeyCode::KEY_SHIFT },
			{ VK_CONTROL, KeyCode::KEY_CTRL },
			{ VK_MENU, KeyCode::KEY_ALT },
			{ VK_PAUSE, KeyCode::KEY_PAUSE },
			{ VK_SCROLL, KeyCode::KEY_SCROLL_LOCK },

			// Navigation keys
			{ VK_INSERT, KeyCode::KEY_INSERT },
			{ VK_DELETE, KeyCode::KEY_DELETE },
			{ VK_HOME, KeyCode::KEY_HOME },
			{ VK_END, KeyCode::KEY_END },
			{ VK_PRIOR, KeyCode::KEY_PG_UP },
			{ VK_NEXT, KeyCode::KEY_PG_DOWN },
			{ VK_LEFT, KeyCode::KEY_LEFT_ARROW },
			{ VK_RIGHT, KeyCode::KEY_RIGHT_ARROW },
			{ VK_UP, KeyCode::KEY_UP_ARROW },
			{ VK_DOWN, KeyCode::KEY_DOWN_ARROW },

			// Number keys
			{ '0', KeyCode::KEY_0 },
			{ '1', KeyCode::KEY_1 },
			{ '2', KeyCode::KEY_2 },
			{ '3', KeyCode::KEY_3 },
			{ '4', KeyCode::KEY_4 },
			{ '5', KeyCode::KEY_5 },
			{ '6', KeyCode::KEY_6 },
			{ '7', KeyCode::KEY_7 },
			{ '8', KeyCode::KEY_8 },
			{ '9', KeyCode::KEY_9 },

			// Letter keys
			{ 'A', KeyCode::KEY_A },
			{ 'B', KeyCode::KEY_B },
			{ 'C', KeyCode::KEY_C },
			{ 'D', KeyCode::KEY_D },
			{ 'E', KeyCode::KEY_E },
			{ 'F', KeyCode::KEY_F },
			{ 'G', KeyCode::KEY_G },
			{ 'H', KeyCode::KEY_H },
			{ 'I', KeyCode::KEY_I },
			{ 'J', KeyCode::KEY_J },
			{ 'K', KeyCode::KEY_K },
			{ 'L', KeyCode::KEY_L },
			{ 'M', KeyCode::KEY_M },
			{ 'N', KeyCode::KEY_N },
			{ 'O', KeyCode::KEY_O },
			{ 'P', KeyCode::KEY_P },
			{ 'Q', KeyCode::KEY_Q },
			{ 'R', KeyCode::KEY_R },
			{ 'S', KeyCode::KEY_S },
			{ 'T', KeyCode::KEY_T },
			{ 'U', KeyCode::KEY_U },
			{ 'V', KeyCode::KEY_V },
			{ 'W', KeyCode::KEY_W },
			{ 'X', KeyCode::KEY_X },
			{ 'Y', KeyCode::KEY_Y },
			{ 'Z', KeyCode::KEY_Z },

			// Numpad keys
			{ VK_NUMPAD0, KeyCode::KEY_KP_INSERT },
			{ VK_NUMPAD1, KeyCode::KEY_KP_END },
			{ VK_NUMPAD2, KeyCode::KEY_KP_DOWN },
			{ VK_NUMPAD3, KeyCode::KEY_KP_PG_DOWN },
			{ VK_NUMPAD4, KeyCode::KEY_KP_LEFT },
			{ VK_NUMPAD5, KeyCode::KEY_KP_FIVE },
			{ VK_NUMPAD6, KeyCode::KEY_KP_RIGHT },
			{ VK_NUMPAD7, KeyCode::KEY_KP_HOME },
			{ VK_NUMPAD8, KeyCode::KEY_KP_UP },
			{ VK_NUMPAD9, KeyCode::KEY_KP_PG_UP },
			{ VK_MULTIPLY, KeyCode::KEY_KP_MULTIPLY },
			{ VK_ADD, KeyCode::KEY_KP_PLUS },
			{ VK_SUBTRACT, KeyCode::KEY_KP_MINUS },
			{ VK_DECIMAL, KeyCode::KEY_KP_DELETE },
			{ VK_DIVIDE, KeyCode::KEY_KP_DIVIDE },

			// Punctuation
			{ VK_OEM_MINUS, KeyCode::KEY_MINUS },
			{ VK_OEM_PLUS, KeyCode::KEY_EQUAL },
			{ VK_OEM_4, KeyCode::KEY_LEFT_BRACKET },
			{ VK_OEM_6, KeyCode::KEY_RIGHT_BRACKET },
			{ VK_OEM_1, KeyCode::KEY_SEMICOLON },
			{ VK_OEM_7, KeyCode::KEY_APOSTROPHE },
			{ VK_OEM_3, KeyCode::KEY_GRAVE },
			{ VK_OEM_5, KeyCode::KEY_BACK_SLASH },
			{ VK_OEM_COMMA, KeyCode::KEY_COMMA },
			{ VK_OEM_PERIOD, KeyCode::KEY_PERIOD },
			{ VK_OEM_2, KeyCode::KEY_SLASH },
		};

		auto it = keyMap.find(o2Key);
		if (it != keyMap.end())
			return it->second;

		return KeyCode::KEY_NONE;
	}
}

class O2GLView : public cocos2d::GLView
{
public:
	O2GLView(CocosO2Application& app):
		mApplication(app)
	{
		_designResolutionSize.width = app.GetContentSize().x;
		_designResolutionSize.height = app.GetContentSize().y;
	}

	void end() override {}
	bool isOpenGLReady() override { return true; }
	void swapBuffers() override {}
	void setIMEKeyboardState(bool open) override {}
	HWND getWin32Window() override { return mApplication.GetWindowHandle(); }

protected:
	CocosO2Application& mApplication;
};

CocosO2Application::CocosO2Application(o2::RefCounter* refCounter) :
	o2::Application(refCounter)
{
}

CocosO2Application::~CocosO2Application()
{
}

void CocosO2Application::BasicInitialize()
{
	o2::Application::BasicInitialize();

	mCocosDirector = cocos2d::Director::getInstance();
	mCocosGLView = new O2GLView(*this);
	mCocosDirector->setOpenGLView(mCocosGLView);
}

void CocosO2Application::Deinitialize()
{
	o2::Application::Deinitialize();
}

void CocosO2Application::ProcessFrame()
{
	float dt = o2Time.GetDeltaTime();

	// Process input events before Cocos update
	ProcessInputEvents();

	PreCocosUpdate(dt);

	o2::Application::ProcessFrame();

	PostCocosUpdate(dt);
}

void CocosO2Application::PreCocosUpdate(float dt)
{
	mCocosDirector->mainLoop();
}

void CocosO2Application::PostCocosUpdate(float dt)
{
}

void CocosO2Application::OnStarted()
{
	o2::Application::OnStarted();
}

void CocosO2Application::OnClosing()
{
	o2::Application::OnClosing();
}

void CocosO2Application::ProcessInputEvents()
{
	if (!mCocosDirector || !mCocosGLView)
		return;

	auto eventDispatcher = mCocosDirector->getEventDispatcher();
	if (!eventDispatcher)
		return;

	// Get viewport and scale for coordinate transformation
	const auto& viewport = mCocosGLView->getViewPortRect();
	float scaleX = mCocosGLView->getScaleX();
	float scaleY = mCocosGLView->getScaleY();

	// Process keyboard events
	auto& pressedKeys = o2Input.GetPressedKeys();
	for (const auto& key : pressedKeys)
	{
		auto cocosKey = MapO2KeyToCocos(key.keyCode);
		if (cocosKey != cocos2d::EventKeyboard::KeyCode::KEY_NONE)
		{
			cocos2d::EventKeyboard event(cocosKey, true);
			eventDispatcher->dispatchEvent(&event);
		}
	}

	auto& releasedKeys = o2Input.GetReleasedKeys();
	for (const auto& key : releasedKeys)
	{
		auto cocosKey = MapO2KeyToCocos(key.keyCode);
		if (cocosKey != cocos2d::EventKeyboard::KeyCode::KEY_NONE)
		{
			cocos2d::EventKeyboard event(cocosKey, false);
			eventDispatcher->dispatchEvent(&event);
		}
	}

	// Get current cursor state
	auto cursor = o2Input.GetCursor();
	if (cursor)
	{
		// Convert O2 coordinates to Cocos coordinates
		o2::Vec2F cocosPos = O2ToCocosCoords(cursor->position);
		float cocosX = cocosPos.x;
		float cocosY = cocosPos.y;

		// Track mouse position for touch events
		mLastMouseX = cocosX;
		mLastMouseY = cocosY;

		// Handle left mouse button (primary touch)
		bool leftPressed = o2Input.IsCursorPressed();
		bool leftDown = o2Input.IsCursorDown();
		bool leftReleased = o2Input.IsCursorReleased();

		if (leftPressed)
		{
			mLeftButtonPressed = true;
			
			// Dispatch touch event
			intptr_t id = 0;
			mCocosGLView->handleTouchesBegin(1, &id, &cocosX, &cocosY);

			// Dispatch mouse event
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_DOWN);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_LEFT);
			eventDispatcher->dispatchEvent(&event);
		}
		else if (leftReleased)
		{
			mLeftButtonPressed = false;
			
			// Dispatch touch event
			intptr_t id = 0;
			mCocosGLView->handleTouchesEnd(1, &id, &cocosX, &cocosY);

			// Dispatch mouse event
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_UP);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_LEFT);
			eventDispatcher->dispatchEvent(&event);
		}
		else if (leftDown && mLeftButtonPressed)
		{
			// Dispatch touch move event
			intptr_t id = 0;
			mCocosGLView->handleTouchesMove(1, &id, &cocosX, &cocosY);
		}

		// Handle right mouse button
		bool rightPressed = o2Input.IsRightMousePressed();
		bool rightReleased = o2Input.IsRightMouseReleased();

		if (rightPressed)
		{
			mRightButtonPressed = true;
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_DOWN);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_RIGHT);
			eventDispatcher->dispatchEvent(&event);
		}
		else if (rightReleased)
		{
			mRightButtonPressed = false;
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_UP);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_RIGHT);
			eventDispatcher->dispatchEvent(&event);
		}

		// Handle middle mouse button
		bool middlePressed = o2Input.IsMiddleMousePressed();
		bool middleReleased = o2Input.IsMiddleMouseReleased();

		if (middlePressed)
		{
			mMiddleButtonPressed = true;
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_DOWN);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_MIDDLE);
			eventDispatcher->dispatchEvent(&event);
		}
		else if (middleReleased)
		{
			mMiddleButtonPressed = false;
			cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_UP);
			event.setCursorPosition(cocosX, cocosY);
			event.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_MIDDLE);
			eventDispatcher->dispatchEvent(&event);
		}

		// Always dispatch mouse move event
		cocos2d::EventMouse moveEvent(cocos2d::EventMouse::MouseEventType::MOUSE_MOVE);
		moveEvent.setCursorPosition(cocosX, cocosY);
		
		// Set current button state
		if (mLeftButtonPressed)
			moveEvent.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_LEFT);
		else if (mRightButtonPressed)
			moveEvent.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_RIGHT);
		else if (mMiddleButtonPressed)
			moveEvent.setMouseButton(cocos2d::EventMouse::MouseButton::BUTTON_MIDDLE);
		
		eventDispatcher->dispatchEvent(&moveEvent);
	}

	// Handle mouse wheel
	float wheelDelta = o2Input.GetMouseWheelDelta();
	if (wheelDelta != 0.0f)
	{
		cocos2d::EventMouse event(cocos2d::EventMouse::MouseEventType::MOUSE_SCROLL);
		event.setCursorPosition(mLastMouseX, mLastMouseY);
		// Normalize wheel delta (O2 uses raw values, Cocos expects normalized)
		event.setScrollData(0.0f, wheelDelta / 120.0f);
		eventDispatcher->dispatchEvent(&event);
	}
}

o2::Vec2F CocosO2Application::O2ToCocosCoords(const o2::Vec2F& o2Pos) const
{
	// O2 coordinate system (from Input::Cursor): top-left = (0, 0), Y axis down
	// O2 rendering coordinate system: center = (0, 0), Y axis up
	// Cocos coordinate system: bottom-left = (0, 0), Y axis up
	
	// Get content size
	o2::Vec2F contentSize = GetContentSize();
	
	// Transform from O2 Input coords to Cocos coords:
	// O2 X: [0, width] -> Cocos X: [0, width]
	// O2 Y: [0, height] (down) -> Cocos Y: [0, height] (up)
	
	float cocosX = o2Pos.x;
	float cocosY = contentSize.y - o2Pos.y; // Invert Y axis
	
	return o2::Vec2F(cocosX, cocosY);
}


