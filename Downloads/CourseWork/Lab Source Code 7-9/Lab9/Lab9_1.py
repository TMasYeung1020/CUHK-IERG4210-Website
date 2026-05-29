# Lab9_1.py, ICP, 2021
# Use main() function approach
import os.path

# Function writedata(), for writing data to the text file
def writedata(fname):
    outFile = open(fname, "w")
    for student in range(3):
        print("Enter record for",student + 1,"[1.Name and 2.Mark:]")
        stud = input("Student name: ")
        icpr = input("Mark for ICP: ")

        outFile.write(stud + "\n")
        outFile.write(icpr + "\n")
    
    outFile.close()
    print("OUTPUT file writing finished OK!")
    
# Function main() for inputting file name and checking file existence
def main():
    fileName = input("Enter a OUTPUT file name for writing: ")
    if os.path.exists(fileName):
        print(fileName,"exists in the current directory")
    writedata(fileName)

# Call and execute for main()
main()
