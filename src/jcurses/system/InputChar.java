
package jcurses.system;

import java.util.Objects;

/**
 * The instances of this class represent characters or key codes, that are input by an user. An instance of the class contains einther a ascii character or one
 * of in the class declared constants for function keys and control keys.
 */
public class InputChar
{
    /* Function are the special keys of the keyboard that have
       an special meaning (such as arrows, pgup, pgdown, etc).
       Since these keys might change between platforms, these have its own
       identifier set by jcurses, that must be mapped with the real key
       internally by the native library. This means that the identifiers
       of these case has no other sense but identifying the key.
     */
    public static class Function {
        public static final InputChar KEY_UP                = fkey(KeyDefs.KEY_UP);
        public static final InputChar KEY_DOWN              = fkey(KeyDefs.KEY_DOWN);
        public static final InputChar KEY_LEFT              = fkey(KeyDefs.KEY_LEFT);
        public static final InputChar KEY_RIGHT             = fkey(KeyDefs.KEY_RIGHT);
        public static final InputChar KEY_BACKSPACE         = fkey(KeyDefs.KEY_BACKSPACE);
        public static final InputChar KEY_DELETE            = fkey(KeyDefs.KEY_DELETE);
        public static final InputChar KEY_HOME              = fkey(KeyDefs.KEY_HOME);
        public static final InputChar KEY_END               = fkey(KeyDefs.KEY_END);
        public static final InputChar KEY_PGUP              = fkey(KeyDefs.KEY_PGUP);
        public static final InputChar KEY_PGDOWN            = fkey(KeyDefs.KEY_PGDOWN);

        private static InputChar fkey(char k) {
            return new InputChar(k, KeyDefs.CTYPE_FUNCTION);
        }
    }

    /*
      Control keys are some specific characters that are generated by
      the terminal just when the control + another key are pressed. They
      are defined in the ASCII table and should be the same for any platform,
      except for the backspace key. The backspace key is actually a control key,
      but since, depending on the source terminal, its key code might vary,
      it is defined as a function key in jcurses and internally managed by
      the native library.
     */
    public static class Control {
        public static final InputChar ESCAPE    = fromRawCode((char) 27);
        public static final InputChar TAB       = fromRawCode('\t');
        public static final InputChar ENTER     = fromRawCode('\r');
        public static final InputChar SIGINT    = fromRawCode((char) 3);

        public static InputChar fromKey(char kbdKey) {
            return new InputChar((char) (kbdKey & 0x1f), KeyDefs.CTYPE_CONTROL);
        }

        public static InputChar fromRawCode(char x) {
            return new InputChar(x, KeyDefs.CTYPE_CONTROL);
        }
    }

    public static InputChar literal(char ch) {
        return new InputChar(ch, KeyDefs.CTYPE_PRINTABLE);
    }

    public enum InputType {
        PRINTABLE(KeyDefs.CTYPE_PRINTABLE),
        CONTROL(KeyDefs.CTYPE_CONTROL),
        FUNCTION(KeyDefs.CTYPE_FUNCTION),
        ERROR(KeyDefs.CTYPE_ERROR),
        UNKNOWN(KeyDefs.CTYPE_UNKNOWN);

        int code;
        InputType(int code) {
            this.code = code;
        }

        public static InputType from(int type) {
            if ((type & KeyDefs.CTYPE_PRINTABLE) > 0)
                return PRINTABLE;

            if ((type & KeyDefs.CTYPE_CONTROL) > 0)
                return CONTROL;

            if ((type & KeyDefs.CTYPE_FUNCTION) > 0)
                return FUNCTION;

            if ((type & KeyDefs.CTYPE_ERROR) > 0)
                return ERROR;

            return UNKNOWN;
        }
    }

    private final int raw;
    private final char code;
    private final InputType type;

    public InputChar(int raw) {
        this.raw = raw;
        this.code = (char) (raw & 0xffff);
        this.type = InputType.from(raw >>> 24);
    }

    public InputChar(char value, int type) {
        this.code = value;
        this.type = InputType.from(type);
        this.raw = ((type & 0xff) << 24) | (value & 0xffff);
    }

    public int getRaw() {
        return this.raw;
    }

    public InputType getType() {
        return type;
    }

    public boolean isPrintable() {
        return type == InputType.PRINTABLE;
    }

    public boolean isControl() {
        return type == InputType.CONTROL;
    }

    public boolean isFunction() {
        return type == InputType.FUNCTION;
    }

    public char getCode()
    {
        return code;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        InputChar inputChar = (InputChar) o;
        return code == inputChar.code &&
                type == inputChar.type;
    }

    @Override
    public int hashCode() {
        return Objects.hash(code, type);
    }

    @Override
    public String toString() {
        return "InputChar{" +
                "code=" + code +
                ", type=" + type +
                '}';
    }
}