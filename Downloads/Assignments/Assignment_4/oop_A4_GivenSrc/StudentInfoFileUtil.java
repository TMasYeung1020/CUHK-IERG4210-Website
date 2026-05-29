// StudentInfoFileUtil.java, For A4, OOP, 2022
// Given: To be FINISHED by student
/**
 * @author Student: Yeung Man, 20189102, CL06, 2022
 */
 
import java.io.*;
import java.util.ArrayList;
import javax.management.modelmbean.InvalidTargetObjectTypeException;
public class StudentInfoFileUtil { // declare a class
    
    public static StudentInfo[] readStudFile(String rStudFile){  // DO NOT MODIFY the method header
        StudentInfo[] StsInfoWithSINFO = new StudentInfo[5];
        int i = 0;
        int j;
        Boolean xyz = false;
        ArrayList StuList = new ArrayList();
        try {
            String Line = "";
            BufferedReader StuBr = new BufferedReader(new FileReader(rStudFile));
            while((Line = StuBr.readLine()) != null){
                String[] values = Line.split(",");
                for(j = 0; j < values.length; j++){
                    xyz = false;
                    StuList.add(values[j]);
                    if(StuList.contains("PartTime")){
                        xyz = true;
                    }
                    StuList.clear();
                }
                StsInfoWithSINFO[i] = new StudentInfo(Integer.valueOf(values[0]), values[1], Integer.valueOf(values[2]), xyz);
                i += 1;
                }
                StuBr.close();
        }
        catch (FileNotFoundException FNFE) {
            FNFE.printStackTrace();
        }
        catch (IOException IOE) {
            IOE.printStackTrace();
        }
        return StsInfoWithSINFO;
    }
    
    public static void writePTStudFile(StudentInfo[] sArr, String wSFile){  // DO NOT MODIFY the method header
        try {
            PrintWriter Ostr = new PrintWriter(wSFile);
            for (int i = 0; i < sArr.length; i++){
                StudentInfo Arrrr = sArr[i];
                if (Arrrr.sPT == true){
                    Ostr.println(sArr[i]);
                }
            }
            Ostr.close();
        }
        catch (FileNotFoundException FNFE){
            FNFE.printStackTrace();
        }
    }
    
    public static void writeStudObjectFile(StudentInfo[] sArr, String wSObjF){  // DO NOT MODIFY the method header
        try{
            FileOutputStream outFileStream = new FileOutputStream(wSObjF);
            ObjectOutputStream outObjectStream = new ObjectOutputStream(outFileStream);
                for (int i = 0; i < sArr.length; i++){
                    outObjectStream.writeObject(sArr[i]);
                }
            outObjectStream.close();
        }
        catch (IOException IOE) {
            IOE.printStackTrace();
        }
    }
    
    public static StudentInfo[] readStudObjectFile(String rSObjF){  // DO NOT MODIFY the method header
        StudentInfo[] result = null;
        try{
            FileOutputStream OUT1 = new FileOutputStream(rSObjF);
            ObjectOutputStream OUT2 = new ObjectOutputStream(OUT1);
        }
        catch(Exception e){
            e.printStackTrace();
        }
        return result;
}
}