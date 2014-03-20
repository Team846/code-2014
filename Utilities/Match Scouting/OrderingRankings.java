import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.StringTokenizer;

import com.tonypeng.api.thebluealliance.BLUE;
import com.tonypeng.api.thebluealliance.BLUE.BLUEApiException;


public class OrderingRankings
{

    static int NUM_MATCHES;

    static int NUM_TEAMS = 20;

    static int K = 20;

    static int[] list_teams = { 100, 200, 300, 400, 500, 600, 700, 800, 900,
        1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000 };

    static double[] ratings = new double[NUM_TEAMS];

    static double first = .28;

    static double second = .24;

    static double third = .20;

    static double fourth = .14;

    static double fifth = .09;

    static double sixth = 0.07;


    public static void main( String[] args ) throws Exception
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        // run initial Scan first to get stuff set up in Ratings.txt
        importFile();
        
        int[] parsedNumbers = new int[6];
        BufferedReader br = new BufferedReader( new InputStreamReader( System.in ) );
        for(int i = 0; i < 6; i++)
        {
            System.out.println("Enter the team in place: " + i);
            parsedNumbers[i] = Integer.parseInt( br.readLine() );
        }
        updateRatings( parsedNumbers );

        printRankings();
        
//        if(br.readLine().equalsIgnoreCase( "sort" ))
//        {
//            double[][] sorted = new double[ratings.length][2];
//            for(int i = 0; i < ratings.length; i++)
//            {
//                sorted[i][0] = list_teams[i];
//                sorted[i][1] = ratings[i];
//            }
//            
//            System.out.println(sorted);
//        }
    }

    private static void importFile() throws Exception
    {
        in = new BufferedReader( new FileReader( new File( "Ratings.txt" ) ) );
        for ( int i = 0; i < list_teams.length; i++ )
        {
            list_teams[i] = nextInt();
            next();
            ratings[i] = nextDouble();
        }
    }

    static BufferedReader in;

    static StringTokenizer st;


    static int nextInt() throws Exception
    {
        return Integer.parseInt( next() );
    }


    static double nextDouble() throws Exception
    {
        return Double.parseDouble( next() );
    }


    static String next() throws Exception
    {
        while ( st == null || !st.hasMoreTokens() )
        {
            st = new StringTokenizer( in.readLine() );
        }
        return st.nextToken();
    }


    private static void printRankings() throws FileNotFoundException
    {
        PrintWriter pw = new PrintWriter( new File( "Ratings.txt" ) );
        for ( int i = 0; i < list_teams.length; i++ )
        {
            if ( list_teams[i] / 10 == 0 )
            {
                pw.printf( list_teams[i] + "    " );
            }
            else if ( list_teams[i] / 100 == 0 )
            {
                pw.printf( list_teams[i] + "   " );
            }
            else if ( list_teams[i] / 1000 == 0 )
            {
                pw.printf( list_teams[i] + "  " );
            }
            else
            {
                pw.printf( list_teams[i] + " " );
            }
            pw.printf( " -   " + "%.5f%n", ratings[i] );
        }
        pw.close();
    }


    public static void updateRatings( int[] parsedNumbers )
    {
        int[] index = new int[6];
        for ( int i = 0; i < 6; i++ )
        {
            index[i] = findIndex( parsedNumbers[i] );
        }
        bruteCalculate( index );
    }


    public static void bruteCalculate( int[] index )
    {
        double[] newRatings = new double[2];
        newRatings = calculateNew( ratings[index[0]],
            ratings[index[1]],
            first,
            second );
        ratings[index[0]] = newRatings[0];
        ratings[index[1]] = newRatings[1];
        newRatings = calculateNew( ratings[index[0]],
            ratings[index[2]],
            first,
            third );
        ratings[index[0]] = newRatings[0];
        ratings[index[2]] = newRatings[1];
        newRatings = calculateNew( ratings[index[0]],
            ratings[index[3]],
            first,
            fourth );
        ratings[index[0]] = newRatings[0];
        ratings[index[3]] = newRatings[1];
        newRatings = calculateNew( ratings[index[0]],
            ratings[index[4]],
            first,
            fifth );
        ratings[index[0]] = newRatings[0];
        ratings[index[4]] = newRatings[1];
        newRatings = calculateNew( ratings[index[0]],
            ratings[index[5]],
            first,
            sixth );
        ratings[index[0]] = newRatings[0];
        ratings[index[5]] = newRatings[1];
        newRatings = calculateNew( ratings[index[1]],
            ratings[index[2]],
            second,
            third );
        ratings[index[1]] = newRatings[0];
        ratings[index[2]] = newRatings[1];
        newRatings = calculateNew( ratings[index[1]],
            ratings[index[3]],
            second,
            fourth );
        ratings[index[1]] = newRatings[0];
        ratings[index[3]] = newRatings[1];
        newRatings = calculateNew( ratings[index[1]],
            ratings[index[4]],
            second,
            fifth );
        ratings[index[1]] = newRatings[0];
        ratings[index[4]] = newRatings[1];
        newRatings = calculateNew( ratings[index[1]],
            ratings[index[5]],
            second,
            sixth );
        ratings[index[1]] = newRatings[0];
        ratings[index[5]] = newRatings[1];
        newRatings = calculateNew( ratings[index[2]],
            ratings[index[3]],
            third,
            fourth );
        ratings[index[2]] = newRatings[0];
        ratings[index[3]] = newRatings[1];
        newRatings = calculateNew( ratings[index[2]],
            ratings[index[4]],
            third,
            fifth );
        ratings[index[2]] = newRatings[0];
        ratings[index[4]] = newRatings[1];
        newRatings = calculateNew( ratings[index[2]],
            ratings[index[5]],
            third,
            sixth );
        ratings[index[2]] = newRatings[0];
        ratings[index[5]] = newRatings[1];
        newRatings = calculateNew( ratings[index[3]],
            ratings[index[4]],
            fourth,
            fifth );
        ratings[index[3]] = newRatings[0];
        ratings[index[4]] = newRatings[1];
        newRatings = calculateNew( ratings[index[3]],
            ratings[index[5]],
            fourth,
            sixth );
        ratings[index[3]] = newRatings[0];
        ratings[index[5]] = newRatings[1];
        newRatings = calculateNew( ratings[index[4]],
            ratings[index[5]],
            fifth,
            sixth );
        ratings[index[4]] = newRatings[0];
        ratings[index[5]] = newRatings[1];
    }


    public static double[] calculateNew(
        double orig1,
        double orig2,
        double points1,
        double points2 )
    {
        double expected1 = 1 / ( 1 + Math.pow( 10, ( orig2 - orig1 ) / 400 ) );
        double expected2 = 1 / ( 1 + Math.pow( 10, ( orig1 - orig2 ) / 400 ) );
        double[] newRating = new double[2];
        points1 = points1 / ( points1 + points2 );
        points2 = points2 / ( points1 + points2 );
        newRating[0] = orig1 + K * ( points1 - expected1 );
        newRating[1] = orig2 + K * ( points2 - expected2 );
        return newRating;
    }


    public static int findIndex( int a )
    {
        for ( int i = 0; i < list_teams.length; i++ )
        {
            if ( list_teams[i] == a )
            {
                return i;
            }
        }
        return -1;
    }

}
