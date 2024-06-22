#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
// Structure to represent a point (x, y)
typedef struct {
int x;
int y;
} Point;
// Enumeration for Tetris piece shapes
typedef enum {
SHAPE_O,
SHAPE_I,
SHAPE_T,
SHAPE_L,
SHAPE_J,
SHAPE_S,
SHAPE_Z,
NUM_SHAPES
} TetrisShape;
// Structure to represent a Tetris piece
typedef struct {
TetrisShape shape;
Point blocks[4]; // Each Tetris piece can have up to 4 blocks
} TetrisPiece;
// Node structure for the queue
typedef struct Node {
TetrisPiece data;
struct Node* next;
} Node;
// Queue structure
typedef struct {
Node* front;
Node* rear;
} Queue;
// Function to initialize an empty queue
void initializeQueue(Queue* q) {
q->front = q->rear = NULL;
}
// Function to check if the queue is empty
int isQueueEmpty(Queue* q) {
return (q->front == NULL);
}
// Function to enqueue a Tetris piece to the queue
void enqueue(Queue* q, TetrisPiece p) {
Node* newNode = (Node*)malloc(sizeof(Node));
newNode->data = p;
newNode->next = NULL;
if (q->rear == NULL) {

q->front = q->rear = newNode;
return;
}
q->rear->next = newNode;
q->rear = newNode;
}
// Function to dequeue a Tetris piece from the queue
TetrisPiece dequeue(Queue* q) {
if (isQueueEmpty(q)) {
TetrisPiece p;
return p;
}
Node* temp = q->front;
TetrisPiece p = temp->data;
q->front = q->front->next;
if (q->front == NULL) {
q->rear = NULL;
}
free(temp);
return p;
}
// Function to print the Tetris board
void printBoard(int board[10][20], TetrisPiece* currentPiece, int score) {
system("cls"); // For Windows
int i, j, k;
for (i = 0; i < 10; i++) {
for (j = 0; j < 20; j++) {
int blockIndex = -1;
for (k = 0; k < 4; k++) {
if (currentPiece->blocks[k].x == i && currentPiece->blocks[k].y ==
j) {
blockIndex = k;
break;
}
}
if (blockIndex != -1) {
printf("X ");
} else if (board[i][j] == 1) {
printf("X ");
} else {
printf(". ");
}
}
printf("\n");
}
}
// Function to move the current piece down
void moveDown(TetrisPiece* currentPiece) {
int i;

for (i = 0; i < 4; i++) {
currentPiece->blocks[i].x++;
}
}
// Function to move the current piece left
void moveLeft(TetrisPiece* currentPiece) {
int i;
for (i = 0; i < 4; i++) {
currentPiece->blocks[i].y--;
}
}
// Function to move the current piece right
void moveRight(TetrisPiece* currentPiece) {
int i;
for (i = 0; i < 4; i++) {
currentPiece->blocks[i].y++;
}
}
// Function to rotate the current piece
void rotate(TetrisPiece* currentPiece) {
// Store the current state of the piece
Point originalBlocks[4];
int i;
for (i = 0; i < 4; i++) {
originalBlocks[i] = currentPiece->blocks[i];
}
// Assume rotation around the first block
Point rotationCenter = currentPiece->blocks[0];
// Update the position of each block based on the rotation
for (i = 0; i < 4; i++) {
int relativeX = originalBlocks[i].x - rotationCenter.x;
int relativeY = originalBlocks[i].y - rotationCenter.y;
// Rotate the relative coordinates 90 degrees counterclockwise
int rotatedX = -relativeY;
int rotatedY = relativeX;
// Update the block position based on the rotation
currentPiece->blocks[i].x = rotationCenter.x + rotatedX;
currentPiece->blocks[i].y = rotationCenter.y + rotatedY;
}
}
// Function to update the Tetris board when the current piece reaches the bottom
void updateBoard(int board[10][20], TetrisPiece* currentPiece, int* score) {
int i;
for (i = 0; i < 4; i++) {
board[currentPiece->blocks[i].x][currentPiece->blocks[i].y] = 1; // Mark the cell as filled
}
// Increment the score by 100 for every deque operation
*score += 100;

int row, col;
// Check for completed rows
for (row = 0; row < 20; row++) {
int count = 0;
for (col = 0; col < 10; col++) {
if (board[col][row] == 1) {
count++;
}
}
if (count == 10) {
int r, col;
// Row is full, remove it
for (r = row; r > 0; r--) {
for (col = 0; col < 10; col++) {
board[col][r] = board[col][r - 1];
}
}
for (col = 0; col < 10; col++) {
board[col][0] = 0;
}
}
}
}
// Function to check for collision with filled cells on the board
bool collisionDetected(int board[10][20], TetrisPiece* currentPiece) {
int i;
for (i = 0; i < 4; i++) {
if (board[currentPiece->blocks[i].x][currentPiece->blocks[i].y] == 1) {
return true; // Collision detected
}
}
return false; // No collision
}
// Function to handle user input
void handleInput(TetrisPiece* currentPiece) {
if (_kbhit()) {
char key = _getch();
switch (key) {
case 'a':
moveLeft(currentPiece);
break;
case 'd':
moveRight(currentPiece);
break;
case 's':
moveDown(currentPiece);
break;
case 'w':
rotate(currentPiece);
break;
default:
break;
}
}
}

