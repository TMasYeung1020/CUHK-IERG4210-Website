# A4B2.py, ICP, 2021
# FINISHED by Yeung Man, 20189102, CL13

def readRunnerFile(fName):
    f_Name = open(fName, "r")
    return(f_Name.readlines())

def writeRunnerFile(fName, rList):
    with open(fName, "w") as OutFile:
        for Record in rList:
            OutFile.write(Record)
    OutFile.close()
    
def appendRunnerRecord(rList, id, name, pbt):
    rList.append(str(id)+","+name+","+str(pbt)+"\n")
    
def avgPBT(rList):
    bTimes=[]
    for items in rList:
        bTimes.append(float(items.split(",")[2]))
    return sum(bTimes)/len(bTimes)
    
    
