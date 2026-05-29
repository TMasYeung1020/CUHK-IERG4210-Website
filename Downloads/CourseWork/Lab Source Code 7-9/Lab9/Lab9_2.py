# Lab9_2.py, ICP, 2021
# Use main() function approach
import os.path
# Function readdata(), for reading data from the text file
def readdata(fname):
    totalS = 0
    totalM = 0
    inFile = open(fname, "r")
    lineList = inFile.readlines()
    inFile.close()
    print("The student records include: ")
    for i in range(0,len(lineList),2):
        stud = lineList[i]
        icpr = int(lineList[i+1])
        totalS +=1
        totalM += icpr
        print("-Student Name: ", stud, end="")
        print("  Mark for ICP: ", icpr)
    print(f"The average of ICP for {totalS} students is {totalM/totalS:.1f}")

# Function main() for inputting file name and checking file existence
def main():
    fileName = input("Enter a INPUT file name for reading:")
    if os.path.exists(fileName):
        print(fileName,"exists in the current directory")
    readdata(fileName)

# Call and execute for main()
main()
