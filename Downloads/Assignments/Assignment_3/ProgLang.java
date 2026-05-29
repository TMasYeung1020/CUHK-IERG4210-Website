public class ProgLang {
    String pName;
    protected int pCode;
    ProgLang(String inName, int inCode){
        pName = inName;
        pCode = inCode;
    }
    public String[] getInfo(){
        String [] Arr = new String[2];
        Arr[0] = pName;
        Arr[1] = Integer.toString(pCode);
        return Arr;
    }
    public void dispInfo(){
        System.out.println("P Name: "+pName+"; P Code: "+pCode);
    }
    public static void main(String args[]){
        ProgLang Hello = new ProgLang("Python", 4020);
        Hello.dispInfo();
    }
}
