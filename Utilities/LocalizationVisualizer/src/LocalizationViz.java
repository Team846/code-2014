import java.awt.Color;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.event.*;

import javax.swing.*;

import edu.wpi.first.wpilibj.networktables.NetworkTable;
import edu.wpi.first.wpilibj.tables.TableKeyNotDefinedException;


public class LocalizationViz extends JPanel implements ActionListener
{
	private String x, y,theta;
	
	private static final int width = 700;
	private static final int height = 800;
	
	private Timer timer;	
	
	private NetworkTable nt;

	
	public LocalizationViz()
	{	
		
		NetworkTable.setClientMode();
		NetworkTable.setIPAddress("192.168.1.105");
		
		nt = NetworkTable.getTable("Test");
		
		x = y = theta = "0";
		
		timer  = new Timer(10, this);
		timer.start();
		
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
				
		try
		{
			String val = nt.getString("values");
			String[] valArray = val.split(" ");
			x = valArray[0];
			y = valArray[1];
			theta = valArray[2];
		}
		
		catch(TableKeyNotDefinedException ex)
		{
			System.out.println(ex);		
		}	
				
		repaint();
		
	}
	
	
	public void paintComponent(Graphics g)
	{
		
		super.paintComponent(g);
		
		
		g.drawOval(width/2 - 50 + Integer.parseInt(x),height/2 -50 + Integer.parseInt(y),100,100);
		
		g.setColor(Color.RED);
		g.drawString("X Position: " + x, 0, 10);
		g.drawString("Y Position: " + y, 0, 20);
		g.drawString("Rotation Angle: " + theta, 0, 30);
	
		
	}
	

	public static void main(String[] args)
	{
		JFrame w = new JFrame("Localization Visualizer");	

		w.setSize(width, height);

		w.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		Container c = w.getContentPane();

		c.add(new LocalizationViz());

		w.setResizable(false);

		w.setVisible(true);
		
	}
	



}
