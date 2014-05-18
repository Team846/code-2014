import java.awt.BorderLayout;
import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.apache.commons.io.filefilter.DirectoryFileFilter;
import org.apache.commons.io.filefilter.WildcardFileFilter;


class PartBrowser
{
    JTextArea fileDetailsTextArea = new JTextArea();

    JList drawList;

    JList printList;

    File[] drawFiles;

    String[] drawFilesNames;

    File[] printFiles;

    String[] printFilesNames;


    public PartBrowser( File[] file1, File[] file2 )
    {

        drawFiles = file1;
        printFiles = file2;
        drawFilesNames = new String[drawFiles.length];
        printFilesNames = new String[printFiles.length];
        for ( int i = 0; i < drawFiles.length; i++ )
        {
            drawFilesNames[i] = drawFiles[i].getName();
        }
        for ( int i = 0; i < printFiles.length; i++ )
        {
            printFilesNames[i] = printFiles[i].getName();
        }
        drawList = new JList( drawFilesNames );
        printList = new JList( printFilesNames );
        JFrame frame = new JFrame();
        frame.setSize( 700, 500 );
        JPanel panel = new JPanel( new BorderLayout() );
        JTabbedPane tabbedPane = new JTabbedPane();
        panel.add( tabbedPane );
        panel.add( fileDetailsTextArea, BorderLayout.SOUTH );
        tabbedPane.addTab( "Ready For Drawing", new JScrollPane( drawList ) );
        tabbedPane.addTab( "Ready For Print", new JScrollPane( printList ) );

        drawList.addListSelectionListener( new ListSelectionListener()
        {

            public void valueChanged( ListSelectionEvent e )
            {
                fileDetailsTextArea.setText( ""
                    + ( "Name: " + drawFiles[drawList.getSelectedIndex()].getName() )
                    + '\n' + "Path: "
                    + ( drawFiles[drawList.getSelectedIndex()].getPath() )
                    + '\n' + "Size: "
                    + ( drawFiles[drawList.getSelectedIndex()].length() ) );
            }
        } );
        printList.addListSelectionListener( new ListSelectionListener()
        {

            public void valueChanged( ListSelectionEvent e )
            {
                fileDetailsTextArea.setText( ""
                    + ( "Name: " + printFiles[printList.getSelectedIndex()].getName() )
                    + '\n' + "Path: "
                    + ( printFiles[printList.getSelectedIndex()].getPath() )
                    + '\n' + "Size: "
                    + ( printFiles[printList.getSelectedIndex()].length() ) );
            }
        } );
        frame.add( panel );
        frame.setVisible( true );
    }


    static File[] readyForPrint( String dirPath )
    {

        File topDir = new File( dirPath );

        List<File> directories = new ArrayList<>();
        directories.add( topDir );

        List<File> textFiles = new ArrayList<>();

        List<String> filterWildcards = new ArrayList<>();
        filterWildcards.add( "*.ReadyForPrint" );

        FileFilter typeFilter = new WildcardFileFilter( filterWildcards );

        while ( directories.isEmpty() == false )
        {
            List<File> subDirectories = new ArrayList();

            for ( File f : directories )
            {
                subDirectories.addAll( Arrays.asList( f.listFiles( (FileFilter)DirectoryFileFilter.INSTANCE ) ) );
                textFiles.addAll( Arrays.asList( f.listFiles( typeFilter ) ) );
            }

            directories.clear();
            directories.addAll( subDirectories );

        }
        File[] files = textFiles.toArray( new File[textFiles.size()] );
        return files;

    }


    static File[] readyForDrawing( String dirPath )
    {

        File topDir = new File( dirPath );

        List<File> directories = new ArrayList<>();
        directories.add( topDir );

        List<File> textFiles = new ArrayList<>();

        List<String> filterWildcards = new ArrayList<>();
        filterWildcards.add( "*.ReadyForDrawing" );

        FileFilter typeFilter = new WildcardFileFilter( filterWildcards );

        while ( directories.isEmpty() == false )
        {
            List<File> subDirectories = new ArrayList();

            for ( File f : directories )
            {
                subDirectories.addAll( Arrays.asList( f.listFiles( (FileFilter)DirectoryFileFilter.INSTANCE ) ) );
                textFiles.addAll( Arrays.asList( f.listFiles( typeFilter ) ) );
            }

            directories.clear();
            directories.addAll( subDirectories );

        }
        File[] files = textFiles.toArray( new File[textFiles.size()] );
        return files;

    }


    public static void main( String[] args )
    {

        File[] drawFiles = readyForDrawing( System.getProperty( "user.dir" ) );
        System.out.println( System.getProperty( "user.dir" ) );
        File[] printFiles = readyForPrint( System.getProperty( "user.dir" ) );

        PartBrowser test = new PartBrowser( drawFiles, printFiles );

    }
}
