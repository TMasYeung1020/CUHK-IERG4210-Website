// A1B1.java, For A1, OOP, 2022
// SAMPLE Solution, REFERENCE ONLY
/**
 * @author Student: CHAN Tai Man, 21124025, CL02, 2022
 */

import java.util.Scanner;
import java.util.Calendar;

public class A1B1 { // declare a class, name A1B1

    public static void showWelcome(){  // DO NOT MODIFY the method header
        // 1. METHOD BODY TO BE FINISHED BY STUDENT
        System.out.println(" Welcome!");
    }
    
    public static String getCurTime(){ // GIVEN, DO NOT MODIFY
        Calendar c = Calendar.getInstance(); // current date & time as Calendar
        String curTimeStr = c.get(Calendar.HOUR_OF_DAY) + ":"
                        + c.get(Calendar.MINUTE) + ":"
                        + c.get(Calendar.SECOND);
        return curTimeStr;
    }
    public static int[] getRaNum(){  // DO NOT MODIFY the method header
        // 2. METHOD BODY TO BE FINISHED BY STUDENT
        int totNum=5, min=21, max=98;
        int[] retN = new int[totNum];
        for (int i = 0; i < retN.length; i++)
            retN[i] = (int)(min + Math.random() * (max - min + 1)); // random number
        return retN;
    }
    
    public static void dispMenu(){  // DO NOT MODIFY the method header
        // 3. METHOD BODY TO BE FINISHED BY STUDENT
        String menuStr = "\n>> Options (Integer only):\n"
        + "1: Say Welcome\n"
        + "2: Check Time\n"
        + "3: Random Numbers\n"
        + "0: Exit the program\n"
        + "  .. Select an options: ";
        System.out.print(menuStr);
    }
    
    public static void main(String[] args){ // main() method, the entry point where the program starts
        System.out.println("--- START of A1B1 ---");
        // 4. METHOD BODY TO BE FINISHED BY STUDENT
        Scanner scanner = new Scanner(System.in);// Scanner scans from standard input
        boolean isExit = false;
        while (!isExit){
            dispMenu();
            int opt = scanner.nextInt(); // scan as an integer
            switch (opt){
                case 0:
                    isExit = true;
                    break;
                case 1:
                    showWelcome();
                    break;
                case 2:
                    System.out.println(" Current Time is " + getCurTime() );
                    break;
                case 3:
                    int [] raNum = getRaNum();
                    System.out.println(" Random numbers: " + java.util.Arrays.toString(raNum) );
                    break;
                default:
                    System.out.println(" UNKNOWN option <" + opt + ">.. try again!");
            }
        }
        
        System.out.println("\n>>> END of main(). Done by CHAN Tai Man, 21124025, CL02, 2022 <<<");                
    }
}