import com.tonypeng.api.thebluealliance.BLUE;
import com.tonypeng.api.thebluealliance.BLUE.BLUEApiException;
import com.tonypeng.api.thebluealliance.BLUE.Teams.Team;


public class Matrix
{
    final int NUM_MATCHES = 99;

    final static int NUM_TEAMS = 59;

    static int[] list_teams = new int[NUM_TEAMS];

    int[][] matches = new int[2 * NUM_MATCHES][NUM_TEAMS];

    int[][] play_matches = new int[NUM_TEAMS][NUM_TEAMS];


    public static void main( String[] args ) throws BLUEApiException
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        setArrays();
        // double[][] matrix = { { 1, 2, 2 }, { 1, 1, 3 } };
        // RREF( matrix );
        // printMatrix( matrix );
    }


    public static void setArrays() throws BLUEApiException
    {
        int error = 0;
        int count2 = 0;
        BLUE.Events.Event event = BLUE.Events.getEvent( "casj", 2013 );
        BLUE.Teams.Team[] teams = event.getTeams();
        int[] list_teams = new int[NUM_TEAMS];

        for ( int j = 0; j < teams.length; j++ )
        {
            String[] teams_split = teams[j].toString().split( "}" );
            BLUE.Matches.Match[] matches = event.getMatches();
            for ( int i = 0; i < teams_split.length; i++ )
            {
                int[] s = checkString( teams_split[i] );
                if ( !(s[0] == -1 ))
                {
                    int end_num = teams_split[i].indexOf( ",", k + 5 );
                    String team_number = teams_split[i].substring( k + 5,
                        end_num );
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
        for ( int i = 0; i < list_teams.length; i++ )
        {
            System.out.println( list_teams[i] );
            System.out.println( i );
        }
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
        for ( int j = 0; j < s.length(); j++ )
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