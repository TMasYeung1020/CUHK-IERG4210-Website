// A1B2.java, For A1, OOP, 2022
// SAMPLE Solution, REFERENCE ONLY
/**
 * @author Student: CHAN Tai Man, 21124025, CL02, 2022
 */

import java.util.Scanner;

public class A1B2 { // declare a class, name A1B2
    public static void main(String[] args){ // main() method, the entry point where the program starts
        System.out.println("--- START of A1B2, The Guessing Game  ---\n");
        Scanner scanner = new Scanner(System.in);// Scanner scans from standard input        
        int guessNum = 123;  // Supposed to be a randomly generated number.  HERE is for testing
        int maxAttempt = 8;
        System.out.println("Guess a number between 5 to 211, max number of attempts is " + maxAttempt);
        
        int numAttempt = 0;
        while (true){
            numAttempt++;
            System.out.print("Attempt "+ numAttempt + ": Guess a random number <between 5 to 211>, then <ENTER>: ");
            int userGuess = scanner.nextInt(); // scan as an integer
            if (userGuess > guessNum)
                System.out.println(".. The number you typed is [ " + userGuess 
                        + " ] - Too LARGE, guess the number again!");
            if (userGuess < guessNum)
                System.out.println(".. The number you typed is [ " + userGuess 
                        + " ] - Too SMALL, guess the number again!");
            if (userGuess == guessNum){
                System.out.println(".. WELL DONE! This is the random number [ "+ guessNum + " ]... QUIT!");
                break;
            }
            if (numAttempt==maxAttempt){
                System.out.println(".. FAIL! Max Attempt reached! Our random number is [ "+ guessNum + " ]... QUIT!");
                break;
            }                
        }
        
        System.out.println("\n>>> END of main(). Done by CHAN Tai Man, 21124025, CL02, 2022 <<<");                
    }
}