# Adventure Game Database
This project features a small Adventure Game database in PostgreSQL, and performing application programming techniques onto the database. This project was done as part of the course CSE 180: Database Systems 1 at the University of California, Santa Cruz
# Application Programming Approaches used
While a majority is done using the native PostgreSQL interface libpq in C, a Stored Function is also done and is called in C.
# Usage
Create and load the Adventure Game data using the included .sql files, along with the .pgsql file to a database server. Compile the .c file using the command

> gcc -L/usr/include -lpq -o runAdventureApplication runAdventureApplication.c

After compiling, you can run it using the command

> runAdventureApplication (username) (password)

where you must fill your credentials to the server as arguments
# What does the program do?
The C program runs tests on the database using three functions, using main to test the functions.

### printNumberOfThingsInRoom
This prints the number of Things in a room. A thing is in a room if one of two conditions are met. If the thing IS NOT owned (indicated by a NULL ownermemberid and ownerrole), the thing is in initialroomid. If the thing IS owned, the thing is in the room where the owner currently is.

### updateWasDefeated
A character, or a monster, is considered defeated when in a battle against an opponent with higher battle points. A character and monster has an attribute wasDefeated, where false means it has not been defeated and true means it has been defeated. This function seeks to update any inconsistent defeated states between the Battles and the wasDefeated attribute for both monsters and characters

### increaseSomeThingCost
Written as a stored function, this function seeks to increase the cost of things based on the popularity of certain kinds of things, using a parameter maxTotalIncrease as a limiter of how much in total should be increased. If a thingKind is owned 5 times or more, increase cost by 5, if 4 increase by 4, if 3 increase by 2, otherwise no increase. The increase is based on popularity, where the priority is to increase the most popular kind of things. Here is an example.

<p> Assume that scrolls are owned by 20 characters (cost increase is 5), swords are owned by 4 characters (cost increase is 4), maps are owned by 3 characters (cost increase is 2) and shields are owned by 3 characters (cost increase is also 2). </p>

<p> What happens if maxTotalIncrease is 62? Then the costs of 12 scrolls are increased by 5, the costs of the swords stay the same, and the cost of 1 of the maps (or shields) is increased by 2. The value that is returned is 62 (which is 12*5 + 0*4 + 1*2). (There's no specific guideline for items of same popularity, so any of the 12 scrolls can do as long as we are increasing 12 scrolls) </p>

<p> (Example written by Shel Finkelstein) </p>
