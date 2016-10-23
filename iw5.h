#ifndef IW5_H
#define IW5_H

#define MP
//#define SP

// comment both for MW3
// both require x64 configuration
//#define IW6 // need addresses for this
//#define S1

#define MAX_MENUS 0x38E
// 1.4
#ifdef MP
// AW 1.8
	#ifdef S1	// Advance Warfare
		// menu addresses
		#define MENU_POOL		0x0000000142D57FA8
		#define KEYNAMES_ADDR	0x0
		#define OPERATORS_ADDR	0x0000000140AC8690
		// localize addresses
		#define LOCALIZE_POOL	0x0000000142D84C38
	#elif defined IW6	// Ghosts
	#else	// Modern Warfare 3
		#define MENU_POOL		0x148879C
		#define KEYNAMES_ADDR	0x8AEEB8
		#define OPERATORS_ADDR	0x8B2E30
	#endif
#elif defined SP
// AW 1.8
	#ifdef S1	// Advance Warfare
		#define MENU_POOL		0x0000000142D57FA8
		#define KEYNAMES_ADDR	0x0
		#define OPERATORS_ADDR	0x0000000140AC8690
	#elif defined IW6	// Ghosts
	#else	// Modern Warfare 3
		#define MENU_POOL		0xB1CD6C
		#define KEYNAMES_ADDR	0x8AEEB8
		#define OPERATORS_ADDR	0x8531E0
	#endif
#endif

struct Material
{
	const char *name;
};

typedef float vec_t;
typedef vec_t vec4_t[4];
struct menuDef_t;
enum operationEnum;

struct keyname_t
{
	const char *name;
	int keynum;
};

keyname_t *keynames = (keyname_t *)KEYNAMES_ADDR;

struct ItemFloatExpressionEntry
{
	int target;
	const char *s1;
	const char *s2;
};

ItemFloatExpressionEntry itemFloatExpressions[19] = 
{
	{ 0, "rect", "x" },
	{ 1, "rect", "y" },
	{ 2, "rect", "w" },
	{ 3, "rect", "h" },
	{ 4, "forecolor", "r" },
	{ 5, "forecolor", "g" },
	{ 6, "forecolor", "b" },
	{ 7, "forecolor", "rgb" },
	{ 8, "forecolor", "a" },
	{ 9, "glowcolor", "r" },
	{ 10, "glowcolor", "g" },
	{ 11, "glowcolor", "b" },
	{ 12, "glowcolor", "rgb" },
	{ 13, "glowcolor", "a" },
	{ 14, "backcolor", "r" },
	{ 15, "backcolor", "g" },
	{ 16, "backcolor", "b" },
	{ 17, "backcolor", "rgb" },
	{ 18, "backcolor", "a" }
};

const char *d[4] = 
{
	"dvarint( \"%s\" )",
	"dvarbool( \"%s\" )",
	"dvarfloat( \"%s\" )",
	"dvarstring( \"%s\" )"
};