// Function to hide the console cursor
void hideCursor() {
CONSOLE_CURSOR_INFO cursorInfo;
cursorInfo.dwSize = 1;
cursorInfo.bVisible = FALSE;
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
// Function to show the console cursor
void showCursor() {
CONSOLE_CURSOR_INFO cursorInfo;
cursorInfo.dwSize = 1;
cursorInfo.bVisible = TRUE;
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
// Function to initialize Tetris pieces and enqueue them
void initializeTetrisPieces(Queue* pieceQueue) {
TetrisPiece pieceO = {
SHAPE_O,
{{0, 0}, {0, 1}, {1, 0}, {1, 1}}
};
TetrisPiece pieceI = {
SHAPE_I,
{{0, 0}, {0, 1}, {0, 2}, {0, 3}}
};
TetrisPiece pieceT = {
SHAPE_T,
{{0, 1}, {1, 0}, {1, 1}, {1, 2}}
};
TetrisPiece pieceL = {
SHAPE_L,
{{0, 0}, {1, 0}, {1, 1}, {1, 2}}
};
TetrisPiece pieceJ = {
SHAPE_J,
{{0, 2}, {1, 0}, {1, 1}, {1, 2}}
};
TetrisPiece pieceS = {
SHAPE_S,
{{0, 1}, {0, 2}, {1, 0}, {1, 1}}
};
TetrisPiece pieceZ = {
SHAPE_Z,
{{0, 0}, {0, 1}, {1, 1}, {1, 2}}
};
// Enqueue the Tetris pieces
int i;
for (i = 0; i < 4; i++) {
enqueue(pieceQueue, pieceO);
enqueue(pieceQueue, pieceI);
enqueue(pieceQueue, pieceT);

enqueue(pieceQueue, pieceL);
enqueue(pieceQueue, pieceJ);
enqueue(pieceQueue, pieceS);
enqueue(pieceQueue, pieceZ);
}
}
int main() {
int board[10][20] = {0}; // Initialize the Tetris board
int score = 0; // Initialize the score
Queue pieceQueue;
initializeQueue(&pieceQueue);
initializeTetrisPieces(&pieceQueue);
TetrisPiece currentPiece = dequeue(&pieceQueue); // Initial position of the current piece
int iterations = 0;
int maxIterations = 1000; // Set a maximum number of iterations
hideCursor(); // Call hideCursor() to hide the cursor at the beginning of thegame
while (iterations < maxIterations) {
printBoard(board, &currentPiece, score);
// Provide instructions to the user
printf("Press 'a' to move left, 'd' to move right, 's' to move down, 'w' to rotate.\n");
handleInput(&currentPiece);
// Check for collision with filled cells
if (collisionDetected(board, &currentPiece)) {
printf("Game Over! Your Score: %d\n", score);
break;
}
// Move down only when the 's' key is pressed
if (_kbhit()) {
char key = _getch();
if (key == 's') {
moveDown(&currentPiece);
} else if (key == 'a') {
moveLeft(&currentPiece);
} else if (key == 'd') {
moveRight(&currentPiece);
}
}
// Check if the piece reached the bottom or collided with other blocks
if (currentPiece.blocks[0].x == 9 || currentPiece.blocks[1].x == 9 ||
currentPiece.blocks[2].x == 9 || currentPiece.blocks[3].x == 9 ||
board[currentPiece.blocks[0].x + 1][currentPiece.blocks[0].y] == 1 ||
board[currentPiece.blocks[1].x + 1][currentPiece.blocks[1].y] == 1 ||
board[currentPiece.blocks[2].x + 1][currentPiece.blocks[2].y] == 1 ||
board[currentPiece.blocks[3].x + 1][currentPiece.blocks[3].y] == 1) {

updateBoard(board, &currentPiece, &score); // Update the Tetris board and score
currentPiece = dequeue(&pieceQueue); // Get a new piece from the queue
}
// Add a delay for visualization (you may need to adjust this based on your system)
Sleep(100); // Sleep for 100 milliseconds
iterations++;
}
showCursor(); // Call showCursor() to show the cursor after the game ends
// Display the final score
return 0;
}
