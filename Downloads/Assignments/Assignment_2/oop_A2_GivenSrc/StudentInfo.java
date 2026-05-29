// StudentInfo.java, For A2, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */
 
public class StudentInfo { // declare a class, name StudentInfo
    int sID; // Field Student ID
    String sName; // Field Student Name
    int sPhone; // Field Student Phone Numbers
    boolean sPT; // Field Student type
    
    public StudentInfo(int id, String name, int phone, boolean pt){ // Constructor
        sID = id;
        sName = name;
        sPhone = phone;
        sPT = pt;
    }
    public Boolean isValidPhone(){ // Method 1
        boolean NumOfPhone = true; // Set Variable
        if (sPhone < 20000000){ // Invaild if lower than 20000000
            NumOfPhone = false;
        }
        else if (sPhone > 99999999){ // Invaild if higher than 99999999
            NumOfPhone = false;
        }
        return NumOfPhone; // Invaild or Vaild
    }

    public String getSInfo() { // Method 2
        String StuInFo = sID +","+ sName + ","+ sPhone; // With no "part time"
        if (sPT == true){ // With "part time"
            StuInFo = sID +","+ sName + ","+ sPhone +",PartTime";
        }
        return StuInFo; // Part time or Nothing
    }
}
