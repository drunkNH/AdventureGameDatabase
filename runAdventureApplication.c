/**
 * runAdventureApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define  MAXNUMBERSTRINGSIZE        20


/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/*
 * checkTuples checks if the query was successfully executed. If not, then produce an error message, commit, clear, and bad_exit. Used for statements that returns data such as SELECT
 * takes in the connection, the failing result, and the function name called from
*/
static void checkTuples (PGconn *conn, PGresult *res, char *functionName) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "%s failure: %s\n", functionName, PQerrorMessage(conn));
        res = PQexec(conn, "COMMIT TRANSACTION");
        PQclear(res);
        bad_exit(conn);
    }
}

/*
 * checkCommandOK checks if the query was successfully executed. If not, then produce an error message, commit, clear, and bad_exit. Used for statements that does not returns data such as UPDATE
 * takes in the connection, the failing result, and the function name called from
*/
static void checkCommandOK (PGconn *conn, PGresult *res, char *functionName) {
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "%s failure: %s\n", functionName, PQerrorMessage(conn));
        res = PQexec(conn, "COMMIT TRANSACTION");
        PQclear(res);
        bad_exit(conn);
    }
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 */

/* Function: printNumberOfThingsInRoom:
 * -------------------------------------
 * Parameters:  connection, and theRoomID, which should be the roomID of a room.
 * Prints theRoomID, and number of things in that room.
 * Return 0 if normal execution, -1 if no such room.
 * bad_exit if SQL statement execution fails.
 */

int printNumberOfThingsInRoom(PGconn *conn, int theRoomID)
{
    //for every pqexec, perform a check that the statement was successful, using either a custom if statement, or using the function checkTuples and checkCommandOK
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "%s failure: %s\n", "printNumberOfThingsInRoom", PQerrorMessage(conn));
        PQclear(res);
        bad_exit(conn);
    }

    //create query to check if theRoomID exists
    char query[MAXSQLSTATEMENTSTRINGSIZE];
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, "SELECT roomID, roomDescription FROM Rooms WHERE roomID = %i", theRoomID); 
    res = PQexec(conn, query); 
    checkTuples(conn, res, "printNumberOfThingsInRoom");

    //If query was successful, but got no tuples, theRoomID does not exist
    if (atoi(PQcmdTuples(res)) == 0) {
        res = PQexec(conn, "COMMIT TRANSACTION");
        checkCommandOK(conn, res, "printNumberOfThingsInRoom");
        PQclear(res);
        return -1;
    }

    //create query to find things that is not owned in the same room as theRoomID, and add it to the counter numOfThings
    char *roomDescription = PQgetvalue(res, 0, 1);
    int numOfThings;
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, "SELECT COUNT(initialroomid) FROM Things WHERE ownermemberid IS NULL AND initialroomID = %i", theRoomID);
    res = PQexec(conn, query); 
    checkTuples(conn, res, "printNumberOfThingsInRoom");
    numOfThings = atoi(PQgetvalue(res, 0, 0));

    //create query to find things owned by a character in Characters, where the character is in the same room as theRoomID and owns the corresponding thingID, and add it to the counter numOfThings
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, "SELECT count(roomid) FROM Characters, Things WHERE (ownermemberid, ownerrole) = (memberid, role) AND roomid = %i", theRoomID);
    res = PQexec(conn, query); 
    checkTuples(conn, res, "printNumberOfThingsInRoom");
    numOfThings += atoi(PQgetvalue(res, 0, 0));

    //print roomid, the description, and how many things in the room, commit transaction, clear res, return 0.
    printf("Room %i, %s, has %i in it.\n", theRoomID, roomDescription, numOfThings);
    res = PQexec(conn, "COMMIT TRANSACTION");
    checkCommandOK(conn, res, "printNumberOfThingsInRoom");
    PQclear(res);
    return 0;
}

/* Function: updateWasDefeated:
 * ----------------------------
 * Parameters:  connection, and a string, doCharactersOrMonsters, which should be 'C' or 'M'.
 * Updates the wasDefeated value of either characters or monsters (depending on value of
 * doCharactersOrMonsters) if that value is not correct.
 * Returns number of characters or monsters whose wasDefeated value was updated,
 * or -1 if doCharactersOrMonsters value is invalid.
 */

