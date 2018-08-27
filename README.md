# Softwar

## Server

> server/

### Architecture

file | content
------|------
`libs/` | functions used indrectly (not directly server related)
`srcs/` | server code, as a library
`includes/` | server headers
`main.c` | entrypoint, as light as possible


### Testing

Unit tests written in Python

- Run the server
- Run the tests
- Kill the server


## Client

> client/


## Commands

---

`Identify` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "identify|player_id"

| Condition | Response |
| -------- | -------- |
| If the client can connect | `"OK|You are in !"` |
| If the client already exists in the list | `"KO|identity already exists"` |
| If a 5th client connects | `"KO|game full"` |

Fills the list of players and sets their positions by arrival order.

---

`Forward` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "forward|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If he wants to go out of map size | `"KO|out of map"` |
| If he tries to walk on another client | `"KO|another player is on this cell"` |
| Else, we will check his looking orientation, make him go forward.<br>Then we send one of these messages | `"OK|you moved up"`<br>`"OK|you moved right"`<br>`"OK|you moved down"`<br>`"OK|you moved left"` |

---

`Backward` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "backward|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If he wants to go out of map size | `"KO|out of map"` |
| If he tries to walk on another client | `"KO|another player is on this cell"` |
| Else, we will check his looking orientation and will make him go backward.<br>Then, we send one of these messages | `"OK|you moved up"`<br>`"OK|you moved right"`<br>`"OK|you moved down"`<br>`"OK|you moved left"` |

---

`LeftForward (leftfwd)` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "leftfwd|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If he wants to go out of map size | `"KO|out of map"` |
| If he tries to walk on another client | `"KO|another player is on this cell"` |
| Else, we will check his looking orientation, rotate him left once and make him go forward.<br> Then we send one of these messages | `"OK|you moved up"`<br>`"OK|you moved right"`<br>`"OK|you moved down"`<br>`"OK|you moved left"` |

---

`RightForward (rightfwd)` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "rightfwd|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If he wants to go out of map size | `"KO|out of map"` |
| If he tries to walk on another client | `"KO|another player is on this cell"` |
| Else, we will check his looking orientation, rotate him right once and make him go forward.<br> Then we send one of these messages | `"OK|you moved up"`<br>`"OK|you moved right"`<br>`"OK|you moved down"`<br>`"OK|you moved left"` |

---

`Left` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "left|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we will rotate him left once and send | `"OK|Rotated left"` |

---

`Right` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "right|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we will rotate him right once and send | `"OK|Rotated right"` |

---

`Looking` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "looking|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we will send his looking orientation | `"OK|0"`<br>`"OK|1"`<br>`"OK|2"`<br>`"OK|3"` |

| Enum | Result |
| -------- | -------- |
| 0 | `left` |
| 1 | `up` |
| 2 | `right` |
| 3 | `down` |

---

`Gather` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "gather|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If there is no energy to gather | `"KO|no energy to gather"` |
| Else, we will transfer the energy value to the client.<br> Then, we send | `"OK|player gathered energy"` |

---

`Watch` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "watch|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we check his looking orientation,<br> take the 4 cells in cone in front of him,<br> and send as an array the result | `"OK|["empty","player_id2","energy",""]"` |

| Possibilities | Signification |
| -------- | -------- |
| `"empty"` | An empty cell, with no one or nothing on it |
| `"energy"` | An energy cell that can be picked up |
| `"player_id"` | Another player standing on a cell |
| `""` | Out of map |

**The order of the values in that array is :** 

    1. The cell in front of the client.
    2. 2 cells in front and one to the left of the client.
    3. 2 cells in front of the client.
    4. 2 cells in front and one to the right of the client.

**Here is an example :**

![](https://image.noelshack.com/fichiers/2018/34/1/1534787061-vision.png)

---

`Attack` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "attack|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we check his looking orientation,<br> use 1 energy point,<br>stun every clients in vision,<br> and send | `"OK|2 player(s) have been attacked"` |

---

`SelfId` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "selfid|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we send | `"OK|player_id"` |

---

`SelfStats` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "selfstats|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we send | `"OK|50" (Self energy)` |

---

`Inspect` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "inspect|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If inspected client is not known | `"KO|process does not exist"` |
| Else, we send | `"OK|50" (Inspected energy)` |

---

`Next` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "next|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| Else, we send | `"OK|process waiting next cycle"` |

---

`Jump` :

**Client message format (stack zmq) :**

>   Identity : "player_id"
>   Empty
>   Command : "jump|"

| Condition | Response |
| -------- | -------- |
| If client identity is not known | `"KO|identity unknown"` |
| If he wants to go out of map size | `"KO|out of map"` |
| If he jumps to another client | `"KO|another player is on this cell"` |
| Else, we will check his looking orientation, make him go forward twice.<br>Then we send one of these messages | `"OK|you moved up"`<br>`"OK|you moved right"`<br>`"OK|you moved down"`<br>`"OK|you moved left"` |

**A client can jump above another client. We wanted that.**

---

**In every single one of these functions, if there is something preventing the process to perform an action, it will be notified as so :**

| Condition | Response |
| -------- | -------- |
| If client has not enough energy | `"KO|process dead"` |
| If client has not enough action points | `"KO|not enough action points"` |
| If client is stunned | `"KO|process stunned"` |

---