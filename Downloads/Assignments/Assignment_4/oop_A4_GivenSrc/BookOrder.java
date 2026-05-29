// BookOrder.java, For A2, OOP, 2022
// SAMPLE SOLUTION, Reference ONLY
// Also given for A4. DO NOT MODIFY THIS

/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */
 
public class BookOrder { // declare a class, name BookOrder
// Fields
    int bID;
    String bTitle;
    double bPrice;
    StudentInfo bStudent;
    
// Constructor    
    public BookOrder(int id, String title, double price, StudentInfo student){
        bID = id;
        bTitle = title;
        bPrice = price;
        bStudent = student;
    }
    
// Methods
    public void dispBOInfo(){
        String binfo = "Order ID: " + bID + "\n"
            + "Title: " + bTitle + "\n"
            + "Price(HKD): " + bPrice + "\n"
            + "Student: " + bStudent.getSInfo() + "\n";
            
        System.out.println(binfo);
    }
    
}
