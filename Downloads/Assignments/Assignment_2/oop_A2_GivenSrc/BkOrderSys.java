// BkOrderSys.java, For A2, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */

import java.util.Scanner;  // explicit Scanner import
import java.util.*; // wildcard Scanner import

public class BkOrderSys { // declare a class, name PurchaseSys

    public static StudentInfo addNewFTStudent(){  // generate and return a new student, with user input
    Scanner FtInput = new Scanner(System.in); // Scanner for full time student input
    System.out.println("--- Enter a new Full-Time student in the form <ID,Name,Phone>:"); 
    String FtInfo = FtInput.nextLine(); // Input by fulltime student
    String SthInfo[] = FtInfo.split(","); // from sentence to 3 parts, divided by ","
    int SID = Integer.valueOf(SthInfo[0]); // tranfer SID to int
    int SPHONE = Integer.valueOf(SthInfo[2]); // tranfer SPHONE to int
    StudentInfo NStudent = new StudentInfo(SID,SthInfo[1],SPHONE,false); // Create new student
    FtInput.close(); // Close the scanner
    return NStudent; // Return record of new student
    }

    public static void main(String[] args){ // main method, entry point to start program execution
		StudentInfo Studenta = new StudentInfo(20189102,"Yeung Man",55551111,false); // New varieble
    StudentInfo Studentb = new StudentInfo(20214321,"CHOW Betty",88888888,true); // New varieble
    StudentInfo Studentc = new StudentInfo(20215678,"AU Candy",12000000,false); // New varieble
    BookOrder Book1 = new BookOrder(901, "Seven Little Plays", 225, Studentb); // New varieble
    BookOrder Book2 = new BookOrder(902, "Treasure Island", 126.5, Studenta); // New varieble
    System.out.println("--- Book Order System, A2, OOP, 2022 ***");
    Book1.dispBOInfo(); // Display booking 1
    Book2.dispBOInfo(); // Display booking 2

    StudentInfo Studentd = addNewFTStudent(); // Let new student to be Studentd
    BookOrder Book3 = new BookOrder(901, "Seven Little Plays", 225, Studentd); // Booking of studentd
    System.out.println("--- Updated Order:");
    Book3.dispBOInfo(); // Display booking of Studentd
    System.out.println("\n>>> END of main(). Done by Yeung Man, 20189102, CL06, 2022 <<<");
 
	}
    
}
