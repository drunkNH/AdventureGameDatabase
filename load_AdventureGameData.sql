-- Members(memberID, name, address, joinDate, expirationDate, isCurrent)
COPY Members FROM stdin USING DELIMITERS '|';
101|Catalina|123 Main St, New York, NY|2018-01-09|2022-01-09|FALSE
120|Tom Johnson|333 Meder Dr, St. Paul, MN|2011-03-21|2023-04-21|TRUE
103|Siobhan Roy|9931 El Camino, New York, NY|2016-07-20|2022-12-10|TRUE
104|Greg Hirsch|831 Santa Cruz Ave, Toronto, ON|2021-02-28|2024-03-15|TRUE
111|Roman Chan|123 Main St, New York, NY|2018-12-17|2022-07-25|\N
109|Gerri Kellman|222 Emmet Grade, Newark, NJ|2016-07-31|\N|\N
131|Roman Chan|11 Private Dr, New York, NY|2022-01-04|2022-12-28|TRUE
100|Logan Schwartz|101 W34 St, New York, NY|2018-05-16|2024-05-16|FALSE
143|Tom Johnson|101 W34 St, New York, NY|2018-12-02|2030-02-17|FALSE
99|Robert Smith|200 West Cliff Dr, Santa Cruz, CA|2016-07-31|\N|\N
165|Greg Hirsch|101 W34 St, New York, NY|2013-11-01|2027-07-01|TRUE
102|Greg Hirsch|101 W34 St, New York, NY|2008-05-14|2024-09-18|TRUE
\.

-- Rooms(roomID, roomDescription, northNext, eastNext, southNext, westNext)
COPY Rooms FROM stdin USING DELIMITERS '|';
1|Hall of Mountain King|2|5|4|3
2|Witts End|\N|\N|1|\N
3|Twopit Room|\N|1|6|5
4|Misty Cavern|1|\N|\N|6
5|Barren Room|\N|3|\N|1
6|Twilight Genics|3|4|\N|\N
\.

-- Roles(role, battlePoints, InitialMoney)
COPY Roles FROM stdin USING DELIMITERS '|';
knight|30|62.34
mage|129|95.11
cleric|203|48.97
rogue|169|150.00
\.

-- Characters(memberID, role, name, roomID, currentMoney, wasDefeated)
COPY Characters FROM stdin USING DELIMITERS '|';
101|knight|Lancelot|1|62.34|FALSE
99|mage|Oliver|4|123.69|TRUE
111|cleric|Patrick|1|48.97|FALSE
165|rogue|Oliver|4|165.00|TRUE
101|mage|Jack|3|82.34|FALSE
100|knight|Galahad|2|55.11|FALSE
101|rogue|Jack|3|100.25|TRUE
103|mage|Morgan|3|95.15|TRUE
165|cleric|Michael|1|48.97|TRUE
99|cleric|Lancelot|4|66.08|TRUE
143|cleric|Jack|6|75.48|TRUE
103|rogue|Lucas|6|150.00|TRUE
102|cleric|Henry|1|48.97|TRUE
165|knight|Mateo|2|23.48|TRUE
102|rogue|Lucas|3|150.00|TRUE
143|mage|Lancelot|4|95.11|TRUE
165|mage|Levi|6|95.11|TRUE
102|knight|Henry|1|156.81|TRUE
111|mage|\N|4|168.33|FALSE
\.

-- Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
COPY Things FROM stdin USING DELIMITERS '|';
6002|sh|1|\N|\N|7.32|5
6091|sh|3|111|mage|11.65|9
6093|sh|6|101|mage|9.50|8
6049|sw|3|101|knight|20.25|18
6050|sw|1|103|rogue|18.12|13
6051|sw|3|165|mage|20.25|18
6052|sw|1|165|rogue|25.12|13
6075|sh|5|111|cleric|14.99|12
6034|sc|3|101|mage|8.42|8
6021|ma|1|111|mage|9.00|6
6014|sc|3|101|knight|5.55|8
6023|sc|6|165|rogue|18.00|8
6005|st|4|101|rogue|17.76|10
6006|sc|3|165|rogue|11.39|12
6007|sc|4|165|knight|35.00|15
6008|sc|5|111|mage|31.00|11
\.

-- Monsters(monsterID, monsterKind, name, battlePoints, roomID, wasDefeated)
COPY Monsters FROM stdin USING DELIMITERS '|';
944|ba|Unfriendly Giant|202|5|FALSE
963|ch|Bob|24|3|TRUE
965|ba|Fee Fie|15|4|FALSE
925|gi|Fee Fie|213|3|FALSE
988|st|Bullseye|7|1|FALSE
971|ga|Gargamel|39|2|TRUE
972|\N|Bullseye|64|5|FALSE
923|ba|Fee Fie|40|3|FALSE
942|mi|Bane|33|5|FALSE
956|mi|Unfriendly Giant|21|4|FALSE
\.

-- Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
COPY Battles FROM stdin USING DELIMITERS '|';
102|cleric|67|988|7
101|knight|21|963|24
101|rogue|200|925|213
101|knight|6|988|7
101|rogue|220|944|202
111|cleric|13|925|213
165|rogue|38|971|39
101|knight|60|944|202
111|cleric|13|963|24
111|cleric|40|971|39
165|rogue|77|923|40
101|mage|50|923|40
103|mage|27|972|64
101|knight|2|942|5
101|mage|30|971|39
111|cleric|13|956|21
101|mage|63|972|64
165|rogue|29|988|7
101|mage|3|988|7
103|mage|102|988|7
102|cleric|98|972|64
101|mage|19|963|24
101|knight|70|972|64
101|mage|15|956|21
111|mage|25|972|64
111|mage|30|971|36
111|mage|19|963|24
111|mage|36|923|40
\.

