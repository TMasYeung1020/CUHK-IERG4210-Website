// A1B1.java, For A1, OOP, 2022
// Given: To be FINISHED by student
/**
 * Student: Yeung Man, 20189102, CL06, 2022
 */

import java.util.Scanner;
import java.util.Calendar;

public class A1B1 { // declare a class, name A1B1
    public static void showWelcome(){ // Show Welcome!
        System.out.println(" Welcome!");
    }

    public static String getCurTime(){ // Get the Current Time
        Calendar c = Calendar.getInstance(); // current date & time as Calendar
        String curTimeStr = c.get(Calendar.HOUR_OF_DAY) + ":"
                + c.get(Calendar.MINUTE) + ":"
                + c.get(Calendar.SECOND);
        return curTimeStr;
    }

    public static int[] getRaNum(){ // Get the List of 5 random number
        int[] NumList = new int[5]; // Create the List to save random number
        for (int i = 0; i < 5; i++){ // Generate 5 random number
            int RN = (int)(Math.random() * (98 - 21) + 21); // Set the range from 21 to 98
            NumList[i] = RN; // Generate and install the number
        }
        return NumList;
    }

    public static void dispMenu(){ // Print the Menu out
        System.out.println("\n>> Options (Integer only):\n1: Say Welcome\n2: Check Time\n3: Random Numbers\n0: Exit the program");
    }

    public static void main(String[] args){ // Start Main(), input and output
        System.out.println("--- START of A1B1 ---"); // Print the Title
        Scanner InputNum = new Scanner(System.in); // Setup the Scanner "InputNum" for user input
        boolean x = true;
        while (x){
            dispMenu(); // Show the Menu
            System.out.print("  .. Select an options: "); // Tell User to select the number
            int Num = InputNum.nextInt(); // Let the user input number
            switch(Num){ // Stituation of different case
                case 0: // exit the loop
                    x = false;
                    break;
                case 1: // Print "Welcome!"
                    showWelcome();
                    break;
                case 2: // print the Time out
                    System.out.println(" Current Time is " + getCurTime());
                    break;
                case 3: // Change the List from Arrays type to String, and print the List out
                    System.out.println(" Random numbers: " + java.util.Arrays.toString(getRaNum()));
                    break;
                default: // For the other ANS, just tell user to try again
                    System.out.println(" UNKNOWN option <" + Num + ">.. try again!"); 
                    break;
            }
        }
        InputNum.close(); // close the Scanner
            System.out.println("\n>>> END of main(). Done by Yeung Man, 20189102, CL06, 2022 <<<");
    }
}


