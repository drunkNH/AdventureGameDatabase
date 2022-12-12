CREATE OR REPLACE FUNCTION
increaseSomeThingCostsFunction(maxTotalIncrease INTEGER)
RETURNS INTEGER AS $$

    DECLARE
        totalIncreaseTillNow INTEGER;
        theThingID	         INTEGER;
        numOwned             INTEGER;
        valIncrease          INTEGER;

    DECLARE increaseCursor CURSOR FOR
        SELECT thingid, owned 
        FROM things, ThingsOwned 
        WHERE thingkind = kind 
        AND (ownerMemberID, ownerRole) IS NOT NULL
        ORDER BY owned DESC;

    BEGIN
        IF maxTotalIncrease <= 0 THEN
            RETURN -1;		
            END IF;

        totalIncreaseTillNow := 0;

        OPEN increaseCursor;

        
        LOOP
            valIncrease := 0;
 
            FETCH increaseCursor INTO theThingID, numOwned;

            EXIT WHEN NOT FOUND OR totalIncreaseTillNow = maxTotalIncrease;

            IF numOwned >= 5 THEN
                IF (totalIncreaseTillNow + 5) <= maxTotalIncrease THEN
                    valIncrease := 5;
                    END IF;
                END IF;

            IF numOwned = 4 THEN
                IF (totalIncreaseTillNow + 4) <= maxTotalIncrease THEN
                    valIncrease := 4;
                    END IF;
                END IF;

            IF numOwned = 3 THEN
                IF (totalIncreaseTillNow + 2) <= maxTotalIncrease THEN
                    valIncrease := 2;
                    END IF;
                END IF;

            UPDATE Things
            SET cost = cost + valIncrease
            WHERE thingID = theThingID;

            totalIncreaseTillNow := totalIncreaseTillNow + valIncrease;

        END LOOP;
        CLOSE increaseCursor;

	RETURN totalIncreaseTillNow;

    END;

$$ LANGUAGE plpgsql;
