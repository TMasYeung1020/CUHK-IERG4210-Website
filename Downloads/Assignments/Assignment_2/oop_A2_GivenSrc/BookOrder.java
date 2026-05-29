// BookOrder.java, For A2, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */

public class BookOrder{ // Declare the Class
    int bID; // ID of the order
    String bTitle; // Title of book
    double bPrice; // Price(HKD) of this order
    StudentInfo bStudent; // Who place this order?

    public BookOrder(int id, String title, double price, StudentInfo student){ // Constructor
        bID = id;
        bTitle = title;
        bPrice = price;
        bStudent = student;
    }
    public void dispBOInfo(){ // Print the order list out
        System.out.println("Order ID: " + bID +"\nTitle: " + bTitle + "\nPrice(HKD): " + bPrice + "\nStudent: " +  bStudent.getSInfo());
        System.out.println(" "); // Left one space
    }
}