#define ADDED_CMD_COUNT 1
#define COMMAND_COUNT 0x66
const char *commandList[COMMAND_COUNT + ADDED_CMD_COUNT] = 
{
	"fadein",
	"fadeout",
	"show",
	"hide",
	"showMenu",
	"hideMenu",
	"setcolor",
	"open",
	"close",
	"forceClose",
	"escape",
	"closeForAllPlayers",
	"ingameopen",
	"ingameclose",
	"setbackground",
	"setitemcolor",
	"focusfirst",
	"setfocus",
	"setfocusbydvar",
	"setdvar",
	"exec",
	"execfirstclient",
	"execnow",
	"execkeypress",
	"execOnDvarStringValue",
	"execOnDvarIntValue",
	"execOnDvarFloatValue",
	"execNowOnDvarStringValue",
	"execNowOnDvarIntValue",
	"execNowOnDvarFloatValue",
	"play",
	"scriptmenuresponse",
	"scriptMenuRespondOnDvarStringValue",
	"scriptMenuRespondOnDvarIntValue",
	"scriptMenuRespondOnDvarFloatValue",
	"setPlayerData",
	"setPlayerDataSplitScreen",
	"resetStatsConfirm",
	"resetStatsCancel",
	"setGameMode",
	"deleteEliteCacFile",
	"integrateEliteCacFile",
	"setMatchRulesData",
	"loadMatchRulesDataFromPlayer",
	"saveMatchRulesDataToPlayer",
	"loadMatchRulesDataFromHistory",
	"loadMatchRulesDataDefaults",
	"setUsingMatchRulesData",
	"saveMatchRulesDedicatedServer",
	"loadMatchRulesDedicatedServer",
	"onlineVaultEditMetadata",
	"includeInMapRotation",
	"usecustommaprotation",
	"useintermissiontimer",
	"stopintermissiontimer",
	"setLocalVarBool",
	"setLocalVarInt",
	"setLocalVarFloat",
	"setLocalVarString",
	"feederTop",
	"feederBottom",
	"feederPageUp",
	"feederPageDown",
	"showGamerCard",
	"showCoopGamerCard",
	"showSplitscreenGamerCard",
	"reportPlayerOffensive",
	"reportPlayerExploiting",
	"reportPlayerCheating",
	"reportPlayerBoosting",
	"setCardIcon",
	"setCardTitle",
	"setCardIconNew",
	"setCardTitleNew",
	"setCardIconSplitScreen",
	"setCardTitleSplitScreen",
	"setCardIconNewSplitScreen",
	"setCardTitleNewSplitScreen",
	"purchasePrestigeTitle",
	"setProfileItemNew",
	"setProfileItemNewSplitScreen",
	"openforgametype",
	"closeforgametype",
	"storePopupXuid",
	"kickPlayer",
	"getKickPlayerQuestion",
	"partyUpdateMissingMapPackDvar",
	"getHostMigrateQuestion",
	"makehost",
	"togglePlayerMute",
	"resolveError",
	"runCompletionResolve",
	"clearCompletionResolve",
	"execWithResolve",
	"lerp",
	"playMenuVideo",
	"setBackgroundVideo",
	"partyAbortWaiting",
	"clearEntitlementNew",
	"setPastTitleRank",
	"setPastTitlePrestige",
	"anticheat_bancheck",
	// added by me
	"uiScript"
};

const char **g_expOperatorNames = (const char **)OPERATORS_ADDR;

#define ITEM_TYPE_TEXT				0		// simple text
#define ITEM_TYPE_BUTTON			1		// button, basically text with a border
#define ITEM_TYPE_RADIOBUTTON		2		// toggle button, may be grouped
#define ITEM_TYPE_CHECKBOX			3		// check box
#define ITEM_TYPE_EDITFIELD 		4		// editable text, associated with a dvar
#define ITEM_TYPE_COMBO 			5		// drop down list
#define ITEM_TYPE_LISTBOX			6		// scrollable list
#define ITEM_TYPE_MODEL 			7		// model
#define ITEM_TYPE_OWNERDRAW 		8		// owner draw, name specs what it is
#define ITEM_TYPE_NUMERICFIELD		9		// editable text, associated with a dvar
#define ITEM_TYPE_SLIDER			10		// mouse speed, volume, etc.
#define ITEM_TYPE_YESNO 			11		// yes no dvar setting
#define ITEM_TYPE_MULTI 			12		// multiple list setting, enumerated
#define ITEM_TYPE_DVARENUM 			13		// multiple list setting, enumerated from a dvar
#define ITEM_TYPE_BIND				14		// bind
#define ITEM_TYPE_MENUMODEL 		15		// special menu model
#define ITEM_TYPE_VALIDFILEFIELD	16		// text must be valid for use in a dos filename
#define ITEM_TYPE_DECIMALFIELD		17		// editable text, associated with a dvar, which allows decimal input
#define ITEM_TYPE_UPREDITFIELD		18		// editable text, associated with a dvar
#define ITEM_TYPE_GAME_MESSAGE_WINDOW 19	// game message window
#define ITEM_TYPE_NEWSTICKER		20		// horizontal scrollbox
#define ITEM_TYPE_TEXTSCROLL		21		// vertical scrollbox
#define ITEM_TYPE_EMAILFIELD		22
#define ITEM_TYPE_PASSWORDFIELD		23

struct MenuEventHandlerSet;
struct Statement_s;

struct UIFunctionList
{
  int totalFunctions;
  Statement_s **functions;
};
 
struct StaticDvar
{
  /*dvar_t*/
  void *dvar;
  char *dvarName;
};
 
struct StaticDvarList
{
  int numStaticDvars;
  StaticDvar **staticDvars;
};
 
struct StringList
{
  int totalStrings;
  const char **strings;
};
 
