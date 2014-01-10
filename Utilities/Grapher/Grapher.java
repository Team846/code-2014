import java.awt.Color;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.Timer;

import java.util.*;

import edu.wpi.first.wpilibj.networktables.NetworkTable;

public class Grapher extends JPanel implements ActionListener
{
    private int time;

    float lastValue = 0;
    JTextArea text;
    
    private static final int width = 800;
    private static final int height = 500;

    NetworkTable table;

    Map<String, Deque<Float> > values = new HashMap<String, Deque<Float> >();
    Map<String, Color > colors = new HashMap<String, Color >();
    String[] fieldArray;

    public Grapher()
    {
        time = 0;
        NetworkTable.setClientMode();
        NetworkTable.setIPAddress( "10.8.46.2" );
        table = NetworkTable.getTable( "RobotData" );
        text = new JTextArea(5, 20);
        this.add( text );
        Timer clock = new Timer( 10, this );
        clock.start();
    }

    public void paintComponent( Graphics g )
    {
        super.paintComponent( g );
        
        for (int i = 0; i < fieldArray.length; i++)
        {
            paintLine( colors.get( fieldArray[i] ) , values.get(fieldArray[i]), g);
        }
        
        g.setColor( Color.LIGHT_GRAY );
        g.fillRect( 0, 0, 250, 100 );
        
        for (int i = 0; i < fieldArray.length; i++)
        {
            g.setColor( colors.get( fieldArray[i] ) );
            g.drawString( fieldArray[i] + " - " + values.get(fieldArray[i]).peekLast(), 10, 20 + 15 * i );
        }
    }
    
    void paintLine(Color c1, Deque<Float> d, Graphics g)
    {
        Container c = getRootPane();
        Iterator<Float> itr = d.iterator();
        int index = 0;
        g.setColor( c1 );
        while ( itr.hasNext() )
        {
            float current = itr.next();
            g.drawLine( index - 1,
                c.getHeight() - 50 - (int)(height * lastValue),
                index,
                c.getHeight() - 50 - (int)(height * current));
            index++;
            lastValue = current;
        }
    }

    public void actionPerformed( ActionEvent e )
    {
        String fields = text.getText();
        fieldArray = fields.split("\n");
        for (int i = 0; i < fieldArray.length; i++)
        {
            if (values.get(fieldArray[i]) == null)
            {
                values.put( fieldArray[i], new LinkedList<Float>() );
                Random rand = new Random();
                float r = rand.nextFloat() * (float)0.5;
                float g = rand.nextFloat() * (float)0.5;
                float b = rand.nextFloat() * (float)0.5;
                colors.put(fieldArray[i], new Color(r, g, b));
            }
            addValues(fieldArray[i], values.get(fieldArray[i]));
        }
        
        repaint();
    }
    
    void addValues(String s, Deque<Float> d)
    {
        double val = table.getNumber( s, 0.0 );

        if ( d.size() > width )
            d.removeFirst();
        d.addLast( (float) val );
    }

    public static void main( String[] args )
    {
        JFrame w = new JFrame( "Grapher" );
        w.setSize( width, height );
        w.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        Container c = w.getContentPane();
        c.add( new Grapher() );
        w.setResizable( false );
        w.setVisible( true );
    }
}
