/*Intro*/

The programming task creates a utility to traverse a specified directory in breadth-first order. Breadth-first search explores all the nodes at a given level before descending lower in the tree. Breadth-first search is implemented with a queue. As the program encounters each directory node at a particular level, it enqueues the pathname for later examination. Do the following steps to get the desired output.

Run this command in terminal to compile the main

gcc main.c -o bt

Step 1 : Run this command: type "Make" in terminal.
    
Step 2 : Use the output format specified. The executable should be called bt. The program will be invoked by:
bt [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]
The options are to be interpreted as follows:
 h Print a help message and exit.
 L Follow symbolic links, if any. Default will be to not follow symbolic links.
 t Print information on file type.
 p Print permission bits as rwxrwxrwx. 
i Print the number of links to file in inode table. 
u Print the UID associated with the file. 
g Print the GID associated with the file.
s Print the size of file in bytes. If the size is larger than 1K, indicate the size in KB with a suffix K; if the size is larger than 1M, indicate the size in MB with a suffix M; if the size is larger than 1G, indicate the size in GB with a suffix G
d Show the time of last modification
l This option will be used to print information on the file as if the options tpiugs are all specified.


