// MainTA2B.java, For A2, OOP, 2022
// *** FOR TESTING ONLY, DO NOT MODIFY

public class MainTA2B {
    
    public static void main(String[] args){
        System.out.println("*** START of MAIN TESTING, for A2B ***");
        System.out.println("\n** StudentInfo TESTING **");
        StudentInfo a = new StudentInfo(10011001,"CHAN Tai Man",3322,true);
        StudentInfo b = new StudentInfo(20022002,"CHAN Siu Ming",33333333,false);
        System.out.println(a.getSInfo());
        System.out.println(b.getSInfo());
        
        System.out.println("\n\n** BookOrder TESTING **");
        BookOrder c = new BookOrder(20210, "ABC", 100, a);
        c.dispBOInfo();
        
        System.out.println("\n*** END of MAIN TESTING ***");                
    }
}