import com.tonypeng.api.thebluealliance.BLUE;
import com.tonypeng.api.thebluealliance.BLUE.BLUEApiException;

public class Matrix
{
    final int NUM_MATCHES = 99;
    final int NUM_TEAMS = 59;
    double[][] matches = new double[2 * NUM_MATCHES][NUM_TEAMS];
    double[][] play_matches = new double[NUM_TEAMS][NUM_TEAMS];
    
    public static void main( String[] args )
    {
        BLUE.setAppId( "frc846:scouting_app:v01" );
        double[][] matrix = { { 1, 2, 2 }, { 1, 1, 3 } };
        RREF( matrix );
        printMatrix( matrix );
    }

    public static void setArrays() throws BLUEApiException
    {
        BLUE.Events.Event event = BLUE.Events.getEvent( "casj", 2013 );
        BLUE.Teams.Team[] teams = event.getTeams();
        BLUE.Matches.Match[] matches = event.getMatches();
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