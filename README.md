#Breadth-fist directory traversing utitlity
Traverse directory recursevly (level by level) and print information about files.

#Buil
Run *make* in same directory as *Makefile* and *new_main.c*

#Usage
./bt [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]
    -h: Print a help message and exit.
    -L: Follow symbolic links, if any.
    -d: Show the time of last modification.
    -g: Print the GID associated with the file.
    -i: Print the number of links to file in inode table.
    -p: Print permission bits as rwxrwxrwx.
    -s: Print the size of file in readable format.
    -t: Print information on file type.
    -u: Print the UID associated with the file.
    -l: Same as: -tpiudgs.

#Algorithm
For breadth-first traversing two structures are introduced, node and queue. 
Each node presents one file and contains link to the next node, and it's path.
After current path is initially added to the queue, in each cycle, if the 
queue head is a directory, all it's contents are added to the end of the queue, 
and queue is dequeued, freeing the previoous queue head, therefore at the end
of the algotithm all memory resourses are freed.
