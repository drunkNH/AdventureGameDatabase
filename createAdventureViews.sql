-- This View is used to determine the popularity of thingkinds
CREATE VIEW ThingsOwned AS
SELECT thingkind AS kind, count(*) AS owned 
FROM Things
WHERE (ownermemberID, ownerrole) IS NOT NULL 
GROUP BY kind
ORDER BY count(*) DESC
