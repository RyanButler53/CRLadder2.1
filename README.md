# CrLadder2.0
Updated and highly optimized version of my original CR ladder simulation

**Note: This project is actively being developed. Project will be set to private soon and a full release will hopefully be out by october. There are issues and incomplete features**

### Features and Key Improvements:

- Significantly improved performance
  - Appears to be 100-1000x speedup over previous simulation
  - Still room for performance optimizations!

- Uses real api data from 24191 players to more accurately guage card level for a corresponding trophy range

- Adds *many* more parameters:
	- Trophy gates can now be added easily
	- Resets can be divided by trophy level and reset percent can be tweaked
	- Card Level caps and Inflation are paired with each separate region of ladder

	   
- Tweaking parameters requires only changing a text file or adding a compiler flag
  - Compile with ```make``` once, run many times
  
- Has functionality to investigate arenas, trophy gates, multiple seasons

- Multiple improvements for reproducability:
    - Each run provides a cfg file with all parameters listed out.  Running ```./main -c FILENAME.cfg``` will build a settings file (called temp.settings) and run the parameters exactly the same as the previously
    - RNG seed can be set with the flag ```-d SEED```
    - Results and config filenames can be customized to track which params were used
    - Graphs default to the data filename + `.graph.png`. 

  
## Quick Start Guide  
  1. Download or clone this git repository to your local machine
  2. In the same directory of this git repo, build the program from the makefile.  On Mac OS, I type ```make main``` in the command line
  3. Run the command ```./main -o firstSim -i settings.txt -p ./ladderSample/25k.txt``` in the terminal. 
  
    There is a file in the ladderSample folder with 25k players to get started. The final status of the players is in the file ```firstSim.results``` and ```firstSim.cfg``` will track your arguments. 
 
 4. If you have python installed, type ```python graphing.py firstSim``` into the command line to run the graphing script.  By default this will show all stats and will not save the file
    - 4a. The script will display plots of 9 different stats for a player.  The plot will by default show stats for all king towers, but```-k <KINGLEVEL>``` flag will show plots for just that one king tower. 
    - The `-c` argument is for each column to graph.  Here is a table of which number corresponds with each stat (This order is found in the `resultsToFile` function in `main.cpp`

 **Report bugs on the issues page or on discord @Nova Light#2565. (With the space)**
 
## Using the software
Some more detailed notes about how this softare can be used: 
The main executable has several other command line arguments. Run ```./main -h``` to see how to use them.  
The format of the settings file can be found in ```settings-format.txt```. You can also use a config file to load in settings. It will output them to ```temp.settings``` so they can be used later. 

Just copy-paste and modify the help message here...

To make players... `python ladderSample/ladderSample.py` then enter how many players and the file you want to send them to (I send mine to the `ladderSample` folder. Note that CR has roughly 7.5 million players -- This will take a significant amount of time to generalte and will use significant memory (~359 MB)


Using the Graphing Script: 
There are 4 command line arguments

- Filename: File path to find the data.  This is a required argument

- `-k  KINGTOWERS`Graphs only the given king towers. Default is all KTs.  
		-Ex: `-k 11 12 13 14` graphs data for only KTs 11,12, 13 and 14.  The historm 
- `-c COLUMNS_TO_GRAPH` Graphs the selected columns in the data. 
	- `-c 2 3 4 12 ` graphs card level, wins, losses and queue time

This is a table of the column indexes and the corresponding statistic that gets plotted

| Column Index | Stat Name | Description/Notes |
| ----------  | --------- | -----------|
| 1 | King Tower| *Self Explanatory*
| 2 | Card Level| "
| 3 | Wins | "
| 4 | Losses| "
| 5 | Games Played| " 
| 6 | Net Level Advantage|Calculated by taking Player Levels - Opponent Levels. Negative -> underleveled|
| 7 | Net Level Advantage per Match| Level advantage / Total Games
| 8 | Total Level Difference| Calculated by taking absolute value of Player Levels - Opponent Levels. 
| 9 | Level Difference per Match| Level Diff / Total Games
| 10 | Personal Best| Trophy PB. Used as a proxy for "skill". PB is dynamic. 
| 11 | Party Percent| Percent chance a player plays party mode and not ladder
| 12 | Time In Queue| Measure of how many *matches* took place before a player found a match|
	
- `-s SAVE-FILENAME` Filename/path to save the graphs.  If none is specified, then the graph is named `filename.graph.png` and `filename.graphHist.png`  where filename is the data file. 

 
## Roadmap: 

### Known Issues: 

- Issue with the graphing script graphing less than 4 plots. Something to handle 1 row of plots. 


### Functionality:

- **Update formula representing how card levels and skill affect the match (High Priority)** 
    - 0.182x + 0.521 is outdated and doesn't account skill
    - Multiple linear regression isn't great..although maybe its better than I think since I want a percentage, not a 1 or 0. 
    - Using Trophy PB as proxy for skill
    	- Problem is *solved* for Equal Level case!
		- Uneven levels case is significantly harder...

- **Season Statistics** 
	-  How many trophies were *gained* overall in the season
	-  Maximum Trophies obtained (Am I shooting for 8500-9000?)
	-  Performance statistics? (Battles/Sec, Tree Sizes)
	-  Stripped down version of results?
		-  I mostly care about all stats from beginning to end of sim -- less so in between. 
		-  Trophies, KT, PB?
		- Less memory intensive: 1 GB per season is *extremely* expensive 
		- Useful for histogram animation 

- **Create Animation from many seasons of data**
	- Mostly the animated histogram would be useful.  
	- Time series of PBs?
	- Compressed/Smaller form data from seasons?

- **Add more Features to graphing Script (Low Priority)** 
	- **Consistent** colors between stat graphs and histogram

- **Relaxing Model Assumptions:**
    - How do I make some players quit the game?
    - When do players upgrade cards? 
    	- Used to show the double standard issue with gates and KT mm
    - When do players level up their king? 
    - How many players start out every season? 
    	
### Performance improvements:

  - **Use a Splay tree for the queue (High priority)**
       - Has log n amortized bound.  
       - Temporal locality regarding dense trophy regions results in frequent, extremely quick match finding
       - This has to be implemented by hand -- There are special features in my treequeues I cannot get from libraries. 
       - Random BST was seen with avg depth of 13 for 94 elements.  Splay Tree could have higher avg, but locality makes usage much quicker
   

  
