#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

// faces numbering / colours
// 0 = white ->right
// 1 = red   ->right
// 2 = yellow ->right
// 3 = orange -> up
// 4 = blue  ->down,down
// 5 = green ->up

int grid[6][3][3] = {
    {{0,0,0},{0,0,0},{0,0,0}},
    {{1,1,1},{1,1,1},{1,1,1}},
    {{2,2,2},{2,2,2},{2,2,2}},
    {{3,3,3},{3,3,3},{3,3,3}},
    {{4,4,4},{4,4,4},{4,4,4}},
    {{5,5,5},{5,5,5},{5,5,5}}
};

int backup[6][3][3];

stack<pair<char,pair<int,int>>> record;

// orientation (which index in grid corresponds to each logical face)
int currface = 0;
int leftface = 3;
int rightface = 1;
int upface = 4;
int downface = 5;
int backface = 2;

string emoji(int c) {
    switch(c){
        case 0: return "⬜";
        case 1: return "🟥";
        case 2: return "🟨";
        case 3: return "🟧";
        case 4: return "🟦";
        case 5: return "🟩";
    }
    return "?";
}

void display() {
    cout << "\n+--------------------------- CUBE ---------------------------+\n\n";

    // ---------- UP ----------
    for(int i=0;i<3;i++){
        cout << "          ";
        for(int j=0;j<3;j++){
            cout << emoji(grid[upface][i][j]) << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    // LEFT - CURR - RIGHT - BACK
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)
            cout << emoji(grid[leftface][i][j]) << " ";
        cout << " ";

        for(int j=0;j<3;j++)
            cout << emoji(grid[currface][i][j]) << " ";
        cout << " ";

        for(int j=0;j<3;j++)
            cout << emoji(grid[rightface][i][j]) << " ";
        cout << " ";

        for(int j=0;j<3;j++)
            cout << emoji(grid[backface][i][j]) << " ";

        cout << "\n";
    }
    cout << "\n";

    // ---------- DOWN ----------
    for(int i=0;i<3;i++){
        cout << "          ";
        for(int j=0;j<3;j++){
            cout << emoji(grid[downface][i][j]) << " ";
        }
        cout << "\n";
    }

    cout << "\n";
    cout << "+-----------------------------------------------------------+\n";
}

void rotateDown(int c, int l,int r,int u,int d,int b){
    // rotate orientation so the current face moves "down" (viewer rotates cube down)
    currface = u;
    leftface = l;
    rightface = r;
    upface = b;
    downface = c;
    backface = d;
}

void rotateUp(int c, int l,int r,int u,int d,int b){
    currface = d;
    leftface = l;
    rightface = r;
    upface = c;
    downface = b;
    backface = u;
}

void rotateRight(int c, int l,int r,int u,int d,int b){
    currface = l;
    leftface = b;
    rightface = c;
    upface = u;
    downface = d;
    backface = r;
}

void rotateLeft(int c, int l,int r,int u,int d,int b){
    currface = r;
    leftface = c;
    rightface = b;
    upface = u;
    downface = d;
    backface = l;
}

void rotate(char input){
    int c = currface, l = leftface, r = rightface, u = upface, d = downface, b = backface;
    switch (input){
        case 'u': rotateUp(c,l,r,u,d,b); break;
        case 'd': rotateDown(c,l,r,u,d,b); break;
        case 'l': rotateLeft(c,l,r,u,d,b); break;
        case 'r': rotateRight(c,l,r,u,d,b); break;
        default: cout<<"invalid rotate input\n"; break;
    }
}

void makeBackUp(){
    for(int i=0;i<6;i++) for(int j=0;j<3;j++) for(int k=0;k<3;k++) backup[i][j][k] = grid[i][j][k];
}

void undo(){
    for(int i=0;i<6;i++) for(int j=0;j<3;j++) for(int k=0;k<3;k++) grid[i][j][k] = backup[i][j][k];
}

void transpose(int face){
    // rotate face 90 degrees clockwise
    int copy[3][3];
    const int n = 3;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            // new[i][j] = old[n-1-j][i]
            copy[i][j] = grid[face][n-1-j][i];
        }
    }
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) grid[face][i][j] = copy[i][j];
}

void moveUp(int row,int col){
    // shift column 'col' upward relative to current face
    for(int i=0;i<3;i++){
        grid[currface][i][col] = backup[downface][i][col];
        grid[upface][i][col]   = backup[currface][i][col];
        grid[backface][i][col] = backup[upface][i][col];
        grid[downface][i][col] = backup[backface][i][col];
    }

    // rotating adjacent face pieces when the edge column is moved:
    if(col == 0){
        // left face rotated 270 deg (3 * 90)
        transpose(leftface); transpose(leftface); transpose(leftface);
    } else if(col == 2){
        // right face rotated 90 deg
        transpose(rightface);
    }
}

