# 🪄 Fairy Chess ♟️
## A C++ implementation of the Fairy Chess game
### ❕Fairy Chess is a type of chess variant that modifies some rules, pieces and/or the board of traditional chess.

### ➡️ Setup 
```./fairy-chess``` to run the game without recompiling <br/> 
```make``` to run the Makefile and create the executable `fairy-chess` <br/> 
```./run.sh``` to recompile the application and run `fairy-chess` <br/> 

## ❕Main Menu Options
- `Player vs Player` will start a match between two players
- `Player vs Bot` will start a match against the bot. The bot currently uses one of its legal moves randomly
- `Help` will display information about the fairy pieces, the rules of the game and how to play it <br/>
 &nbsp; <img src="https://raw.githubusercontent.com/xbandrade/fairy-chess/master/img/menu.png" width=30% height=30%>


## ❕This implementation of Fairy Chess currently has three Fairy Pieces:
- `The Wazir` has the same movement pattern as the Rook, but can only move a single square, making it very weak.
   Its symbol in algebraic notation is `W`  and it is worth as much as a Pawn.
- `The Camel` moves 3 squares forward and one to the side, in a long L shape. It can also jump over other pieces, just like the Knight.
   Its symbol in algebraic notation is `C` and it is worth as much as a Knight.
- `The Grasshopper` has the same movement pattern as the Queen, but can only move by hopping over another piece, then it lands immediately behind that piece.
   If there is no pieces to hop over, the Grasshopper cannot move! Its symbol in algebraic notation is `G` and it is worth as much as a Knight.
- All the other pieces have their traditional movement pattern and exchange value. The board and other chess rules remain unchanged.
  
  &nbsp; <img src="https://raw.githubusercontent.com/xbandrade/fairy-chess/master/img/board.png" width=70% height=70%>
  
 ## ❕Moves
➡️ You can move a piece by using its algebraic notation:
- `Qe5` moves the Queen to **e5**
- `Gxd4` moves the Grasshopper to **d4**, capturing a piece
- `h3` moves a Pawn to **h3**
- `dxe4` moves a Pawn in the **d** file to **e4**, capturing a piece 

➡️ Special moves:
- `m` + `[algebraic position]` will display all legal moves for the piece on that position, e.g. the Wazir is on square **b6**, 
  then `mb6` shows all available moves for that Wazir.
- `m*` will display all legal moves for the current player
- `M1B` will take you to a mate in 1 position for the black pieces
- `M1W` will take you to a mate in 1 position for the white pieces

More information about Fairy Chess pieces and rules can be found on [chess.com](https://www.chess.com/clubs/forum/view/the-fairy-pieces-introduction)
and [Wikipedia](https://en.wikipedia.org/wiki/Fairy_chess_piece).  
 
