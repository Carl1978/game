//  Constants.h

#ifndef Game_Constants_h
#define Game_Constants_h

// Bridge size
#define PHYSICS_CARD_WIDTH				0.0572f		//57.15mm
#define PHYSICS_CARD_HEIGHT				0.0889f		//88.9mm
#define PHYSICS_CARD_THICKNESS			0.001f		//1.0mm

// Domino brick size (Note: x10 scale)
#define DOMINO_WIDTH					0.02f
#define DOMINO_HEIGHT					0.03f
#define DOMINO_THICKNESS				0.008f

// Table Size
#define TABLE_WIDTH						1.0f
#define TABLE_HEIGHT					1.0f
#define TABLE_THICKNESS					0.05f

//#define PHYSICS_DOMINO_WIDTH				0.169f		//1.69 cm
//#define PHYSICS_DOMINO_HEIGHT				0.254f		//2.54 cm => 1 Inch
//#define PHYSICS_DOMINO_THICKNESS			0.085f		//0.85 cm

#define EVENT_CUSTOM_TOUCHCARD				"event_custom_touchcard"
#define EVENT_CUSTOM_TOUCHSPRITE			"event_custom_touchsprite"
#define EVENT_CUSTOM_PROCESS				"event_custom_process"
#define EVENT_CUSTOM_RESETGAME				"event_custom_resetgame"
#define EVENT_CUSTOM_RESETGAMEPATIENCE		"event_custom_resetgamepatience"
#define EVENT_CUSTOM_RESETPANEL				"event_custom_resetpanel"
#define EVENT_CUSTOM_POSTINIT				"event_custom_postinit"
#define EVENT_CUSTOM_SPAWNCARD				"event_custom_spawncard"
#define EVENT_CUSTOM_DEBUG3DPHYSICS			"event_custom_debug3dphysics"
#define EVENT_CUSTOM_DRAGSPRITEPOS			"event_custom_dragspritepos"
#define EVENT_CUSTOM_DRAGPHYSICSSPRITEPOS	"event_custom_dragphysicsspritepos"
#define EVENT_CUSTOM_TOUCHEDNOTHING			"event_custom_touchednothing"
#define EVENT_CUSTOM_TOUCHEDSOMETHING		"event_custom_touchedsomething"
#define EVENT_CUSTOM_UNDO					"event_custom_undo"
#define EVENT_CUSTOM_MOVED					"event_custom_moved" // TODO: we should have a game state packet of data i.e. EVENT_CUSTOM_UPDATE
#define EVENT_CUSTOM_DIALOGUE_CLOSE			"event_custom_dialogue_close"
#define EVENT_CUSTOM_DIALOGUE_OPEN			"event_custom_dialogue_open"
#define EVENT_CUSTOM_GAME_COMPLETE			"event_custom_game_complete"
#define EVENT_CUSTOM_MSG_SEND				"event_custom_msg_send"

enum LabelFooter {
	ELabelFooter_score = 0,
	ELabelFooter_time,
	ELabelFooter_moves
};

enum ButtonFooter {
	EButtonFooter_play = 0,
	EButtonFooter_undo,
	EButtonFooter_hint
};

#define kBackgroundWidth	1024.0f
#define kBackgroundHeight	518.0f

#define kFontFilePath		"fonts/arial.ttf"

#define kViewBlackJackGame		1
#define kViewKingsAndJacks		2
#define kViewPatience			3
#define kViewSnap				4
#define kViewMatch				5
#define kViewFrogGame			6
#define kViewGame				7
#define kViewSnake				8
#define kViewHello				9
#define kViewMapTap				10

#define kViewBackground			100
#define kViewPanel				101
#define kViewSplash				102
#define kViewTitle				103
#define kViewCredits			104
#define kViewEnd				105
#define kViewLobby				106
#define kViewTable				107
#define kViewSnookerGame		108
#define kViewTest				109
#define kViewBiomess			110

#endif