struct ExpressionSupportingData
{
  UIFunctionList uifunctions;
  StaticDvarList staticDvarList;
  StringList uiStrings;
};

enum expDataType : int
{
	VAL_INT = 0x0,
	VAL_FLOAT = 0x1,
	VAL_STRING = 0x2,
	VAL_FUNCTION = 0x3,
};

struct ExpressionString
{
	const char *string;
};

union operandInternalDataUnion 
{
	int intVal;
	float floatVal;
	ExpressionString stringVal;
	Statement_s *function;
};

#pragma pack(push, 1)
struct Operand	// x86 size=0x8, x64 size=0xC
{
#ifndef S1 || IW6	// MW3
	expDataType dataType;
#endif
	operandInternalDataUnion internals;	// 0x0
#ifdef S1 || IW6	// Ghosts and AW
	int dataType;	// 0x8
#endif
};
#pragma pack(pop)

union entryInternalData
{
	/*operationEnum*/
	int op;
	Operand operand;
};

/* expressionEntry->type */
#define OPERATOR	0
#define OPERAND		1

struct expressionEntry	// x86 size=0xC, x64 size=0x10
{
#ifndef S1 || IW6
	int type;
#endif
	entryInternalData data;	// 0x0
#ifdef S1 || IW6
	int type;	// 0xC
#endif
};

struct ExpressionPersistentState
{
	int flags;
	int playerDataKey[4];
	int lastExecuteTime[4];
	Operand lastResult[4];
};

struct Statement_s	 // x86 size=0x50, x64 size=0x70
{
	int numEntries;	// 0x0
	expressionEntry *entries;	// 0x8
	ExpressionSupportingData *supportingData;	// 0x10
	ExpressionPersistentState persistentState;	// 0x18
	// 0x6C - 4 byte padding
};

struct SetLocalVarData
{
	const char *localVarName;
	Statement_s *expression;
};

struct ConditionalScript
{
	MenuEventHandlerSet *eventHandlerSet;
	Statement_s *eventExpression;  // loads this first
};

union EventData
{
	const char *unconditionalScript;
	ConditionalScript *conditionalScript;
	MenuEventHandlerSet *elseScript;
	SetLocalVarData *setLocalVarData;
};

enum EventType
{
	EVENT_UNCONDITIONAL = 0x0,
	EVENT_IF = 0x1,
	EVENT_ELSE = 0x2,
	EVENT_SET_LOCAL_VAR_BOOL = 0x3,
	EVENT_SET_LOCAL_VAR_INT = 0x4,
	EVENT_SET_LOCAL_VAR_FLOAT = 0x5,
	EVENT_SET_LOCAL_VAR_STRING = 0x6,
	EVENT_COUNT = 0x7,
};

struct MenuEventHandler
{
	EventData eventData;
	EventType eventType;
};

struct MenuEventHandlerSet
{
	int eventHandlerCount;
	MenuEventHandler **eventHandlers;
};

struct ItemKeyHandler
{
	int key;
	MenuEventHandlerSet *action;
	ItemKeyHandler *next;
};

#pragma pack(push, 4)
struct rectDef_s
{
	float x;
	float y;
	float w;
	float h;
	char horzAlign;
	char vertAlign;
};
#pragma pack(pop)

/* windowDef_t->dynamicFlags */
// 0x1
#define WINDOWDYNAMIC_HASFOCUS		0x00000002
#define WINDOWDYNAMIC_VISIBLE		0x00000004
#define WINDOWDYNAMIC_FADEOUT		0x00000010
#define WINDOWDYNAMIC_FADEIN		0x00000020
// 0x40
// 0x80
#define WINDOWDYNAMIC_CLOSED		0x00000800
// 0x2000
#define WINDOWDYNAMIC_BACKCOLOR		0x00008000
#define WINDOWDYNAMIC_FORECOLOR		0x00010000
 
/* windowDef_t->staticFlags */
#define WINDOWSTATIC_DECORATION				0x00100000
#define WINDOWSTATIC_HORIZONTALSCROLL			0x00200000
#define WINDOWSTATIC_SCREENSPACE				0x00400000
#define WINDOWSTATIC_AUTOWRAPPED				0x00800000
#define WINDOWSTATIC_POPUP						0x01000000
#define WINDOWSTATIC_OUTOFBOUNDSCLICK			0x02000000
#define WINDOWSTATIC_LEGACYSPLITSCREENSCALE	0x04000000
#define WINDOWSTATIC_HIDDENDURINGFLASH			0x10000000
#define WINDOWSTATIC_HIDDENDURINGSCOPE			0x20000000
#define WINDOWSTATIC_HIDDENDURINGUI			0x40000000
#define WINDOWSTATIC_TEXTONLYFOCUS				0x80000000

