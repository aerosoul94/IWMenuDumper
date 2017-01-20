#include <Windows.h>
#include <stdio.h>
#include <string>

//#define IW4	// MW2
//#define IW5	// MW3.. and above since they're the same..

#if !defined(IW4) && !defined(IW5)
#error Must choose either IW4 or IW5 from above.
#endif

#if defined(IW4) && defined(IW5)
#error Must choose either IW4 or IW5, not both.
#endif

#ifdef IW5
#include "iw5.h"
#endif
#ifdef IW4
#include "iw4.h"
#endif

using namespace std;

const char *va(const char *format, ...)
{
	va_list argptr;
#define MAX_VA_STRING 32000
	static char temp_buffer[MAX_VA_STRING];
	static char string[MAX_VA_STRING];
	static int index = 0;
	char *buf;
	int len;

	va_start(argptr, format);
	vsprintf(temp_buffer, format, argptr);
	va_end(argptr);

	if ((len = strlen(temp_buffer)) >= MAX_VA_STRING)
		printf("");
	if (len + index >= MAX_VA_STRING - 1)
		index = 0;

	buf = &string[index];
	memcpy(buf, temp_buffer, len + 1);

	index += len + 1;

	return buf;
}

const char *Key_KeynumToString(int keynum)
{
	for (keyname_t *kn=keynames; kn->name; kn++)
		if (keynum == kn->keynum)
			return kn->name;
	return NULL;
}

menuDef_t *m = (menuDef_t *)MENU_POOL;

void PrintStatement(FILE *f, Statement_s *e, const char *name, int tabc, bool semiColon = false);
void PrintMenuEventHandlerSet(FILE *f, MenuEventHandlerSet *s, const char *name, int tabc);

char *getTabs(int t)
{
	static char tabs[10];
	for (int i = 0;
		i < t && i < 10; i++)
		*(tabs + i) = '\t';
	*(tabs + t) = 0;
	return tabs;
}

void PrintString(FILE *f, const char *str, const char *name, int tabc)
{
	if (str && *str)
			fprintf(f, "%s%s \"%s\"\n", getTabs(tabc), name, str);
}

#define VEC4(v) (v[0] > 0 || v[1] > 0 || v[2] > 0 || v[3] > 0)

void PrintColor(FILE *file, float *color, const char *name, int tabc)
{
	if (color[0] > 0.0f || color[1] > 0.0f || color[2] > 0.0f || color[3] > 0.0f)
		fprintf(file, "%s%s %g %g %g %g\n", getTabs(tabc), name, color[0], color[1], color[2], color[3]);
}

void PrintRect(FILE *file, rectDef_s rect, const char *name, int tabc)
{
	fprintf(file, "%s%s %g %g %g %g %i %i\n", getTabs(tabc), name, rect.x, rect.y, rect.w, rect.h, rect.horzAlign, rect.vertAlign);
}

void PrintFloat(FILE *file, float value, const char *name, int tabc)
{
	if (value) fprintf(file, "%s%s %g\n", getTabs(tabc), name, value);
}

void PrintInt(FILE *file, int value, const char *name, int tabc)
{
	if (value) fprintf(file, "%s%s %i\n", getTabs(tabc), name, value);
}

void PrintBool(FILE *file, bool value, const char *name, int tabCount)
{
	if (value) fprintf(file, "%s%s\n", getTabs(tabCount), name);
}

