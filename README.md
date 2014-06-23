The shell was honestly quite hard to get going. I originally created my parser in C++, but realized this probably wouldn't pan out in C. Therefore, it took me about equal time to set up the code needed to even get the right inputs as it took me to implement the commands the C faux-shell could take. My shell seems to work great (I've been testing with mkdir, rmdir, ls, mv, rm, etc.), although when I write to a file sometimes the cmd still executes as well in the shell. With a few more days and a better background understand of pipes and execvp I believe I could tighten up the loose screws that may have not come out in the small sample of testing I did. IT did help me learn a lot about pipes and exactly how complicated the shell is. 

I tried to modularize my code a ton so I could pinpoint exactly where any errors would occur. Figuring out the best way to parse was very difficult - I tried a few different ways but decided to use the C equivalent of a string array - an array of character arrays, so that I could easily loop through all the "strings" in the array. Reading in the input was also tough as well - It was not easy to choose which wa, since C has a couple different ways to read in input - using fgets was the easiest way and allowed me to parse through the input and divy up each seperate command into its own character array.

I'll go through each of the major functions and their order:
	ResetCmds - Creates available memory for arrays of char arrays
	freeMem - Frees any allocated memory
	getCmds - This takes the input and breaks it up into character arrays which are then linked together in an array of arrays
	getInput - This receives input that the user types from the stdin buffer
	getAllCmds - This returns a list of all hardcoded regular commands
	getSpecialCmds - This returns a list of all hardcoded special commands
	verifySpecial - This method scans the input for any special characters and returns an indicator to let the program know which route it should go
	verifyCmd - This has a hardcoded list of all the commands that I think the shell should implement - it checks to see if any of the input arguments have a correct command and returns it
	getArgs - Not used, but I'm leaving it in there in case we use this code again
	getLength - This just counts the number of commands that were inputted
	setFirstCmd - This method takes the input and takes the first half of the commands (those to the left of the special command)
	setSecondCmd - This method takes the input and takes the second half of the commands (those to the right of the special command) and returns it
	redirection - This takes in the input and splits it into two seperate commands and executes them correctly by reading in the special char
	startProcess - This takes the input from main and checks to see if it has a special character or if its just one command.
	main - contains the loop that runs the shell until "exit" is stated.
	
	
	
What parts succeeded: I feel that my shell works at the very least in a basic way. It can create, remove , append to or list files just like a regular command. It parses well (even though this is where many of my errors occurred) and can perform piping and redirection, even though how that works was definitely a little "hacky." It creates child processes no problems and essentially works.

What I didn't do was include the "cd" command and the history feature, although that would've been really cool to do (I think I may have need an array of arrays of arrays, so a three-dimensional array). With more time I would like to implement how to do that. I also did not implement the Ctrl+C option since many times my programs did not take that long to run, and so I never really had a chance on how to implement and test it. I'm also not sure how this performs if a wrong command is inputted - I believe it just spits out an error from execvp.

I tested the code by running through many different possible commnands and version of implementing them, such as listing files both to the shell, appending them to text files and overwriting text files as well. I tried to test at least three different commands for each type of special character. To debug, I used many printf statements to see where errors were occurring, especially when I would receive "segmentation fault" errors which never really gave the place where they occurred, making it very hard to debug. 