package jcurses.themes;

import jcurses.system.CharColor;

/**
 * Adds theme support for JCurses components.
 * 
 * @author alewis
 *  
 */
public interface Theme
{
  String COLOR_DEFAULT           = "default";

  String COLOR_WINDOW_BORDER     = "window.border";
  String COLOR_WINDOW_BACKGROUND = "window.background";
  String COLOR_WINDOW_TITLE      = "window.title";
  String COLOR_WINDOW_TEXT       = "window.text";
  String COLOR_WINDOW_SHADOW     = "window.shadow";

  //  public final String COLOR_DIALOG_BORDER = "dialog.border";
  //  public final String COLOR_DIALOG_BACKGROUND = "dialog.background";
  //  public final String COLOR_DIALOG_TITLE = "dialog.title";
  //  public final String COLOR_DIALOG_TEXT = "dialog.text";
  //  public final String COLOR_DIALOG_SHADOW = "dialog.shadow";

  String COLOR_WIDGET_BORDER     = "widget.border";
  String COLOR_WIDGET_BACKGROUND = "widget.background";
  String COLOR_WIDGET_TITLE      = "widget.title";
  String COLOR_WIDGET_TEXT       = "widget.text";
  String COLOR_WIDGET_ACTION     = "widget.action";
  String COLOR_WIDGET_SELECTED   = "widget.selected";
  String COLOR_WIDGET_SHORTCUT   = "widget.shortcut";
  String COLOR_WIDGET_SCROLLBAR   = "widget.scrollbar";

  CharColor getColor(String aKey);

  void setColor(String aKey, CharColor aColor);

}