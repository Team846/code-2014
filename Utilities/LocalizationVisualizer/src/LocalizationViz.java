import java.awt.Color;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;

import edu.wpi.first.wpilibj.networktables.NetworkTable;
import edu.wpi.first.wpilibj.tables.TableKeyNotDefinedException;


public class LocalizationViz extends JPanel implements ActionListener
{
	private String x, y,theta;
	
	private static final int width = 700;
	private static final int height = 800;
	
	private Timer timer;	
	
	private NetworkTable nt;

	private String redBall, blueBall;
	
	
	public LocalizationViz()
	{	
		
		NetworkTable.setClientMode();
		NetworkTable.setIPAddress("10.8.46.2");
				
		nt = NetworkTable.getTable("Location");
		
		x = y = theta = "0";
		
		timer  = new Timer(10, this);
		timer.start();
		
		redBall = blueBall = "";
		
		
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
				
		try
		{
			String val = nt.getString("Data");
			String[] valArray = val.split(" ");
			x = valArray[0];
			y = valArray[1];
			theta = valArray[2];
			
			redBall = nt.getString("redBall");
			blueBall = nt.getString("blueBall");
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
		
		g.drawOval(width/2 - 10 + (int)(Double.parseDouble(x) * 10),height/2 - 10 - (int)(Double.parseDouble(y) * 10),20,20);
		
		g.setColor(Color.RED);
		g.drawLine(width / 2, height / 2 - 10, width / 2, height / 2 + 10);
		g.drawLine(width / 2 - 10, height / 2, width / 2 + 10, height / 2);
		g.drawString("X Position: " + x, 0, 10);
		g.drawString("Y Position: " + y, 0, 20);
		g.drawString("Rotation Angle: " + theta, 0, 30);
		
		
		
		
		
		if(redBall !=  "")
		{
			String[] val = redBall.split(" ");
			g.setColor(Color.MAGENTA);
			g.fillOval(width/2 - 10 + (int)(Double.parseDouble(val[0]) * 10),height/2 - 10 - (int)(Double.parseDouble(val[1])*10),20,20);		
			g.drawString("Red Ball X : "+ (int)(Double.parseDouble(val[0]) * 10), width-75, 10);
			g.drawString("Red Ball Y : "+ (int)(Double.parseDouble(val[1])*10), width-75, 20);
		}
		
		if(blueBall !=  "")
		{
			String[] val = blueBall.split(" ");
			g.setColor(Color.BLUE);
			g.fillOval(width/2 - 10 + (int)(Double.parseDouble(val[0]) * 10),height/2 - 10 - (int)(Double.parseDouble(val[1])*10),20,20);					
			g.drawString("Blue Ball X : "+ (int)(Double.parseDouble(val[0]) * 10), width-75, 30);
			g.drawString("Blue Ball Y : "+ (int)(Double.parseDouble(val[1])*10), width-75, 40);
		}
		
		
		
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
