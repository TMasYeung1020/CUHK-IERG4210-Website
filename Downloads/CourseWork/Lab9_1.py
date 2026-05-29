#Lab9_1.py

def writedata(fname):
    outFile = open(fname, "w")
    for student in range(3):
        print("Enter record for", student + 1, "[1.Name and 2. Mark:]")
        stud = input("Student name: ")
        icpr = input("Mark for ICP: ")

        outFile.write(stud + "\n")
        outFile.write(icpr + "\n")

    outFile.close()
    print("OUTPUT file writing finished OK!")

def main():
    File_Name = str(input("Enter a OUTPUT file name for writing:"))
    writedata(File_Name)

main() 