// case sensitive
bool replaceAll(std::string& str, const std::string& from, const std::string& to) {
	bool found = false;
	if(from.empty())
		return false;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		found = true;
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return found;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

bool _replace(std::string& str, const std::string& from, const std::string& to) {
	string t1(str);
	string t2(from);
	for (int i = 0; i < from.length(); i++)
		t2[i] = tolower(from[i]);
	for (int i = 0; i < str.length(); i++)
		t1[i] = tolower(str[i]);
	size_t start_pos = t1.find(t2);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

bool _replaceAll(std::string& str, const std::string& from, const std::string& to) {
	while (_replace(str, from, to))
		continue;
	return false;
}

void formatCMD(string& str)
{
	for (int i = 0; i < COMMAND_COUNT; i++)
	{
		string rep("\"");
		rep.append(commandList[i]).append("\" (");
		string tem(commandList[i]);
		tem.append("(");
		_replaceAll(str, rep, tem);
		rep.clear();
		rep.append("\"").append(commandList[i]).append("\"");
		_replaceAll(str, rep, commandList[i]);
		//rep.clear();
		//rep.append(" ").append(commandList[i]);
		//_replaceAll(str, rep, ";\n" + getTabs(tabCount) + commandList[i]);
	}
}

string format(const char *input, int tabc)
{
	string tabs = getTabs(tabc);
	string output(tabs);
	output.append(input).append("\n");	// needed to mark the end of the script
	replace(output, "\t; ", "\t");
	replace(output, (tabs + "\n"), "");
	replace(output, " ; \n", ";\n");
	replace(output, " \n", ";\n");
	replaceAll(output, " ; ", ";\n" + tabs);
	//replaceAll(output, ") ( ) ", ") ");	// not sure why it does this
	formatCMD(output);	// <- NEEDED in order to load them without errors
							// remove all quotes on commands or maybe i should just strip all quotes..
							// if not then strip from commands
	//replaceAll(output, "\"", "");
	_replaceAll(output, "\"self\"", "self");		// unquote self
	_replaceAll(output, "\"forecolor\"", "forecolor");	// unquote forecolor
	_replaceAll(output, "\"backcolor\"", "backcolor");	// unquote backcolor
	_replaceAll(output, "\"bordercolor\"", "bordercolor");	//unquote bordercolor
	return output;
}

string safetext(const char *input)
{
	string output(input);
	_replaceAll(output, "\\", "\\\\");
	return output;
}

void PrintSetLocalVarData(FILE *f, EventType type, SetLocalVarData *e, int tabc)
{
	if ( e )
	{
		char *cmd;
		switch (type)
		{
		case EVENT_SET_LOCAL_VAR_BOOL: cmd = "setLocalVarBool"; break;
		case EVENT_SET_LOCAL_VAR_INT: cmd = "setLocalVarInt"; break;
		case EVENT_SET_LOCAL_VAR_FLOAT: cmd = "setLocalVarFloat"; break;
		case EVENT_SET_LOCAL_VAR_STRING: cmd = "setLocalVarString"; break;
		}

		fprintf(f, "%s%s \"%s\"", getTabs(tabc), cmd, e->localVarName);
		PrintStatement(f, e->expression, NULL, tabc + 1, true);
	}
}

void PrintConditionalScript(FILE *f, ConditionalScript *s, int tabc)
{
	if ( s )
	{
		fprintf(f, "%sif", getTabs(tabc));
		PrintStatement(f, s->eventExpression, NULL, tabc);
		fprintf(f, "%s{\n", getTabs(tabc));
		PrintMenuEventHandlerSet(f, s->eventHandlerSet, NULL, tabc);
		fprintf(f, "%s}\n", getTabs(tabc));
	}
}

void PrintStatement(FILE *f, Statement_s *e, const char *name, int tabc, bool semiColon /*= false*/)
{
	if ( e )
	{
		const bool cleanStatements = true;	// make false to dump raw expression entries
		bool isZType = false;

		if (name) fprintf(f, "%s%s", getTabs(tabc), name);
		if (cleanStatements) 
			if (e->entries[0].data.op != 16/*OP_LEFTPAREN*/) 
				fputs(" (", f);

		for (int i = 0; i < e->numEntries; i++)
		{
			int type = e->entries[i].type;
			if (type == OPERATOR)
			{
				isZType = true;
				int op = e->entries[i].data.op;
				if (!cleanStatements)
					fprintf(f, " %s ", g_expOperatorNames[op]);
				else 
				{
					if (op < 23/*OP_FIRSTFUNCTIONCALL*/)
					{
						// if last op was OP_RIGHTPAREN and this op is OP_LEFTPAREN, skip it
						if (e->entries[i-1].data.op == 1/*OP_RIGHTPAREN*/ && op == 16/*OP_LEFTPAREN*/)
						{
							i++;
							continue;
						}

						// if last op was OP_NOT
						if (e->entries[i-1].data.op == 7/*OP_NOT*/)
							fprintf(f, "%s", g_expOperatorNames[op]);
						else
							fprintf(f, " %s", g_expOperatorNames[op]);
					}
					else if (op <= 26/*OP_STATICDVARSTRING*/ && op >= 23/*OP_STATICDVARINT*/)
					{
						i++;	// go to next op which is the dvar index in staticVars
						char res[256];
						sprintf(res, d[op-23], e->supportingData->staticDvarList.staticDvars[e->entries[i].data.operand.internals.intVal]->dvarName);
						// if previous op was OP_NOT
						if (e->entries[i-2].data.op == 7)
							fprintf(f, "%s", res);	// so it prints !var (instead of ! var)
						else fprintf(f, " %s", res);	// this one has a space to make it look nice
						i++;
					}
					// everything else (commands)
					else
					{
						// if previous op was OP_NOT
						if (e->entries[i-1].data.op == 7/*OP_NOT*/)
							fprintf(f, "%s(", g_expOperatorNames[op]);	// prints !func
						else fprintf(f, " %s(", g_expOperatorNames[op]);	// this one has a space
					}
				}
			}
			else if (type == OPERAND)
			{
				switch (e->entries[i].data.operand.dataType)
				{
				case VAL_INT:
					fprintf(f, " %i", e->entries[i].data.operand.internals.intVal);
					break;
				case VAL_FLOAT:
					fprintf(f, " %g", e->entries[i].data.operand.internals.floatVal);
					break;
				case VAL_STRING:
					fprintf(f, " \"%s\"", e->entries[i].data.operand.internals.stringVal);
					break;
				case VAL_FUNCTION:
					PrintStatement(f, e->entries[i].data.operand.internals.function, NULL, 0);
					break;
				}
			}
		}

		if (cleanStatements) 
		{
			if (isZType) fputs(" )", f);
			if (e->entries[0].data.op != 16/*OP_LEFTPAREN*/) fputs(" )", f);
			if (semiColon) fputs(";", f);
		}

		if (tabc) fputs("\n", f);
	}
}

void PrintMenuEventHandlerSet(FILE *f, MenuEventHandlerSet *s, const char *name, int tabc)
{
	if ( s )
	{
		bool formatStatement = true;
		if (name) fprintf(f, "%s%s\n%s{\n", getTabs(tabc), name, getTabs(tabc));

		for (int e = 0; e < s->eventHandlerCount; e++)
		{
			switch (s->eventHandlers[e]->eventType)
			{
			case EVENT_UNCONDITIONAL:
				if (formatStatement)
					fprintf(f, "%s", format(s->eventHandlers[e]->eventData.unconditionalScript, tabc + 1).c_str());
				else
					fprintf(f, "%s%s\n", getTabs(tabc+1), s->eventHandlers[e]->eventData);
				break;
			case EVENT_IF:
				PrintConditionalScript(f, s->eventHandlers[e]->eventData.conditionalScript, tabc + 1);
				break;
			case EVENT_ELSE:
				PrintMenuEventHandlerSet(f, s->eventHandlers[e]->eventData.elseScript, "else", tabc + 1);
				break;
			case EVENT_SET_LOCAL_VAR_BOOL:
			case EVENT_SET_LOCAL_VAR_INT:
			case EVENT_SET_LOCAL_VAR_FLOAT:
			case EVENT_SET_LOCAL_VAR_STRING:
				PrintSetLocalVarData(f, s->eventHandlers[e]->eventType, s->eventHandlers[e]->eventData.setLocalVarData, tabc + 1);
				break;
			default:
				break;
			}
		}
		
		if (name) fprintf(f, "%s}\n", getTabs(tabc));
	}
}

void PrintItemKeyHandler(FILE *f, ItemKeyHandler *k, const char *name, int tabc)
{
	if ( k )
	{
		char buf[256];
		//sprintf(buf, "%s \"%s\"", name, Key_KeynumToString(k->key));
		sprintf(buf, "%s %i" //"\t// %s"
						,name
						,k->key 
						//, Key_KeynumToString(k->key)
						);
		PrintMenuEventHandlerSet(f, k->action, buf, tabc);
		PrintItemKeyHandler(f, k->next, "execKeyInt", tabc);
	}
}

void PrintItemFloatExpressions(FILE *f, int count, ItemFloatExpression *e, int tabc)
{
	if ( e )
	{
		for (int i = 0; i < count; i++)
		{
			char name[256];
			sprintf(name, "exp %s %s", itemFloatExpressions[e[i].target].s1, itemFloatExpressions[e[i].target].s2);
			PrintStatement(f, e[i].expression, name, tabc);
		}
	}
}

void PrintLBColumns(FILE *f, int n, columnInfo_s *c, int tabc)
{
	fprintf(f, "//%snumcol\t\t\txpos\txwidth\ttextlen\t  alignment\n", getTabs(tabc));
	fprintf(f, "%s%s %i\t\t%i\t\t%i\t\t%i\t\t  %i\n", getTabs(tabc), "columns", n, c[0].xpos, c[0].width, c[0].maxChars, c[0].alignment);
	for (int i = 1; i < n; i++)
		fprintf(f, "%s\t\t\t\t%i\t\t%i\t\t%i\t\t  %i\n", getTabs(tabc), c[i].xpos, c[i].width, c[i].maxChars, c[i].alignment);
}

void DumpListBox(FILE *f, listBoxDef_s *l, int tabc)
{
	PrintColor(f, l->selectBorder, "selectBorder", tabc);
	if (l->selectIcon)
		PrintString(f, l->selectIcon->name, "selectIcon", tabc);
	PrintFloat(f, l->elementWidth, "elementWidth", tabc);
	PrintFloat(f, l->elementHeight, "elementHeight", tabc);
	PrintInt(f, l->elementStyle, "elementtype", tabc);
	PrintLBColumns(f, l->numColumns, l->columnInfo, tabc);
	PrintBool(f, l->noscrollbars, "noscrollbars", tabc);
	PrintBool(f, l->notselectable, "notselectable", tabc);
	PrintBool(f, l->usepaging, "usepaging", tabc);
	PrintMenuEventHandlerSet(f, l->doubleClick, "doubleClick", tabc);
}

void DumpMultiDef(FILE *f, multiDef_s *m, int tabc)
{
	if (m->strDef)
		fprintf(f, "%s%s {", getTabs(tabc), "dvarStrList");
	else
		fprintf(f, "%s%s {", getTabs(tabc), "dvarFloatList");

	for (int i = 0; i < m->count; i++)
	{
		fprintf(f, " \"%s\"", m->dvarList[i]);
		if (m->strDef)
			fprintf(f, " \"%s\"", m->dvarStr[i]);
		else
			fprintf(f, " %g", m->dvarValue[i]);
	}
	fputs(" }\n", f);
}

void PrintItemDefData(FILE *f, int type, itemDefData_t *d, int tabc, const char *dvar = NULL)
{
	if ( d->data )
	{
		switch (type)
		{
		case ITEM_TYPE_LISTBOX:
			DumpListBox(f, d->listBox, tabc);
			break;
		case ITEM_TYPE_MULTI:
			DumpMultiDef(f, d->multiDef, tabc);
			break;
		case ITEM_TYPE_DVARENUM:
			fprintf(f, "%s%s %s %s", getTabs(tabc), "dvar", "dvarEnumList", d->enumDvarName);
			break;
		case ITEM_TYPE_NEWSTICKER:
			PrintInt(f, d->ticker->feedId, "newsfeed", tabc);
			PrintInt(f, d->ticker->speed, "speed", tabc);
			PrintInt(f, d->ticker->spacing, "spacing", tabc);
			break;
		case ITEM_TYPE_TEXTSCROLL:
			break;
		case ITEM_TYPE_EDITFIELD:
		case ITEM_TYPE_NUMERICFIELD:
		case ITEM_TYPE_VALIDFILEFIELD:
		case ITEM_TYPE_UPREDITFIELD:
		case ITEM_TYPE_YESNO:
		case ITEM_TYPE_BIND:
		case ITEM_TYPE_TEXT:
		case ITEM_TYPE_DECIMALFIELD:
		case ITEM_TYPE_EMAILFIELD:
		case ITEM_TYPE_PASSWORDFIELD:
			PrintInt(f, d->editField->maxChars, "maxChars", tabc);
			PrintInt(f, d->editField->maxCharsGotoNext, "maxCharsGotoNext", tabc);
			PrintInt(f, d->editField->maxPaintChars, "maxPaintChars", tabc);
			break;
		case ITEM_TYPE_SLIDER:
			fprintf(f, "%s%s \"%s\" %g %g %g\n", getTabs(tabc), "dvarFloat", dvar, d->editField->defVal, d->editField->minVal, d->editField->maxVal);
			break;
		default:
			break;
		}
	}
}

void PrintDvarTest(FILE *f, const char *dvarTest, const char *enableDvar, int dvarFlags, int tabc)
{ 
	// should actually be (dvarFlags & value) but i've found no dvarFlags with multiple flags set. so meh.
	// the compiler ORS dvarFlags if any of these keywords are found
	char *d;
	switch (dvarFlags)
	{
		case 0x01: d = "enableDvar"; break;
		case 0x02: d = "disableDvar"; break;
		case 0x04: d = "showDvar"; break;
		case 0x08: d = "hideDvar"; break;
		case 0x10: d = "focusDvar"; break;
	}
	PrintString(f, dvarTest, "dvarTest", tabc);
	if (enableDvar)
	{
		fprintf(f, "%s%s { %s}\n", getTabs(tabc), d, enableDvar);
	}
}

void PrintStaticFlags(FILE *f, int s, int tabc)
{
	PrintBool(f, s & WINDOWSTATIC_DECORATION, "decoration", tabc);
	PrintBool(f, s & WINDOWSTATIC_HORIZONTALSCROLL, "horizontalscroll", tabc);
	PrintBool(f, s & WINDOWSTATIC_SCREENSPACE, "screenSpace", tabc);
	PrintBool(f, s & WINDOWSTATIC_AUTOWRAPPED, "autowrapped", tabc);
	PrintBool(f, s & WINDOWSTATIC_POPUP, "popup", tabc);
	PrintBool(f, s & WINDOWSTATIC_OUTOFBOUNDSCLICK, "outOfBoundsClick", tabc);
	PrintBool(f, s & WINDOWSTATIC_LEGACYSPLITSCREENSCALE, "legacySplitScreenScale", tabc);
	PrintBool(f, s & WINDOWSTATIC_HIDDENDURINGFLASH, "hiddenDuringFlashbang", tabc);
	PrintBool(f, s & WINDOWSTATIC_HIDDENDURINGSCOPE, "hiddenDuringScope", tabc);
	PrintBool(f, s & WINDOWSTATIC_HIDDENDURINGUI, "hiddenDuringUI", tabc);
	PrintBool(f, s & WINDOWSTATIC_TEXTONLYFOCUS, "textOnlyFocus", tabc);
}

void PrintDynamicFlags(FILE *f, int d, int tabc)
{
	PrintBool(f, d & WINDOWDYNAMIC_VISIBLE, "visible 1", tabc);
}

void PrintWindow(FILE *f, windowDef_t *w, int tabc)
{
	PrintString(f, w->name, "name", tabc);
	PrintRect(f, w->rect, "rect", tabc);
	PrintStaticFlags(f, w->staticFlags, tabc);
	PrintDynamicFlags(f, w->dynamicFlags, tabc);
	PrintString(f, w->group, "group", tabc);
	PrintInt(f, w->style, "style", tabc);
	PrintInt(f, w->border, "border", tabc);
	PrintFloat(f, w->borderSize, "bordersize", tabc);
	PrintInt(f, w->ownerDraw, "ownerdraw", tabc);
	PrintInt(f, w->ownerDrawFlags, "ownerdrawFlag", tabc);
	PrintColor(f, w->foreColor, "forecolor", tabc);
	PrintColor(f, w->backColor, "backcolor", tabc);
	PrintColor(f, w->borderColor, "bordercolor", tabc);
	PrintColor(f, w->outlineColor, "outlinecolor", tabc);
	PrintColor(f, w->disableColor, "disablecolor", tabc);
	if (w->background)
		PrintString(f, w->background->name, "background", tabc);
}

void DumpItem(FILE *f, itemDef_t *i)
{
	fputs("\t\titemDef\n\t\t{\n", f);

	PrintWindow(f, &i->window, 3);
	// type info
	PrintInt(f, i->type, "type", 3);
	PrintInt(f, i->alignment, "align", 3);
	PrintInt(f, i->fontEnum, "textfont", 3);
	PrintInt(f, i->textAlignMode, "textalign", 3);
	PrintFloat(f, i->textAlignX, "textalignx", 3);
	PrintFloat(f, i->textAlignY, "textaligny", 3);
	PrintFloat(f, i->textScale, "textscale", 3);
	PrintColor(f, i->glowColor, "glowColor", 3);
	PrintBool(f, i->decayActive, "decodeEffect", 3);
	if (i->type == ITEM_TYPE_GAME_MESSAGE_WINDOW)
	{
		PrintInt(f, i->gameMsgWindowIndex, "gamemsgwindowindex", 3);
		PrintInt(f, i->gameMsgWindowMode, "gamemsgwindowmode", 3);
	}
	PrintString(f, i->text, "text", 3);
	PrintBool(f, i->textSaveGameInfo & 1, "textsavegame", 3);
	PrintBool(f, i->textSaveGameInfo & 2, "textcinematicsubtitle", 3);
	PrintFloat(f, i->special, "feeder", 3);
	PrintString(f, i->focusSound, "focusSound", 3);
	if (i->type != ITEM_TYPE_SLIDER && i->type != ITEM_TYPE_DVARENUM)
	{
		PrintString(f, i->dvar, "dvar", 3);
		PrintItemDefData(f, i->type, &i->typeData, 3);
	}
	else PrintItemDefData(f, i->type, &i->typeData, 3);
	PrintDvarTest(f, i->dvarTest, i->enableDvar, i->dvarFlags, 3);
	// expressions
	PrintItemFloatExpressions(f, i->floatExpressionCount, i->floatExpressions, 3);
	PrintStatement(f, i->visibleExp, "visible when", 3);
	PrintStatement(f, i->disabledExp, "disabled when", 3);
	PrintStatement(f, i->textExp, "exp text", 3);
	PrintStatement(f, i->materialExp, "exp material", 3);
#ifdef IW5
	PrintStatement(f, i->textAlignYExp, "exp textAlignY", 3);
#endif
	PrintItemKeyHandler(f, i->onKey, "execKeyInt", 3);
	// statements
	PrintMenuEventHandlerSet(f, i->mouseEnterText, "mouseEnterText", 3);
	PrintMenuEventHandlerSet(f, i->mouseExitText, "mouseExitText", 3);
	PrintMenuEventHandlerSet(f, i->mouseEnter, "mouseEnter", 3);
	PrintMenuEventHandlerSet(f, i->mouseExit, "mouseExit", 3);
	PrintMenuEventHandlerSet(f, i->action, "action", 3);
	PrintMenuEventHandlerSet(f, i->accept, "accept", 3);
	PrintMenuEventHandlerSet(f, i->onFocus, "onFocus", 3);
#ifdef IW5
	PrintMenuEventHandlerSet(f, i->hasFocus, "hasFocus", 3);
#endif
	PrintMenuEventHandlerSet(f, i->leaveFocus, "leaveFocus", 3);

	fputs("\t\t}\n", f);
}

#define FileExists(name) ((fopen(name, "r")) != NULL)

const char *UniqueFile(FILE *l, const char *name, const char *ext)
{
#define MAX_UNIQUE_FILES	20	// just because

	char *path = (char *)malloc(MAX_PATH);
	sprintf(path, "%s%s", name, ext);

	if (!FileExists(path)) return path;

	for (int i = 1; i < MAX_UNIQUE_FILES; ++i)
	{
		if (!FileExists(path))
		{
			return path;
		}

		sprintf(path, "%s (%i)%s", name, i, ext);
	}
	return "wtf";
}

#define   RAW_PATH	"raw/"
#define MENUS_PATH	"raw/menus/"

DWORD WINAPI DumpMenus(LPVOID)
{
	FILE *l = fopen("log.txt", "w");

	CreateDirectoryA(  RAW_PATH, NULL);
	CreateDirectoryA(MENUS_PATH, NULL);

	for (int n = 0; n < MAX_MENUS; n++)
	{
		//if (g_MenuPool[n].next != &g_MenuPool[n+1].entry)
		if (*(uintptr_t *)(&m[n]) != (uintptr_t)(&m[n+1]))
		{
			// Note: does NOT overwrite files, you must delete files everytime you dump
			const char *filename = UniqueFile(l, va(MENUS_PATH "%s", m[n].window.name), ".menu");
			FILE *f = fopen(filename, "w");

			if (f == NULL)
			{
				MessageBoxA(NULL, va("Failed to create file: %s", filename), "ERROR", MB_OK);
				continue;
			}
			
			fprintf(l, "file: %s\n", filename);
			fprintf(l, "addr: %#x\n", (uintptr_t)&m[n]);
			fflush(l);

			fputs("/*********************************************************\n", f);
			fputs("***************** Dumped by aerosoul94 *******************\n", f);
			fputs("*********************************************************/\n\n", f);
			fputs("{\n\tmenuDef\n\t{\n", f);
			fflush(f);

			// menuDef
			PrintWindow(f, &m[n].window, 2);
			fflush(f);

#ifdef IW5
			PrintColor(f, m[n].data->focusColor, "focuscolor", 2);
			PrintInt(f, m[n].data->fullscreen, "fullscreen", 2);
			PrintInt(f, m[n].data->fadeCycle, "fadeCycle", 2);
			PrintFloat(f, m[n].data->fadeClamp, "fadeClamp", 2);
			PrintFloat(f, m[n].data->fadeAmount, "fadeAmount", 2);
			PrintFloat(f, m[n].data->fadeInAmount, "fadeInAmount", 2);
			PrintFloat(f, m[n].data->blurRadius, "blurWorld", 2);
			PrintString(f, m[n].data->allowedBinding, "allowedBinding", 2);
			PrintStatement(f, m[n].data->visibleExp, "visible when", 2);
			PrintStatement(f, m[n].data->rectXExp, "exp rect x", 2);
			PrintStatement(f, m[n].data->rectYExp, "exp rect y", 2);
			PrintStatement(f, m[n].data->rectHExp, "exp rect h", 2);
			PrintStatement(f, m[n].data->rectWExp, "exp rect w", 2);
			PrintStatement(f, m[n].data->openSoundExp, "exp openSound", 2);
			PrintStatement(f, m[n].data->closeSoundExp, "exp closeSound", 2);
			PrintStatement(f, m[n].data->soundLoopExp, "exp soundLoop", 2);
			PrintItemKeyHandler(f, m[n].data->onKey, "execKey", 2);
			PrintMenuEventHandlerSet(f, m[n].data->onOpen, "onOpen", 2);
			PrintMenuEventHandlerSet(f, m[n].data->onRequestClose, "onRequestClose", 2);
			PrintMenuEventHandlerSet(f, m[n].data->onClose, "onClose", 2);
			PrintMenuEventHandlerSet(f, m[n].data->onEsc, "onEsc", 2);
			PrintMenuEventHandlerSet(f, m[n].data->onFocusDueToClose, "onFocusDueToClose", 2);

#elif defined(IW4)
			PrintColor(f, m[n].focusColor, "focuscolor", 2);
			PrintInt(f, m[n].fullscreen, "fullscreen", 2);
			PrintInt(f, m[n].fadeCycle, "fadeCycle", 2);
			PrintFloat(f, m[n].fadeClamp, "fadeClamp", 2);
			PrintFloat(f, m[n].fadeAmount, "fadeAmount", 2);
			PrintFloat(f, m[n].fadeInAmount, "fadeInAmount", 2);
			PrintFloat(f, m[n].blurRadius, "blurWorld", 2);
			PrintString(f, m[n].allowedBinding, "allowedBinding", 2);
			PrintString(f, m[n].soundLoop, "soundLoop", 2);
			PrintStatement(f, m[n].visibleExp, "visible when", 2);
			PrintStatement(f, m[n].rectXExp, "exp rect x", 2);
			PrintStatement(f, m[n].rectYExp, "exp rect y", 2);
			PrintStatement(f, m[n].rectHExp, "exp rect h", 2);
			PrintStatement(f, m[n].rectWExp, "exp rect w", 2);
			PrintStatement(f, m[n].openSoundExp, "exp openSound", 2);
			PrintStatement(f, m[n].closeSoundExp, "exp closeSound", 2);
			PrintItemKeyHandler(f, m[n].onKey, "execKey", 2);
			PrintMenuEventHandlerSet(f, m[n].onOpen, "onOpen", 2);
			PrintMenuEventHandlerSet(f, m[n].onRequestClose, "onRequestClose", 2);
			PrintMenuEventHandlerSet(f, m[n].onClose, "onClose", 2);
			PrintMenuEventHandlerSet(f, m[n].onEsc, "onEsc", 2);
#endif

			// itemDefs
			for (int i = 0; i < m[n].itemCount; i++)
			{
				DumpItem(f, m[n].items[i]);
				fflush(f);
			}

			fputs("menu completed.", l);
			fputs("\t}\n}", f);
			fclose(f);
		}
	}

	MessageBoxA(NULL, "Menu dump complete.", "Finished", MB_OK);
	fclose(l);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hDllHandle, DWORD dwReason, LPVOID lpReserved)
{
	if ( dwReason == DLL_PROCESS_ATTACH )
	{
		CreateThread(0, 0, DumpMenus, 0, 0, 0);
		return true;
	}

	return false;
}
