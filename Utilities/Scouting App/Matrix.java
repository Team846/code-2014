import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Scanner;

import com.tonypeng.api.thebluealliance.BLUE;
import com.tonypeng.api.thebluealliance.BLUE.BLUEApiException;
import com.tonypeng.api.thebluealliance.BLUE.Teams.Team;


public class Matrix
{
    static int NUM_MATCHES;

    static int NUM_TEAMS = 59;

    static int[] list_teams = new int[NUM_TEAMS];

    static int[] scores;

    static int[][] matches;

    static int[][] play_matches = new int[NUM_TEAMS][NUM_TEAMS];

    static int[][] firstArrayFinal;

    static int[][] finalArray;

    static double[][] finalArrayDouble;

    static double[][] finalDPR;
    
    static double[] DPR_scores;
    
    static double[] opr;


    public static void main( String[] args )
        throws BLUEApiException,
        FileNotFoundException
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        setArrays();
        makeFirstArray();
        makeTotalArray();
        combineArrays();
        makeFinalArray();
        makeDoubleFinal();
        RREF( finalArrayDouble );
        makeOPR();
        displayOPR();
        createFinalDPR();
        makeDPR();
    }

    public static void makeDPR()
    {
        finalDPR = new double[finalArrayDouble.length][finalArrayDouble[0].length];
        DPR_scores = new double[scores.length];
        double currentOPR = 0.0;
        int team1 = 0;
        int team2 = 0;
        int team3 = 0;
        
        for(int i = 0; i < finalArrayDouble.length; i++)
        {
            for(int j = 0; j < finalArrayDouble[0].length; j++)
            {
                finalDPR[i][j] = finalArrayDouble[i][j];
            }
        }
        for(int i = 0; i < DPR_scores.length; i++)
        {
            DPR_scores[i] = (double)scores[i];
        }
        for(int i = 0; i < finalArrayDouble.length; i+=2)
        {
            team1 = findNextIndex(matches[i + 1], 0);
            team2 = findNextIndex(matches[i + 1], team1);
            team3 = findNextIndex(matches[i + 1], team2);
            DPR_scores[i] = opr[team1] + opr[team2] + opr[team3] - scores[i]; 
        }
    }
    
    public static int findNextIndex(int[] matrix, int nice)
    {
        for(int i = nice; i < matrix.length; i++)
        {
            if(matrix[i] == 1)
            {
                return i;
            }
        }
        return -1;
    }

    public static void createFinalDPR()
    {
        
    }
    
    public static void displayOPR() throws FileNotFoundException
    {
        PrintWriter pw = new PrintWriter( new File( "OffensivePowerRating.txt" ) );
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
            pw.printf( " -   " + "%.5f%n", opr[i] );
        }
        pw.close();
    }


    public static void makeOPR()
    {
        opr = new double[list_teams.length];
        for ( int i = 0; i < finalArrayDouble.length; i++ )
        {
            opr[i] = finalArrayDouble[i][finalArrayDouble[0].length - 1];
        }
    }


    public static void makeDoubleFinal()
    {
        int totalScore = 0;
        finalArrayDouble = new double[finalArray.length][finalArray[0].length + 1];
        for ( int i = 0; i < finalArray.length; i++ )
        {
            for ( int j = 0; j < finalArray[0].length; j++ )
            {
                finalArrayDouble[i][j] = (double)finalArray[i][j];
            }
        }
        for ( int i = 0; i < finalArrayDouble.length; i++ )
        {
            int j = finalArrayDouble[0].length - 1;
            if ( j == finalArrayDouble[0].length - 1 )
            {
                for ( int k = 0; k < matches.length; k++ )
                {
                    if ( matches[k][i] == 1 )
                    {
                        totalScore += scores[k];
                    }
                }
                finalArrayDouble[i][j] = (double)totalScore;
                totalScore = 0;
            }
        }
    }


    public static void makeFinalArray()
    {
        int count = 0;

        finalArray = new int[list_teams.length][list_teams.length];
        for ( int i = 0; i < finalArray.length; i++ )
        {
            for ( int j = 0; j < finalArray[0].length; j++ )
            {
                count = 0;
                for ( int k = 0; k < matches.length; k++ )
                {
                    if ( matches[k][i] == 1 && matches[k][j] == 1 )
                    {
                        count++;
                    }
                }
                finalArray[i][j] = count;
            }
        }
    }


    public static void combineArrays()
    {
        int[][] firstArrayFinal = new int[matches.length][matches[0].length];
        for ( int i = 0; i < firstArrayFinal.length; i++ )
        {
            for ( int j = 0; j < firstArrayFinal[0].length; j++ )
            {
                if ( j == firstArrayFinal[0].length - 1 )
                {
                    firstArrayFinal[i][j] = scores[i];
                }
                else
                {
                    firstArrayFinal[i][j] = matches[i][j];
                }
            }
        }
    }


    public static void makeTotalArray() throws BLUEApiException
    {
        BLUE.Matches.Match match[] = BLUE.Events.getEvent( "casj", 2013 )
            .getMatches();
        for ( int i = 0; i < match.length; i++ )
        {
            String[] match_split = match[i].toString().split( "\\}" );
            String[] match_split2 = match_split[1].split( "," );
            scores[2 * i] = Integer.parseInt( match_split2[1].trim() );
            match_split2 = match_split[2].split( "," );
            scores[2 * i + 1] = Integer.parseInt( match_split2[1].trim() );
        }
    }


    public static void makeFirstArray() throws BLUEApiException
    {
        BLUE.Matches.Match match[] = BLUE.Events.getEvent( "casj", 2013 )
            .getMatches();
        NUM_MATCHES = match.length;
        matches = new int[2 * NUM_MATCHES][NUM_TEAMS];
        for ( int i = 0; i < match.length; i++ )
        {
            String[] match_split = match[i].toString().split( "\\[" );
            String[] match_split2 = match_split[1].split( "," );
            changeOne( Integer.parseInt( match_split2[0].substring( 3 ) ),
                2 * i );
            changeOne( Integer.parseInt( match_split2[1].substring( 4 ) ),
                2 * i );
            changeOne( Integer.parseInt( match_split2[2].substring( 4 )
                .replaceAll( "GeoData\\]", "" )
                .replaceAll( "[^\\d]", "" ) ),
                2 * i );
            match_split2 = match_split[2].split( "," );
            changeOne( Integer.parseInt( match_split2[0].substring( 3 ) ),
                2 * i + 1 );
            changeOne( Integer.parseInt( match_split2[1].substring( 4 ) ),
                2 * i + 1 );
            changeOne( Integer.parseInt( match_split2[2].substring( 4 )
                .replaceAll( "GeoData\\]", "" )
                .replaceAll( "[^\\d]", "" ) ),
                2 * i + 1 );
        }
    }


    public static void changeOne( int a, int b )
    {
        for ( int i = 0; i < list_teams.length; i++ )
        {
            if ( a == list_teams[i] )
            {
                matches[b][i] = 1;
            }
        }
    }


    public static void setArrays() throws BLUEApiException
    {
        BLUE.Matches.Match match[] = BLUE.Events.getEvent( "casj", 2013 )
            .getMatches();
        NUM_MATCHES = match.length;
        matches = new int[2 * NUM_MATCHES][NUM_TEAMS];
        scores = new int[2 * NUM_MATCHES];
        int error = 0;
        int count2 = 0;
        BLUE.Events.Event event = BLUE.Events.getEvent( "casj", 2013 );
        BLUE.Teams.Team[] teams = event.getTeams();

        for ( int j = 0; j < teams.length; j++ )
        {
            String[] teams_split = teams[j].toString().split( "}" );
            for ( int i = 0; i < teams_split.length; i++ )
            {
                int[] s = checkString( teams_split[i] );
                if ( !( s[0] == -1 ) )
                {
                    String team_number = "";
                    if ( s[2] == 0 )
                    {
                        int end_num = teams_split[i].indexOf( ",", s[1] + 5 );
                        team_number = teams_split[i].substring( s[1] + 5,
                            end_num );
                    }
                    if ( s[2] == 1 )
                    {
                        int end_num = teams_split[i].indexOf( ",", s[1] + 8 );
                        team_number = teams_split[i].substring( s[1] + 8,
                            end_num );
                    }
                    for ( int count = 0; count < team_number.length(); count++ )
                    {
                        if ( !Character.isDigit( team_number.charAt( 0 ) ) )
                        {
                            error = 1;
                        }
                    }
                    if ( error == 0 )
                    {
                        list_teams[count2] = Integer.parseInt( team_number );
                        count2++;
                    }
                    error = 0;
                }

            }
        }
        Arrays.sort( list_teams );
    }


    public static int[] checkString( String s )
    {
        int[] stuff = new int[3];
        stuff[0] = 0;
        stuff[1] = -1;
        stuff[2] = -1;
        for ( int i = 0; i < s.length() - 3; i++ )
        {
            if ( s.substring( i, i + 3 ).equals( "USA" ) )
            {
                stuff[0] = 1;
                stuff[1] = i;
                stuff[2] = 0;
            }
        }
        for ( int j = 0; j < s.length() - 6; j++ )
        {
            if ( s.substring( j, j + 6 ).equals( "Mexico" ) )
            {
                stuff[0] = 1;
                stuff[1] = j;
                stuff[2] = 1;
            }
        }
        return stuff;
    }


    public static void printMatrix( double[][] matrix )
    {
        int n = matrix.length;
        for ( int i = 0; i < n; i++ )
        {
            for ( int j = 0; j <= n; j++ )
            {
                System.out.print( matrix[i][j] + " " );
            }
            System.out.println();
        }
    }


    /*
     * precondition: n x n+1 matrix postcondition: matrix in rref form
     */
    public static void RREF( double[][] matrix )
    {
        int n = matrix.length;
        for ( int i = 0; i < n; i++ )
        {
            for ( int j = n; j >= i; j-- )
                matrix[i][j] /= matrix[i][i];
            for ( int j = i + 1; j < n; j++ )
                for ( int k = n; k >= i; k-- )
                    matrix[j][k] -= matrix[i][k] * matrix[j][i];
        }
        for ( int i = n - 1; i >= 0; i-- )
        {
            for ( int j = 0; j < i; j++ )
                for ( int k = n; k >= i; k-- )
                    matrix[j][k] -= matrix[i][k] * matrix[j][i];
        }
    }
}