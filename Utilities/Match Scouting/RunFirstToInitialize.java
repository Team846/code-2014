import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Arrays;

import com.tonypeng.api.thebluealliance.BLUE;
import com.tonypeng.api.thebluealliance.BLUE.BLUEApiException;


public class RunFirstToInitialize
{

    static int NUM_MATCHES;

    static int NUM_TEAMS = 38; // hardcoded

    static int[] list_teams = new int[NUM_TEAMS];

    static double[] ratings = new double[NUM_TEAMS];


    public static void main( String[] args )
        throws BLUEApiException,
        FileNotFoundException
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        setArrays();
        firstSet();
        printRankings();
    }


    public static void firstSet()
    {
        for ( int i = 0; i < ratings.length; i++ )
        {
            ratings[i] = 1000.0;
        }
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


    public static void setArrays() throws BLUEApiException
    {
        BLUE.Matches.Match match[] = BLUE.Events.getEvent( "mxmc", 2014 )
            .getMatches();
        NUM_MATCHES = match.length;
        int error = 0;
        int count2 = 0;
        BLUE.Events.Event event = BLUE.Events.getEvent( "mxmc", 2014 );
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

}
