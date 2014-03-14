import java.util.Arrays;

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

    int[][] play_matches = new int[NUM_TEAMS][NUM_TEAMS];

    int[][] firstArrayFinal;

    public static void main( String[] args ) throws BLUEApiException
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        setArrays();
        makeFirstArray();
        makeTotalArray();
        combineArrays();
        // double[][] matrix = { { 1, 2, 2 }, { 1, 1, 3 } };
        // RREF( matrix );
        // printMatrix( matrix );
    }

    public static void combineArrays()
    {
        int[][] firstArrayFinal = new int[matches.length][matches[0].length];
        for(int i = 0; i < firstArrayFinal.length; i++)
        {
            for(int j = 0; j < firstArrayFinal[0].length; j++)
            {
                if(j == firstArrayFinal[0].length - 1)
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
            changeOne(Integer.parseInt( match_split2[0].substring( 3 ) ), 2 * i + 1);
            changeOne(Integer.parseInt( match_split2[1].substring( 4 ) ), 2 * i + 1);
            changeOne( Integer.parseInt( match_split2[2].substring( 4 )
                .replaceAll( "GeoData\\]", "" )
                .replaceAll( "[^\\d]", "" ) ),
                2 * i + 1);
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
     * precondition: n x n+1 matrix postcondition: matrix in rref form (solved)
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