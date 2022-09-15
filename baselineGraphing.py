from matplotlib import pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np #fast math
#include

#global vars that should be inputable. Later
# Big Data File"../CRScale.txt" 
FILENAME = "ladderSample/1%Scale.txt" #"ladderSample/CRScale.txt"
MAXPLAYERS = 7500000 #10% scale of CR
COLORS = ["fuchsia", "mediumturquoise", "steelblue",  "darkorchid", "goldenrod", "peru", "yellowgreen","seagreen","rosybrown", "mediumblue","darkcyan", "indigo", "deeppink", "deepskyblue", "crimson"]

playersByKing = {x:[] for x in range(1, 15)} 

def smoothData(data, smoothFactor):
    """Smoothes data but averaging smoothFactor data points """
    smoothed = []
    for i in range(len(data) - smoothFactor):
        sum = 0
        for nextInd in range(i,i+smoothFactor):
            sum += data[nextInd]
        smoothed.append(sum/smoothFactor)
    return smoothed

def main():
    with open(FILENAME, "r") as f:
        allPlayers = f.readlines()
        print("Read Data")
        numPlayers = min(len(allPlayers), MAXPLAYERS)
        for i, player in enumerate(allPlayers[:numPlayers]):
            if i % (numPlayers//100) == 0:
                print(f"{i} players read in")
            stats = [eval(s) for s in player.split()]
            king = stats[0]
            #filter players by king in linear time
            playersByKing[king].append(stats[1:])

    print("Loaded Data")
    #X axis: Trophies
    #Y axis: Card level, Skill. start with cards
    {value.sort() for value in playersByKing.values()}
    print("Sorted Data")
    
    fig, ax = plt.subplots(figsize = (10, 6))
    title = f"Card Level vs Trophies for all King Towers"

    #need column for data. 
    col = 2 #card lvl
    for kt in range(1, 15):
        minTrophies = playersByKing[kt][0][0]
        maxTrophies = playersByKing[kt][-1][0]
        ind = 0 #the list is sorted.  Use that!
        xCoords, avgData, upperErr, lowerErr = [],[], [], []


        for trophyCount in range(minTrophies, maxTrophies+1): #include maximum
            statsAtCount = []
            while ind != len(playersByKing[kt]) and playersByKing[kt][ind][0] == trophyCount:
                statsAtCount.append(playersByKing[kt][ind][col])
                ind += 1
            #calculate avg and stdev
            if len(statsAtCount) != 0:
                avg = np.average(statsAtCount)
                stdev = np.std(statsAtCount)
                xCoords.append(trophyCount)
                avgData.append(avg)
                upperErr.append(avg + stdev)
                lowerErr.append(avg - stdev)
            else:
                pass

        print(f"Finished Averages for KT {kt}")
        xCoords = xCoords[:-30]
        avgSm = smoothData(avgData, 30)
        lowerErrSm = smoothData(lowerErr, 30)
        upperErrSm = smoothData(upperErr, 30)
        #print(avgData)
        p, = ax.plot(xCoords, avgSm, color = COLORS[kt-1], zorder = 2)
        ax.fill_between(xCoords, lowerErrSm, upperErrSm, color = COLORS[kt-1], alpha = 0.25, zorder = 1)
        print(f"Plotted KT {kt}")
    print("graphing done")
    legend = ax.legend(labels = [f"King Tower {lvl}" for lvl in range(1,15)], loc = "lower right")
    plt.title(title)
    ax.set_ylabel("CL")
    ax.set_xlabel("Trophies")
    plt.show()



def histPlot():
    

#run script
main()