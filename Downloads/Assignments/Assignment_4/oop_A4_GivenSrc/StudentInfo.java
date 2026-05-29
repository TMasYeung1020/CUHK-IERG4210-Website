// StudentInfo.java, For A2, OOP, 2022
// SAMPLE SOLUTION, Reference ONLY
// Also given for A4. DO NOT MODIFY THIS

/**
 * @author Student: <StudentName>, <SID>, <Class>, <Year>
 */
import java.io.Serializable; 

public class StudentInfo implements Serializable { // declare a class, name StudentInfo
// Fields
    int sID;
    String sName;
    int sPhone;
    boolean sPT;
    
// Constructor    
    public StudentInfo(int id, String name, int phone, boolean pt){
        sID = id;
        sName = name;
        sPhone = phone;
        sPT = pt;
    }
    
// Methods
    public boolean isValidPhone(){
        if (sPhone<20000000 || sPhone > 99999999)
            return false;
        else return true;
    }
    
    public String getSInfo(){
        return sID + "," + sName + "," + sPhone
            + (sPT?",PartTime":"");
    }
    
}
