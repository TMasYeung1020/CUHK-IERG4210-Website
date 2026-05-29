// A1B2.java, For A1, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */

import java.util.Scanner;

public class A1B2 { // declare a class, name A1B2
    public static void main(String[] args){ // main() method, the entry point where the program starts
        int maxvalue = 201; // Set the max value to 201
        int minvalue = 2; // Set the min value to 2
        System.out.println("--- START of A1B2, The Guessing Game  ---\n"); // Start the Game
        System.out.println("Guess a number between " + minvalue + " to " + maxvalue +", max number of attempts is 8");
        int ranNum = 123; // Generate ranNum "123"
            for (int i = 0; i < 8; i++){ //Only Try 8 Time 
                System.out.print("Attempt " + (i + 1) + ": Guess a random number <between " + minvalue + " to " + maxvalue +">, then <ENTER>: ");
                Scanner number = new Scanner(System.in); // Let the user enter
                int TheGuess = number.nextInt(); // The user input defined as "TheGuess"
                    if (ranNum == TheGuess){ // The condition of Guessing the right value, and finish the game
                        System.out.println(".. WELL DONE! This is the random number [ " + ranNum + " ]... QUIT!");
                        break;
                        } 
                        else if (ranNum > TheGuess){ // The condition of Guessing the Smaller value
                        System.out.println(".. The number you typed is [ " + TheGuess + " ] - Too SMALL, guess the number again!");
                        } 
                        else if (ranNum < TheGuess){ // The condition of Guessing the Greater value
                        System.out.println(".. The number you typed is [ " + TheGuess + " ] - Too LARGE, guess the number again!");
                        }
                    if (i == 7){ // Game over if attempted 8 times
                        System.out.println(".. FAIL! Max Attempt reached! Our random number is [ " + ranNum + " ]... QUIT!");
                        }    
                    }
        System.out.println("\n>>> END of main(). Done by Yeung Man, 20189102, CL06, 2022 <<<");            
    }   
}