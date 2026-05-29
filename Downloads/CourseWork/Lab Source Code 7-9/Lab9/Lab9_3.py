# Lab9_3.py, ICP, 2021
# Use main() function approach
import os.path
import random

# Function writeBinData(), for writing binary data to the binary file
def writeBinData(fname):
    randNumList = []
    for i in range(30):
        randNumList.append(random.randint(0,255))
    byteData = bytes(randNumList)
    
    outFile = open(fname, "wb")
    outFile.write(byteData)
    outFile.close()

    print("OUTPUT BINARY file writing finished OK!  Data below:")
    print(randNumList)
    
# Function readdata(), for reading data from the text file
def readBinData(fname):
    inFile = open(fname, "rb")
    bytes = inFile.read()
    inFile.close()
    print("\n\nThe binary data READ OK, and data (in a form of List) are: ")
    print( list( bytes ) )

# Function main() for inputting file name and checking file existence
def main():
    fileName = input("Enter an OUTPUT BINARY file name for writing: ")
    writeBinData(fileName)
    readBinData(fileName)
    input("ENTER to exit...")

# Call and execute for main()
main()
