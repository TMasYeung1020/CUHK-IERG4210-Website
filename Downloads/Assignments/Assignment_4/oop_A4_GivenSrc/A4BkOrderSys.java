// A4BkOrderSys.java, For A4, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */

public class A4BkOrderSys { // declare a class
    public static final String STUD_FILE = "A4BStudents.txt";  // input text file
    public static final String PTSTUD_FILE = "A4BPTSts.txt";  // output text file, PT only
    public static final String PTSTUD_OBJ_FILE = "A4BPTStObjects.obj";  // output Object file, PT only
        
    public static void main(String[] args){ // main method, entry point to start program execution
        System.out.println("--- A4 Book Order System, A4, OOP, 2022 ***");
        System.out.println("-1. READ (& Display) Student Text File: " + STUD_FILE);
        StudentInfo[] sArr = StudentInfoFileUtil.readStudFile(STUD_FILE);
        for (int i=0; i<sArr.length; i++)
            System.out.println(sArr[i].getSInfo());
               
        System.out.println("\n--2. WRITE PT Student Text File: " + PTSTUD_FILE);
        StudentInfoFileUtil.writePTStudFile(sArr, PTSTUD_FILE);
        System.out.println(" -- FINISHED: WRITE PT Student Text File: " + PTSTUD_FILE);
        
        System.out.println("\n---3. READ PT Student text File: " + PTSTUD_FILE 
							+ "; and WRITE PT Student Object File: " + PTSTUD_OBJ_FILE);
        StudentInfo[] sPT = StudentInfoFileUtil.readStudFile(PTSTUD_FILE);
        StudentInfoFileUtil.writeStudObjectFile(sPT, PTSTUD_OBJ_FILE);
        System.out.println(" --- FINISHED: WRITE PT Student Object File: " + PTSTUD_OBJ_FILE);


        System.out.println("\n----4. READ (& Display) PT Student Object File: " + PTSTUD_OBJ_FILE);
        StudentInfo[] rsoF = StudentInfoFileUtil.readStudObjectFile(PTSTUD_OBJ_FILE);      
        System.out.println("\n>>> END of main(). Done by Yeung Man, 20189102, CL06, 2023 <<<");                
    }
    
}
