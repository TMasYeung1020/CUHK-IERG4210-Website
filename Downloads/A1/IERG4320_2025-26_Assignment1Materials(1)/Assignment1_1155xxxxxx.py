# <Your student ID>
import numpy as np
import pandas as pd
from scipy.stats import chisquare, wilcoxon, ttest_rel

# Problem 2
def problem_2(df):
    row_mean = 0
    row_sd = 0
    column_sample_mean = 0
    column_ss = 0
    # write your logic here

    
    return row_mean, row_sd, column_sample_mean, column_ss


# Problem 3
def problem_3(list_of_observation):
    p = 0
    chi2 = 0
    # write your logic here


    return p, chi2


# Problem 4
def problem_4(df):
    pairt = w = 0
    # write your logic here


    return pairt, w


if __name__ == "__main__":
    # Testing: Problem 2
    df = pd.read_csv('problem2.csv', sep=',', header=None)
    print(problem_2(df))


    # Testing: Problem 3
    observation = [3,5,3,6,7,8,3,5,5,2,4,4,5,2,8,7,5,5,5,3]
    p, chi2 = problem_3(observation)
    print("p-value :", p)
    print("chi-square :", chi2)
    
    
    # Testing: Problem 4
    df = pd.read_csv('problem4.csv', sep=',', header=None)
    pairt, w = problem_4(df)
    print("p-value from paired sample T-test: ", pairt)
    print("p-value from wilcoxon signed-ranked test with T-statistics: ", w)