struct windowDef_t // 0xA4
{
	const char *name;	// 0x00
	rectDef_s rect;
	rectDef_s rectClient;
	char *group;		// 0x2C
	int style;			// 0x30
	int border;			// 0x34
	int ownerDraw;		// 0x38
	int ownerDrawFlags;	// 0x3C
	float borderSize;	// 0x40
	int staticFlags;	// 0x44
	int dynamicFlags;	// 0x48
	int nextTime;		// 0x4C
	float foreColor[4];	// 0x50
	float backColor[4];	// 0x60
	float borderColor[4];// 0x70
	float outlineColor[4];// 0x80
	float disableColor[4];// 0x90
	Material *background;	// 0xA0
};

enum ItemFloatExpressionTarget
{
  ITEM_FLOATEXP_TGT_RECT_X = 0x0,
  ITEM_FLOATEXP_TGT_RECT_Y = 0x1,
  ITEM_FLOATEXP_TGT_RECT_W = 0x2,
  ITEM_FLOATEXP_TGT_RECT_H = 0x3,
  ITEM_FLOATEXP_TGT_FORECOLOR_R = 0x4,
  ITEM_FLOATEXP_TGT_FORECOLOR_G = 0x5,
  ITEM_FLOATEXP_TGT_FORECOLOR_B = 0x6,
  ITEM_FLOATEXP_TGT_FORECOLOR_RGB = 0x7,
  ITEM_FLOATEXP_TGT_FORECOLOR_A = 0x8,
  ITEM_FLOATEXP_TGT_GLOWCOLOR_R = 0x9,
  ITEM_FLOATEXP_TGT_GLOWCOLOR_G = 0xA,
  ITEM_FLOATEXP_TGT_GLOWCOLOR_B = 0xB,
  ITEM_FLOATEXP_TGT_GLOWCOLOR_RGB = 0xC,
  ITEM_FLOATEXP_TGT_GLOWCOLOR_A = 0xD,
  ITEM_FLOATEXP_TGT_BACKCOLOR_R = 0xE,
  ITEM_FLOATEXP_TGT_BACKCOLOR_G = 0xF,
  ITEM_FLOATEXP_TGT_BACKCOLOR_B = 0x10,
  ITEM_FLOATEXP_TGT_BACKCOLOR_RGB = 0x11,
  ITEM_FLOATEXP_TGT_BACKCOLOR_A = 0x12,
  ITEM_FLOATEXP_TGT__COUNT = 0x13,
};

struct ItemFloatExpression
{
	ItemFloatExpressionTarget target;
	Statement_s *expression;
};

struct editFieldDef_s 
{
	float minVal;
	float maxVal;
	float defVal;
	float range;
	int maxChars;
	int maxCharsGotoNext;
	int maxPaintChars;
	int paintOffset;
};

struct multiDef_s	// 0x188
{
	const char *dvarList[32];
	const char *dvarStr[32];
	float dvarValue[32];
	int count; 
	int strDef;
};

struct columnInfo_s
{
  int xpos;
  int ypos;
  int width;
  int height;
  int maxChars;
  int alignment;
};

struct listBoxDef_s	// 0x1C8
{
	// something is wrong here
	int startPos[2];
	int endPos[2];
	float elementWidth;
	float elementHeight;
	int elementStyle;
	int numColumns;
	columnInfo_s columnInfo[16];
	MenuEventHandlerSet *doubleClick;	// 0x1A0
	int notselectable;
	int noscrollbars;
	int usepaging;
	float selectBorder[4];
	Material *selectIcon;		// 0x1C0
	Statement_s *expression;	// 0x1C4
};

struct newsTickerDef_s
{
	int feedId;
	int speed;
	int spacing;
};

struct textScrollDef_s
{
  int startTime;
};

union itemDefData_t
{
	listBoxDef_s *listBox;
	editFieldDef_s *editField;
	newsTickerDef_s *ticker;
	multiDef_s *multiDef;
	const char *enumDvarName;
	textScrollDef_s *scroll;
	void *data;
};

