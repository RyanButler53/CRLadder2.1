from matplotlib import pyplot as plt
import matplotlib
import seaborn as sns
import pandas as pd
import numpy as np
import argparse

#Can I make a custom colormap and get 14 individually spaced colors from it? Do I even want that? 
COLORS = ["fuchsia", "mediumturquoise", "steelblue",  "darkorchid", "goldenrod", "peru", "yellowgreen","seagreen","rosybrown", "mediumblue","darkcyan", "indigo", "deeppink", "deepskyblue", "crimson"]
TITLES = ["King Tower", "Card Level", "Wins", 'Losses',  "Games Played", "Net Level Advantage", "Net Advantage per Match",  
"Total Level Difference", "Level Difference per Match", "Personal Best", "Party Pct", "Time In Queue"]
#COLS = [2, 3, 4, 5, 7, 8, 10, 11, 12]
ARRANGEMENTS = {1:(1,1), 2:(1,2), 3:(1,3), 4:(2,2), 5:(2,3), 6:(2,3), 7:(3,3), 8:(3,3), 9:(3,3), 10:(3,4), 11:(3,4), 12:(3,4)}
def smoothData(data, smoothFactor):
    """Smoothes data by averaging smoothFactor data points """
    smoothed = []
    for i in range(len(data) - smoothFactor):
        sum = 0
        for nextInd in range(i,i+smoothFactor):
            sum += data[nextInd]
        smoothed.append(sum/smoothFactor)
    return smoothed

parser = argparse.ArgumentParser(description= "Graph The Data")
parser.add_argument("data", help = "File to graph")
parser.add_argument("-k" , "--kingTower", help="Graphs data for selected KTs",nargs='*', type = int, choices=list(range(1, 15)), default = range(1,15))
parser.add_argument("-c", "--cols", help="List of columns to graph", nargs='*', type = int, choices = list(range(1,13)), default=range(1, 13))
parser.add_argument("-s", "--save", help = "Save figure to file. Default: data file + \".graph.png\"")


args = parser.parse_args()
filename = args.data
kingTowers = args.kingTower
columns = args.cols
saveFigure = args.save
columns = list(set(columns))
kingTowers = list(set(kingTowers))
columns.sort()
kingTowers.sort()

if kingTowers != None:
    print(f"Showing Graphs for King Tower {kingTowers}")
if columns != None:
    print(f"Graphing columns {columns}")
if saveFigure == None:
    saveFigure = filename + ".graph"
if filename[-7:] != "results":
    print("Adding .results to filename")
    filename += ".results"


playersByKing = {x:[] for x in range(1, 15)} 
trophies, kts = [],[]
try:
    with open(filename, "r") as f:
        print(f"Opened File named {filename}")
        allData = f.readlines()
        for player in allData:
            asString = player.split()
            if asString[5] != '0': #they played at least 1 game
                playerData = [eval(x) for x in asString]
                kts.append(playerData[1])
                trophies.append(playerData[0])
                #Playerdata[1]is the king tower.  Separate by KT
                playersByKing[playerData[1]].append(playerData)
except FileNotFoundError as error:
    print(f"Unable to open file: {error}")
print("Read in data")
{value.sort() for value in playersByKing.values()}
numCols = len(columns)

graphRows, graphCols = ARRANGEMENTS[numCols]
fig, axes = plt.subplots(nrows = graphRows, ncols = graphCols, figsize = (14, 9))
plt.tight_layout()

colIndex = 0
for r in range(graphRows):
    for c in range(graphCols):
        for kt in kingTowers:
            #print(f"Graphing King tower {kt} with column {TITLES[columns[colIndex]-1]}")
            if colIndex >= numCols:
                
                axes[r,c].axis('off')
            else:
                minTrophies = playersByKing[kt][0][0]
                maxTrophies = playersByKing[kt][-1][0]
                ind = 0 
                xCoords, avgData, upperErr, lowerErr = [],[], [], []
                for trophyCount in range(minTrophies, maxTrophies+1): #include maximum
                    statsAtCount = []
                    while ind != len(playersByKing[kt]) and playersByKing[kt][ind][0] == trophyCount:
                        statsAtCount.append(playersByKing[kt][ind][columns[colIndex]])
                        ind += 1
                    if len(statsAtCount) != 0:
                        avg = np.average(statsAtCount)
                        stdev = np.std(statsAtCount)
                        xCoords.append(trophyCount)
                        avgData.append(avg)
                        upperErr.append(avg + stdev)
                        lowerErr.append(avg - stdev)
                    else:
                        pass
                xCoords = xCoords[:-30]
                avgSm = smoothData(avgData, 30)
                lowerErrSm = smoothData(lowerErr, 30)
                upperErrSm = smoothData(upperErr, 30)
                p, = axes[r,c].plot(xCoords, avgSm, color = COLORS[kt-1], zorder = 2, label = f"KT {kt}")
                axes[r,c].fill_between(xCoords, lowerErrSm, upperErrSm, color = COLORS[kt-1], alpha = 0.25, zorder = 1)
                axes[r,c].set_title(TITLES[columns[colIndex]-1])
        colIndex += 1
        

plt.subplots_adjust(left=0.05, bottom=0.05,right=0.95,top=0.95,wspace=0.3, hspace=0.3)
handles, labels = axes[0,0].get_legend_handles_labels()
fig.legend(handles, labels, loc = 'lower right')

graphfilename = saveFigure + ".png"
plt.savefig(graphfilename)
print(f"Stats Figure Done. Saved to {graphfilename}")

#Make Histogram
plt.figure(2, figsize=(10, 10))

histColors = matplotlib.cm.get_cmap('viridis', 14)
plt.tight_layout()
sns.histplot(x = trophies, stat = 'count', multiple = 'stack', hue = kts, palette=histColors)
plt.xlabel = "Trophies"
plt.title = f"Trophy Distribution of {len(trophies)} players"

filename = saveFigure + "Hist.png"
plt.savefig(filename)
print(f"Histogram Done.  Saved to {filename}")
plt.show()