int updateWasDefeated(PGconn *conn, char *doCharactersOrMonsters)
{
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "%s failure: %s\n", "updateWasDefeated", PQerrorMessage(conn));
        PQclear(res);
        bad_exit(conn);
    }

    //Initialize needed variables
    int updatedItems = 0;
    int val1 = strcmp(doCharactersOrMonsters, "C");
    int val2 = strcmp(doCharactersOrMonsters, "M");
    char *table;
    char *op;
    char *id1;
    char *id2;

    if (val1 == 0) {
        //If doCharactersOrMonsters == "C", change char * variables to the necessary strings
        table = "Characters";
        op = ">";
        id1 = "memberID, role";
        id2 = "characterMemberID, characterRole";
    }
    else if (val2 == 0) {
        //If doCharactersOrMonsters == "M", change char * variables to the necessary strings
        table = "Monsters";
        op = "<";
        id1 = "monsters.monsterID";
        id2 = "battles.monsterID";
    }
    else {
        //If doCharactersOrMonsters != "C" or "M", return -1 after clearing
        res = PQexec(conn, "COMMIT TRANSACTION");
        checkCommandOK(conn, res, "updateWasDefeated");
        PQclear(res);
        return -1;
    }

    //Create general query that updates wasDefeated to true for either C or M if the (M/C) was defeated in battle and has wasDefeated incorrectly set, and add the number of updated items to the counter updatedItems
    char query[MAXSQLSTATEMENTSTRINGSIZE];
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, 
        "UPDATE %s SET wasDefeated = TRUE FROM Battles WHERE monsterBattlePoints %s characterBattlePoints AND (wasDefeated = FALSE OR wasDefeated IS NULL) AND (%s) = (%s)", 
        table, op, id1, id2);
    res = PQexec(conn, query); 
    checkCommandOK(conn, res, "updateWasDefeated");
    updatedItems += atoi(PQcmdTuples(res));
    
    //If doCharactersOrMonster is "M", change id1 and id2 to the necessary strings in order for it to work on the general query
    if (val2 == 0) {
        id1 = "monsterID";
        id2 = "monsterID";
    }

    //Create general query that updates wasDefeated to false for either C or M if the (M/C) was not defeated and has wasDefeated incorrectly set, and add the number of updated items to the counter updatedItems
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, 
        "UPDATE %s SET wasDefeated = FALSE WHERE (%s) NOT IN (SELECT DISTINCT %s FROM Battles WHERE monsterBattlePoints %s characterBattlePoints) AND (wasDefeated = TRUE OR wasDefeated IS NULL)", 
        table, id1, id2, op);
    res = PQexec(conn, query); 
    checkCommandOK(conn, res, "updateWasDefeated");
    updatedItems += atoi(PQcmdTuples(res));

    //return updatedItems after commiting, clearing.
    res = PQexec(conn, "COMMIT TRANSACTION");
    checkCommandOK(conn, res, "updateWasDefeated");
    PQclear(res);
    return updatedItems;
}

/* Function: increaseSomeThingCosts:
 * -------------------------------
 * Parameters:  connection, and an integer maxTotalIncrease, the maximum total increase that
 * it should make in the cost of some things.
 * Executes by invoking a Stored Function, increaseSomeThingCostsFunction, which
 * returns a negative value if there is an error, and otherwise returns the total
 * cost increase that was performed by the Stored Function.
 */

int increaseSomeThingCosts(PGconn *conn, int maxTotalIncrease)
{
    //create query statement to select increaseSomeThingCostsFunction(maxTotalIncrease), check if query was successful, store result in increasedAmount, clear res, and return increasedAmount.
    char query[MAXSQLSTATEMENTSTRINGSIZE];
    snprintf(query, MAXSQLSTATEMENTSTRINGSIZE, "SELECT increaseSomeThingCostsFunction(%i)", maxTotalIncrease);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "increaseSomeThingCosts failure: \n%s\n", PQerrorMessage(conn));
        PQclear(res);
        bad_exit(conn);
    }
    int increasedAmount = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return increasedAmount;
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runAdventureApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }

    int result;
    
    /* Perform the calls to printNumberOfThingsInRoom listed in Section 6 of Lab4,
     * printing error message if there's an error.
     */
    int printNumTest[4] = {1, 2, 3, 7}; //theRoomID to be tested
    for (int i = 0; i < 4; i++) {
        result = printNumberOfThingsInRoom(conn, printNumTest[i]);
        if (result == -1) {
            printf("No room exists whose id is %i\n", printNumTest[i]);
        }
        else if (result == 0) {
            //do nothing
        }
        else {
            fprintf(stderr, "\nERROR: Received unexpected value from printNumberOfThingsInRoom\nParameter used: %i\nBad value returned: %i\nEXPECTED: 0 OR -1\nbad exit\n", printNumTest[i], result);
            bad_exit(conn);
        }
    }
    /* Extra newline for readability */
    printf("\n");
    
    /* Perform the calls to updateWasDefeated listed in Section 6 of Lab4,
     * and print messages as described.
     */
    char str[3][10] = {"C", "M", "C"}; //doCharactersOrMonsters to be tested
    for (int i = 0; i < 3; i++) {
        result = updateWasDefeated(conn, str[i]);
        if (result == -1) {
            printf("Illegal value for doCharactersOrMonsters %s\n", str[i]);
        }
        else if (result >= 0) {
            printf("%i wasDefeated values were fixed for %s\n", result, str[i]);
        }
        else {
            fprintf(stderr, "\nERROR: Received unexpected value from updateWasDefeated\nParameter used: %s\nBad value returned: %i\nEXPECTED: Non-negative value OR -1\nbad exit\n", str[i], result);
            bad_exit(conn);
        }
    }
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to increaseSomeThingCosts listed in Section 6 of Lab4,
     * and print messages as described.
     */
    int increaseCostTest[4] = {12, 500, 39, 1}; //maxTotalIncrease to be tested
    for (int i = 0; i < 4; i++) {
        result = increaseSomeThingCosts(conn, increaseCostTest[i]);
        if (result >= 0) {
            printf("Total increase for maxTotalIncrease %i is %i\n", increaseCostTest[i], result);
        }
        else {
            fprintf(stderr, "\nERROR: Received negative value from increaseCostTest\nParameter used: %i\nParameter is a non-positive value, which is a forbidden value for increaseCostTest\nbad exit\n", increaseCostTest[i]);
            bad_exit(conn);
        }
    }
    printf("\n");
    good_exit(conn);
    return 0;
}
