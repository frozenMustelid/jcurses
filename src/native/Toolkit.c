#define NCURSES_WIDECHAR 1
#include "include/jcurses_system_Toolkit.h"
#include "include/jcurses_system_KeyDefs.h"

#if defined(HAVE_NCURSESW_NCURSES_H)
#include <ncursesw/curses.h>
#elif defined(HAVE_NCURSES_NCURSES_H)
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <wctype.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#define JCURSES_ATTRIBUTES(number,att) (att|COLOR_PAIR(number+1))

FILE * logStream = NULL;

void initLog()
{
    logStream = fopen("njcurses.log","a");
    fprintf(logStream, "native logging initialized!\n");
    fflush(logStream);
}

static int buffer = 1;
static int paintingAction = 0;
static int backspaceKeyCode;

void add_widec(wchar_t ch) {
  wchar_t str[2];
  str[1] = 0;
  str[0] = ch;

  addwstr(str);
}

void echo_widec(wchar_t ch) {
  wchar_t str[2];
  str[1] = 0;
  str[0] = ch;

  addwstr(str);
  refresh();
}

void endPainting()
{
  if(paintingAction <= 0)
    doupdate();
}

void fill_region(int aX, int aY, int aWide, int aHigh, jshort aNumber, jlong aAttr, chtype aCh)
{
    if( aHigh > 0 && aWide > 0 && aX >= 0 && aY >= 0 )
    {
        attrset(JCURSES_ATTRIBUTES(aNumber,aAttr));

        int mY, mX;
        for(mY = aY; mY < (aY + aHigh); mY++)
        {
            move(mY, aX);
            for(mX = aX; mX < (aX + aWide); mX++)
            {
		if(buffer)
              	  addch(aCh);
		else
		{
                  move(mY, mX);
            	  echochar(aCh);
		}
	    }
        }

    }
    endPainting();
}

int mapFunctionKey(unsigned short* code) {
    switch (*code) {
        case KEY_DOWN:
            *code = jcurses_system_KeyDefs_KEY_DOWN;
            break;
        case KEY_UP:
            *code = jcurses_system_KeyDefs_KEY_UP;
            break;
        case KEY_LEFT:
            *code = jcurses_system_KeyDefs_KEY_LEFT;
            break;
        case KEY_RIGHT:
            *code = jcurses_system_KeyDefs_KEY_RIGHT;
            break;
        case KEY_DC:
            *code = jcurses_system_KeyDefs_KEY_DELETE;
            break;
        case KEY_HOME:
            *code = jcurses_system_KeyDefs_KEY_HOME;
            break;
        case KEY_END:
            *code = jcurses_system_KeyDefs_KEY_END;
            break;
        case KEY_NPAGE:
            *code = jcurses_system_KeyDefs_KEY_PGDOWN;
            break;
        case KEY_PPAGE:
            *code = jcurses_system_KeyDefs_KEY_PGUP;
            break;
         default:
            return -1;
    }

    return 0;
}

