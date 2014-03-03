import java.io.*;
import java.util.*;

public class opr {
    void run() throws Exception {

        int numC = nextInt(); // line 1: number of COLUMNS (num of teams)
        int numR = nextInt(); // line 2: number of ROWS (num matches * 2)
        String teams = next(); // line 3: team numbers separated by commas no
                               // space
        int[][] table = new int[numR][numC];
        int[] teamList = new int[numC];

        teamList = parseThing(teams, numC);
        // for(int i = 0; i < teamList.length; i++)
        // {
        // System.out.println("team " + (i+1) + ": " + teamList[i]);
        // }

        // all teams in the array now

        String players = next();

        table = zeroAndOneThing(teamList, numR, numC, players);

        int[][] table2 = new int[numC][numC];
        
        table2 = categorize(table,teamList);
        
        for (int i = 0; i < numC; i++) {

            for (int j = 0; j < numC; j++) {

                System.out.print(table2[i][j] + "\t");

            }
            System.out.println();
        }
        
        
        
        out.print(0);

        in.close();
        out.close();
    }
    
    public int[][] categorize(int[][] ref, int[] teamList)
    {
        int[][] table = new int[ref[0].length][ref[0].length];
        for(int c = 0; c < ref[0].length; c++)
        {
            
            for(int i = 0; i < ref.length;i++)
            {
                
                if(ref[i][c]==1)
                {
                    
                    for(int j = 0; j < ref[i].length;j++)
                    {
                        if(ref[i][j]==1)
                        {
                            table[c][j] ++;
                            System.out.println(c +"  " + j);
                        }
                        
                    }
                    
                }
                
                
            }
            
        }
        
        return table;
        
    }

    public int[][] zeroAndOneThing(int[] teamList, int numR, int numC,
            String players) {

        int[][] binaryThing = new int[numR][numC];

        int[] list = new int[numR * 3];
        list = parseThing(players, list.length);
//        for (int i = 0; i < list.length; i++) {
//            System.out.println("team " + (i + 1) + ": " + list[i]);
//        }
        int threes = 0;
        int index = 0;
        int row = 0;
        while (index < list.length) {
            if (threes != 3) {
                int placePos = positionItAt(list[index], teamList);
                binaryThing[row][placePos] = 1;
//                 System.out.println(list[index] + ", row: " + row + ", col: "
//                 + placePos +", threes: " + threes);

                threes++;
                index++;
            } else {
                threes = 0;

                row++;
            }
        }

        return binaryThing;

    }

    public int positionItAt(int findThis, int[] teamList) {
        int index = 0;
//        System.out.println(findThis);
        for (int i = 0; i < teamList.length; i++) {
            if (findThis == teamList[i]) {
                index = i;
                break;
            }
        }

        return index;

    }

    public int[] parseThing(String teams, int num) {
        int[] list = new int[num];
        int start = 0;
        int index = 0;
        for (int i = 0; i < teams.length(); i++) {

            if (teams.charAt(i) == ',') {
                String str = teams.substring(start, i);
                int n = Integer.parseInt(str);
                list[index] = n;
                start = i + 1;
                index++;
            }

        }
        return list;
    }

    public static void main(String args[]) throws Exception {
        new opr("opr.in", "opr.out").run(); // read from
                                            // stdin/stdout
        // new Main("prog.in", "prog.out").run(); // or use file i/o
    }

    BufferedReader in;
    PrintStream out;
    StringTokenizer st;

    opr() throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = System.out;
    }

    opr(String is, String os) throws Exception {
        in = new BufferedReader(new FileReader(new File(is)));
        out = new PrintStream(new File(os));
    }

    int nextInt() throws Exception {
        return Integer.parseInt(next());
    }

    String next() throws Exception {
        while (st == null || !st.hasMoreTokens()) {
            st = new StringTokenizer(in.readLine());
        }
        return st.nextToken();
    }
}