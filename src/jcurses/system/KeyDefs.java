package jcurses.system;

public class KeyDefs {
    public static final char KEY_UP                = 10;
    public static final char KEY_DOWN              = 20;
    public static final char KEY_LEFT              = 30;
    public static final char KEY_RIGHT             = 40;

    //Backspace isn't actually a function key but we are gonna use it
    //as that because the code of this key differs between Linux and Windows
    public static final char KEY_BACKSPACE         = 50;
    public static final char KEY_DELETE            = 60;
    public static final char KEY_HOME              = 70;
    public static final char KEY_END               = 80;
    public static final char KEY_PGUP              = 90;
    public static final char KEY_PGDOWN            = 100;

    public static final int CTYPE_UNKNOWN = 0;
    public static final int CTYPE_PRINTABLE = 1;
    public static final int CTYPE_CONTROL = 2;
    public static final int CTYPE_FUNCTION = 4;
    public static final int CTYPE_ERROR = 8;
}