jint computeChtype(jshort number)
{
    /* Hardcoded NORMAL is fine here - computeChtype only used for pair */
    return JCURSES_ATTRIBUTES(number,A_NORMAL); 
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_adjustColor (JNIEnv *aEnv, jclass aClass, jshort aColorNo, jshort aRed, jshort aGreen, jshort aBlue)
{
    init_color(aColorNo, aRed, aGreen, aBlue);
}


JNIEXPORT jint JNICALL Java_jcurses_system_Toolkit_getScreenWidth (JNIEnv * env, jclass class)
{
    return COLS;
}


JNIEXPORT jint JNICALL Java_jcurses_system_Toolkit_getScreenHeight (JNIEnv * env, jclass class)
{
    return LINES;
}


JNIEXPORT jint JNICALL Java_jcurses_system_Toolkit_hasColorsAsInteger (JNIEnv * env, jclass class)
{
    return has_colors();
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_fillAttributes (JNIEnv * env , jclass class, jlongArray attributes)
{
    jlong attrs[3];
    attrs[0] = A_NORMAL;
    attrs[1] = A_REVERSE;
    attrs[2] = A_BOLD;
    (*env)->SetLongArrayRegion(env,attributes,0,3,attrs);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_fillBasicColors (JNIEnv * env , jclass class, jshortArray basicColors)
{
    short colors[8];

    colors[0] = COLOR_BLACK;
    colors[1] = COLOR_RED;
    colors[2] = COLOR_GREEN;
    colors[3] = COLOR_YELLOW;
    colors[4] = COLOR_BLUE;
    colors[5] = COLOR_MAGENTA;
    colors[6] = COLOR_CYAN;
    colors[7] = COLOR_WHITE;
    (*env)->SetShortArrayRegion(env,basicColors,0,8,colors);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_initColorPair (JNIEnv * env, jclass class, jshort background, jshort foreground, jshort number)
{
    init_pair(number+1,foreground, background);
}


JNIEXPORT jint JNICALL Java_jcurses_system_Toolkit_computeChtype (JNIEnv * env, jclass class, jshort number)
{
    return computeChtype(number);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_init (JNIEnv * env, jclass class)
{
    setlocale(LC_ALL, "");  //Set to any locale (for preventing issues with get_wch localization)
    initscr();              //Init curses screen
    keypad(stdscr, TRUE);   //Set the keypad to TRUE so the function keys are automatically recognized and passed to the program.
    noecho();               //Disable echoing the input.
    raw();                  //Do not generate signals on Ctrl+C, Ctrl+Q, etc: let the program control them by itself.
    nonl();                 //Do not treat the enter as a new line, just pass the key to the program.
    curs_set(0);            //Hide cursor.
    if (has_colors())
        start_color();      //Init colors, if available.

    //TODO Log errors someway
    struct termios tty;
    tcgetattr(0, &tty);
    backspaceKeyCode = tty.c_cc[VERASE];    // Take the terminal backspace key code from the termios struct.
    if (backspaceKeyCode == _POSIX_VDISABLE) {
        backspaceKeyCode = -1; //TODO Notify jcurses about this
    }
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_clearScreen (JNIEnv * env, jclass class, jshort number, jlong attr)
{
    fill_region(0, 0, COLS, LINES, number, attr, ' ' /* getbkgd(stdscr) */);
}


JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawRectangle (JNIEnv * env , jclass class, jint x, jint y, jint width, jint height, jshort number, jlong attr)
{
    fill_region(x, y, width, height, number, attr, ' ' /* getbkgd(stdscr) */);
}


JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_shutdown (JNIEnv * env, jclass class)
{
    endwin();
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawHorizontalLine (JNIEnv * env, jclass class, jint aX, jint aY ,jint aEndX, jshort aNumber, jlong aAttr)
{
    //mvhline(aY, aX, ACS_HLINE, aEndX-aX+1);
    fill_region(aX, aY, aEndX-aX+1, 1, aNumber, aAttr, ACS_HLINE);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawVerticalLine (JNIEnv * env, jclass class, jint aX, jint aY ,jint aEndY, jshort aNumber, jlong aAttr)
{
    //mvvline(aY, aX, ACS_VLINE, aEndY-aY+1);
    fill_region(aX, aY, 1, aEndY-aY+1, aNumber, aAttr, ACS_VLINE);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawHorizontalThickLine (JNIEnv * env, jclass class, jint aX, jint aY ,jint aEndX, jshort aNumber, jlong aAttr)
{
    //mvhline(aY, aX, ACS_CKBOARD, aEndX-aX+1);
    fill_region(aX, aY, aEndX-aX+1, 1, aNumber, aAttr, ACS_CKBOARD);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawVerticalThickLine (JNIEnv * env, jclass class, jint aX, jint aY ,jint aEndY, jshort aNumber, jlong aAttr)
{
    //mvvline(aY, aX, ACS_CKBOARD, aEndY-aY+1);
    fill_region(aX, aY, 1, aEndY-aY+1, aNumber, aAttr, ACS_CKBOARD);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_drawCorner (JNIEnv *env, jclass class, jint aX, jint aY, jint aPos, jshort number, jlong attr)
{
    chtype ch;

    switch( aPos )
    {
    case jcurses_system_Toolkit_LL_CORNER:
        ch = ACS_LLCORNER;
        break;

    case jcurses_system_Toolkit_LR_CORNER:
        ch = ACS_LRCORNER;
        break;

    case jcurses_system_Toolkit_UL_CORNER:
        ch = ACS_ULCORNER;
        break;

    case jcurses_system_Toolkit_UR_CORNER:
        ch = ACS_URCORNER;
        break;

    default:
        return;
    }

    attrset(JCURSES_ATTRIBUTES(number,attr));
    move(aY, aX);
	
    if(buffer)
      addch(ch);
    else
      echochar(ch);
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_printString (JNIEnv * env, jclass class, jcharArray chars, jint x, jint y, jint width, jint height, jshort number, jlong attr)
{
    int j=0;
    int xpos,ypos;
    int length = (*env)->GetArrayLength(env,chars);
    wchar_t c;
    unsigned short * charArray = (*env)->GetCharArrayElements(env, chars, NULL);

    attrset(JCURSES_ATTRIBUTES(number,attr));

    move(y, x);
    xpos=x-1;
    ypos=y;

    for( j=0; j<length; j++ )
    {
        //TODO Support for multi-wide characters
        c = charArray[j];

        if( c =='\r' )
            continue;

        xpos++;
        if( (xpos >= x+width) || (c=='\n') )
        {
            xpos = x;
            ypos++;
            move(ypos, xpos);
            if( ypos == y+height )
                break;
        }

        if( c == '\n' )
            continue;

        if( c == '\t' )
            c = ' ';

	if(buffer)
          add_widec(c);
	else
          echo_widec(c);
    }

    endPainting();
}


JNIEXPORT jint JNICALL Java_jcurses_system_Toolkit_readByte (JNIEnv * env, jclass class)
{
    wint_t c;
    unsigned short ch;
    int r = get_wch(&c);
    ch = c & 0xffff;


    if (ch == 27) { //Meta key
        nodelay(stdscr, TRUE); // Next get_wch will be non-blocking

        r = get_wch(&c);
        ch = c & 0xffff;

        nodelay(stdscr, FALSE); // Return to default state

        if (r == ERR) { //No key was waiting to be read. Escape is pressed (?)
            r = KEY_CODE_YES;
            ch = 27; //Escape key
        }

        // Currently we're not returning that the meta key was pressed.
    }

    if (r != ERR && ch == backspaceKeyCode) {
        return (jcurses_system_KeyDefs_CTYPE_FUNCTION << 24) | jcurses_system_KeyDefs_KEY_BACKSPACE;
    }

    switch (r) {
        case OK:
            if (ch < 256 && iswcntrl(ch)) {
                r = jcurses_system_KeyDefs_CTYPE_CONTROL;
            } else if (iswprint(ch)) {
                r = jcurses_system_KeyDefs_CTYPE_PRINTABLE;
            } else {
                r = jcurses_system_KeyDefs_CTYPE_UNKNOWN;
            }
            break;
        case KEY_CODE_YES:
            if (mapFunctionKey(&ch) == 0) {
                r = jcurses_system_KeyDefs_CTYPE_FUNCTION;
            } else {
                r = jcurses_system_KeyDefs_CTYPE_UNKNOWN;
            }
            break;
        case ERR:
            r = jcurses_system_KeyDefs_CTYPE_ERROR;
            break;

    }

    return (r << 24) | ch;
}

JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_changeColors (JNIEnv *env, jclass clazz, jint x, jint y, jint width, jint height, jshort colorpair, jlong attr)
{
    int i,j;
    chtype currentChar = 0;
    attrset(JCURSES_ATTRIBUTES(colorpair,attr));
    for( i=0; i<width; i++ )
    {
        for( j=0;j<height; j++ )
        {
            currentChar = (mvinch(y+j,x+i) & A_CHARTEXT);
	    move(y, x);
      	    if(buffer)
              addch(currentChar);
	    else
              echochar(currentChar);
        }
    }
    endPainting();
}


JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_startPainting (JNIEnv * env, jclass clazz)
{
    paintingAction++;
}


JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_endPainting (JNIEnv * env , jclass clazz)
{
    if( paintingAction > 0 )
      paintingAction--;

    endPainting();
}


JNIEXPORT void JNICALL Java_jcurses_system_Toolkit_beep (JNIEnv * env, jclass clazz)
{
    beep();
}
