
package jcurses.widgets;

import jcurses.system.CharColor;
import jcurses.util.Rectangle;

/**
 * This interface is to be implemented by widgets, that use <code>ScrollbarPainter</code> to paint vertical or horizontal (or both ) scrollbars. Througth this
 * interface the widget gives to the <code>ScrollbarPainter</code> needed data to paint or refresh scrollbars;
 */

public interface IScrollable
{

    /**
     * This method returns true, if the using widget has a horizontal scrollbar, ( independent of the size of it, this can be also empty)
     *
     * @return true, if the scrollbar is to be paint, false otherwise
     */

    boolean hasHorizontalScrollbar();

    /**
     * This method returns true, if the using widget has a vertical scrollbar, ( independent of the size of it, this can be also empty)
     *
     * @return true, if the scrollbar is to be paint, false otherwise
     */

    boolean hasVerticalScrollbar();

    /**
     * The method returns the rectangle of the border, on which scrollbars are to be paint
     *
     * @return rectangle of the border
     */
    Rectangle getBorderRectangle();

    /**
     * The method returns colors, with which the border is to be paint
     *
     * @return border colors
     */

    CharColor getBorderColors();

    /**
     * The method returns colors, with which scrollbars are to be paint
     *
     * @return scrollbar colors
     */

    CharColor getScrollbarColors();

    /**
     * The method returns the offset of the horizontal scrollbar as part of the length of the side of the border rectangle ( 0 <=value < 1.0 )
     *
     * @return horizontal scrollbar offset
     */
    float getHorizontalScrollbarOffset();

    /**
     * The method returns the length of the horizontal scrollbar as part of the length of the side of the border rectangle ( 0 < value <= 1.0 )
     *
     * @return vertical scrollbar o
     */
    float getHorizontalScrollbarLength();

    /**
     * The method returns the offset of the vertical scrollbar as part of the length of the side of the border rectangle ( 0 < =value < 1.0 )
     */
    float getVerticalScrollbarOffset();

    /**
     * The method returns the length of the vertical scrollbar as part of the length of the side of the border rectangle ( 0 <value <= 1.0 )
     */
    float getVerticalScrollbarLength();

}