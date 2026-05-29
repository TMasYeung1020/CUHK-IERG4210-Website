import javax.xml.bind.annotation.XmlElement.DEFAULT;

public class FunProgLang extends ProgLang{
    public static int DEF_PCODE = 21102;

    // Call superclass constructor with name and code
    FunProgLang(String name, int code){
        super(name, code);
    }
    // Call superclass constructor with name and code 
    FunProgLang(String name){
        super(name, DEF_PCODE);
    }
    public static void main(String args[]){
        ProgLang First1 = new FunProgLang("Pytthon", 4020);
        ProgLang First2 = new FunProgLang("Java", 4023);
        ProgLang First3 = new FunProgLang("Quantum");
        ProgLang[] Arrr = new ProgLang[]{First1, First2, First3};
        for (int i = 0; i < 3; i++){
            Arrr[i].dispInfo();
        }
    }
}
