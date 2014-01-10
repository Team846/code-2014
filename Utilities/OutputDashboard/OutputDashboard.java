import java.awt.Color;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.Timer;

import java.util.*;

import edu.wpi.first.wpilibj.networktables.NetworkTable;

public class OutputDashboard extends JPanel implements ActionListener
{
    private static final int width = 800;
    private static final int height = 500;
    
    private NetworkTable table;
    
    public OutputDashboard()
    {
        NetworkTable.setClientMode();
        NetworkTable.setIPAddress( "10.8.46.2" );
        table = NetworkTable.getTable( "OutputData" );
       
    }
    
    public void actionPerformed(ActionEvent e)
    {
        repaint();
    }
    
    public void paintComponent(Graphics g)
    {
        for(int i = 1; i <= 16; i++)
        {
            if(table.containsKey( "CANJaguar" + i))
            {
                g.drawRect( 10 + 40 * (i-1) , 20, 30, 20 );
                g.drawString( "CANJaguar" + i, 10 + 40 * (i-1) + 5, 25 );
                g.drawString( "" + table.getNumber( "CANJaguar" + i ), 10 + 40 * (i-1), 40 );
            }
            
            if(table.containsKey("Pneumatics" + i))
            {
                g.drawRect( 10 + 40 * (i-1) , 100, 30, 20 );
                g.drawString( "Pneumatics" + i, 10 + 40 * (i-1) + 5, 105 );
                boolean state = table.getBoolean( "Pneumatics" + i);
                if(state)
                {
                    g.setColor(Color.GREEN);
                    g.fillRect(10 + 40 * (i-1) , 100, 30, 20 );
                }
                else
                {
                    g.setColor(Color.RED);
                    g.fillRect(10 + 40 * (i-1) , 100, 30, 20 );
                }
            }
        }
    }
    public static void main( String[] args )
    {
        JFrame w = new JFrame( "Grapher" );
        w.setSize( width, height );
        w.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        Container c = w.getContentPane();
        c.add( new OutputDashboard() );
        w.setResizable( false );
        w.setVisible( true );
    }
}
