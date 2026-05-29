public class ExtraBookOrder extends BookOrder{
    // One Field
    protected BookOrder orgBookOrder;

    // Two Constructor
    private ExtraBookOrder(int id, String title, double price, StudentInfo student) {
        super(id, title, price, student);
    }
    public ExtraBookOrder(int id, String title, double price, BookOrder orgord){
        this(id, title, price, orgord.bStudent);
        this.orgBookOrder = orgord;
    }

    // One Overriding method "dispBOInfo()"
    public void dispBOInfo(){
        double TotalPrice = orgBookOrder.bPrice + this.bPrice;
        String binfo = "Order ID: " + this.bID + "\n"
            + "Title: " + this.bTitle + "\n"
            + "Price(HKD): " + this.bPrice + "\n"
            + "Student: " + this.bStudent.getSInfo() + "\n";
            System.out.println(".. An EXTRA Order, with Original Order ID: " + orgBookOrder.bID);
            System.out.println("... Total Amount: " + TotalPrice);
            System.out.println(binfo);
    }


    // Display a message of self-testing
    // 3 StudentInfo objects in Table 1(a)
    public static void main(String args[]){
        System.out.println("*** ONLY FOR SULF-TESTING, ExtraBookOeder ***");
        StudentInfo Student1 = new StudentInfo(20210001, "CHAN Tai Man", 98765432, false);
        StudentInfo Student2 = new StudentInfo(20214321, "CHOW Betty", 88888888, true);
        StudentInfo Student3 = new StudentInfo(20215678, "AU Candy", 12000000, false);
        BookOrder[] NewBookOrder = new BookOrder[4];
        BookOrder BKo1 = new BookOrder(901, "Seven Little Plays", 225.0, Student2);
        BookOrder BKo2 = new BookOrder(902, "Treasure Island", 126.5, Student1);
        BookOrder BKo3 = new ExtraBookOrder(1231, "King Kong", 90.5, BKo1);
        BookOrder BKo4 = new ExtraBookOrder(1232, "The Explorer", 173.5, BKo2);
        NewBookOrder[0] = BKo1;
        NewBookOrder[1] = BKo2;
        NewBookOrder[2] = BKo3;
        NewBookOrder[3] = BKo4;
        for (int i = 0; i < 4; i++){
            BookOrder NBO = NewBookOrder[i];
            NBO.dispBOInfo();
        }
        System.out.println("\n>>> END of main(). Done by Yeung Man, 20189102, CL06, 2022 <<<");
    }
    
}