struct itemDef_t 	// 0x180
{
	windowDef_t window;	// 0x00
	rectDef_s textRect;	// 0xA4
	int type;			// 0xB8
	int dataType;		// 0xBC
	int alignment;		// 0xC0
	int fontEnum;		// 0xC4
	int textAlignMode;	// 0xC8
	float textAlignX;	// 0xCC
	float textAlignY;	// 0xD0
	float textScale;	// 0xD4
	int textStyle;		// 0xD8
	int gameMsgWindowIndex;	// 0xDC
	int gameMsgWindowMode;	// 0xE0
	const char *text;	// 0xE4
	int textSaveGameInfo;	// 0xE8
	menuDef_t *parent;	// 0xEC
	MenuEventHandlerSet *mouseEnterText;	// 0xF0
	MenuEventHandlerSet *mouseExitText;	// 0xF4
	MenuEventHandlerSet *mouseEnter;	// 0xF8
	MenuEventHandlerSet *mouseExit;	// 0xFC
	MenuEventHandlerSet *action;	// 0x100
	MenuEventHandlerSet *accept;	// 0x104
	MenuEventHandlerSet *onFocus;	// 0x108
	MenuEventHandlerSet *hasFocus;	// 0x10C
	MenuEventHandlerSet *leaveFocus;	// 0x110
	const char *dvar;	// 0x114
	const char *dvarTest;	// 0x118
	ItemKeyHandler *onKey;	// 0x11C
	const char *enableDvar;	// 0x120
	const char *localVar;	// 0x124
	int dvarFlags;		// 0x128
	const char *focusSound;	// 0x12C
	float special;		// 0x130
	int cursorPos;	// 0x134
	itemDefData_t typeData;	// 0x138
	int floatExpressionCount;	// 0x13C
	ItemFloatExpression *floatExpressions;// 0x140
	Statement_s *visibleExp;// 0x144
	Statement_s *disabledExp;// 0x148
	Statement_s *textExp;// 0x14C
	Statement_s *materialExp;// 0x150
	float glowColor[4];	// 0x154
	bool decayActive;	// 0x164
	int fxBirthTime;	// 0x168
	int fxLetterTime;	// 0x16C
	int fxDecayStartTime;	// 0x170
	int fxDecayDuration;	// 0x174
	int lastSoundPlayedTime;	// 0x178
	Statement_s *textAlignYExp;// 0x17C
};

struct menuTransition	// 0x18
{
  int transitionType;
  int startTime;
  float startVal;
  float endVal;
  float time;
  int endTriggerType;
};

struct menuData_t 
{
	int fullscreen;		// 0x00
	int fadeCycle;		// 0x04
	float fadeClamp;	// 0x08
	float fadeAmount;	// 0x0C
	float fadeInAmount;	// 0x10
	float blurRadius;	// 0x14
	MenuEventHandlerSet *onOpen;		// 0x18
	MenuEventHandlerSet *onRequestClose;// 0x1C
	MenuEventHandlerSet *onClose;		// 0x20
	MenuEventHandlerSet *onEsc;		// 0x24
	MenuEventHandlerSet *onFocusDueToClose;		// 0x28
	ItemKeyHandler *onKey;		// 0x2C
	Statement_s *visibleExp;		// 0x30
	const char *allowedBinding;		// 0x34
	const char *soundName;		// 0x38
	float focusColor[4];		// 0x3C
	Statement_s *rectXExp;		// 0x4C
	Statement_s *rectYExp;		// 0x50
	Statement_s *rectHExp; 	// 0x54
	Statement_s *rectWExp; 	// 0x58
	Statement_s *openSoundExp; 	// 0x5C
	Statement_s *closeSoundExp; 	// 0x60
	Statement_s *soundLoopExp; 	// 0x64
	int cursorItems[1];		// 0x68
	menuTransition scaleTransition[1];	// 0x6C
	menuTransition alphaTransition[1];	// 0x84
	menuTransition xTransition[1];	// 0x9C
	menuTransition yTransition[1];	// 0xB4
	ExpressionSupportingData *localData;// 0xCC loaded first
};

struct menuDef_t	// 0xB0
{
	menuData_t *data;	// 0x00
	windowDef_t window;	// 0x04
	int itemCount;		// 0xA8
	itemDef_t **items;	// 0xAC
};

struct MenuList
{
	const char *name;
	int menuCount;
	menuDef_t **menus;
};

#endif	// IW5_H