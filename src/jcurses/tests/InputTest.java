package jcurses.tests;

import jcurses.system.CharColor;
import jcurses.system.InputChar;
import jcurses.system.Toolkit;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

public class InputTest {
    public static void main(String[] args) throws IOException {
        Toolkit.init();

        InputChar panicInput = InputChar.Control.fromKey('Q');

        Writer w = new FileWriter("input.txt", true);
        CharColor color = new CharColor(CharColor.CYAN, CharColor.BLACK);
        InputChar ch;
        do {
            ch = Toolkit.readCharacter();
            w.write(String.format("Input: { code= %d (%c); type= %s; }\n", (int) ch.getCode(), ch.getCode(), ch.getType()));
            Toolkit.printString(ch.getCode() + "          ", 0, 0, color);
            w.flush();


        } while (!ch.equals(panicInput));

        Toolkit.printString("CULO!", 0, 0, color);
    }
}
