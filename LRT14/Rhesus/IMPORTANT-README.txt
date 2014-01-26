Because SVN does not interpret hardlinks, it is imperative that you add these folders (i.e. Messenger, Toolkit) to your
SVN ignore list, or SVN will commit them as actual folders instead of linking them to Include.

Please note that WindRiver will not commit edited files in Rhesus after the folders are added to the ignore list.  Therefore,
to edit them, you should commit them with TortoiseSVN from Includes.

tl;dr ADD ALL FOLDERS HERE TO THE SVN IGNORE LIST.