void moveDown(int row,int col){
    for(int i=0;i<3;i++){
        grid[currface][i][col] = backup[upface][i][col];
        grid[upface][i][col]   = backup[backface][i][col];
        grid[backface][i][col] = backup[downface][i][col];
        grid[downface][i][col] = backup[currface][i][col];
    }

    if(col == 0){
        transpose(leftface);
    } else if(col == 2){
        transpose(rightface); transpose(rightface); transpose(rightface);
    }
}

void moveLeft(int row,int col){
    for(int i=0;i<3;i++){
        grid[currface][row][i]  = backup[rightface][row][i];
        grid[leftface][row][i]  = backup[currface][row][i];
        grid[backface][row][i]  = backup[leftface][row][i];
        grid[rightface][row][i] = backup[backface][row][i];
    }

    if(row == 0){
        transpose(upface);
    } else if(row == 2){
        transpose(downface); transpose(downface); transpose(downface);
    }
}

void moveRight(int row,int col){
    for(int i=0;i<3;i++){
        grid[currface][row][i]  = backup[leftface][row][i];
        grid[leftface][row][i]  = backup[backface][row][i];
        grid[backface][row][i]  = backup[rightface][row][i];
        grid[rightface][row][i] = backup[currface][row][i];
    }

    if(row == 0){
        transpose(upface); transpose(upface); transpose(upface);
    } else if(row == 2){
        transpose(downface);
    }
}

void move(char input,int row,int col){
    makeBackUp();
    switch (input){
        case 'u': moveUp(row,col); break;
        case 'd': moveDown(row,col); break;
        case 'l': moveLeft(row,col); break;
        case 'r': moveRight(row,col); break;
        default: cout<<"invalid move input\n"; undo(); break;
    }
}

void handleInput(const string &input){
    if(input.size() == 0){ cout<<"empty input\n"; return; }

    char action = input[0];
    if(action == 'r'){
        if(input.size() < 2){ cout<<"rotate input too short\n"; return; }
        char dir = input[1];
        if(dir!='u' && dir!='d' && dir!='l' && dir!='r'){ cout<<"invalid rotate direction\n"; return; }
        rotate(dir);
    } else if(action == 'm'){
        // expected format: m<dir><row><col> e.g. mu01
        if(input.size() < 4){ cout<<"move input too short\n"; return; }
        char dir = input[1];
        if(dir!='u' && dir!='d' && dir!='l' && dir!='r'){ cout<<"invalid move direction\n"; return; }
        int row = input[2] - '0';
        int col = input[3] - '0';
        if(row < 0 || row > 2 || col < 0 || col > 2){ cout<<"row/col must be 0..2\n"; return; }
        move(dir,row,col);
    } else {
        cout<<"invalid input prefix (use 'r' for rotate, 'm' for move)\n";
    }
}

void initRandom() {
    srand(time(NULL));
}

void scrambleCube(int moves = 25) {
    const char dirs[4] = {'u','d','l','r'};

    for (int i = 0; i < moves; i++) {
        char dir = dirs[rand() % 4];
        int row = rand() % 3;
        int col = rand() % 3;
        move(dir, row, col);
        record.push({dir,{row,col}});
    }
}

void unscrambleCube(){
    while(!record.empty()){
        pair<char,pair<int,int>> temp = record.top();
        record.pop();

        if(temp.first == 'u'){
            move('d',temp.second.first,temp.second.second);
        }else if(temp.first == 'd'){
            move('u',temp.second.first,temp.second.second);
        }else if(temp.first == 'l'){
            move('r',temp.second.first,temp.second.second);
        }else{
            move('l',temp.second.first,temp.second.second);
        }
    }

}

bool isSolved(){
    for(int i=0;i<6;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                if(grid[i][j][k] != i){
                    return false;
                }
            }
        }
    }
    return true;
}

// Usage examples:
// ru -> rotate up
// rd -> rotate down
// rl -> rotate left
// rr -> rotate right
// mu -> move up
// md -> move down
// ml -> move left
// mr -> move right

int main(){
    initRandom();

    display();
    scrambleCube();
    display();
    unscrambleCube();
    display();

    // cout<<"Initial cube:\n";
    // display();

    // while (true){
    //     string input;
    //     cout<<"cmd> ";
    //     if(!(cin>>input)) break;
    //     if(input == "quit") break;
    //     handleInput(input);
    //     display();
    // }
    // return 0;
}
