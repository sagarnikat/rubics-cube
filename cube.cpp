#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <cctype>
#include <cstdio>

#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;

class Cube
{
private:
    int grid[6][3][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, // 0 white
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}, // 1 red
        {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}, // 2 yellow (back)
        {{3, 3, 3}, {3, 3, 3}, {3, 3, 3}}, // 3 orange (left)
        {{4, 4, 4}, {4, 4, 4}, {4, 4, 4}}, // 4 blue (up)
        {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}}  // 5 green (down)
    };

    int backup[6][3][3];

    int currface = 0;
    int leftface = 3;
    int rightface = 1;
    int upface = 4;
    int downface = 5;
    int backface = 2;

    string notaion(char cmd)
    {
        switch (cmd)
        {
        case 'r':
            return "R";
        case 'R':
            return "R'";
        case 'l':
            return "L";
        case 'L':
            return "L'";
        case 'u':
            return "U";
        case 'U':
            return "U'";
        case 'd':
            return "D";
        case 'D':
            return "D'";
        case 'f':
            return "F";
        case 'F':
            return "F'";
        case 'b':
            return "B";
        case 'B':
            return "B'";
        case 'x':
            return "X";
        case 'y':
            return "Y";
        case 'z':
            return "Z";
        default:
            return "";
        }
    }

    string emoji(int c)
    {
        switch (c)
        {
        case 0:
            return "⬜";
        case 1:
            return "🟥";
        case 2:
            return "🟨";
        case 3:
            return "🟧";
        case 4:
            return "🟦";
        case 5:
            return "🟩";
        }
        return "?";
    }

    void transpose(int face, int num)
    {
        int copym[3][3];
        const int n = 3;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                copym[i][j] = grid[face][n - 1 - j][i];
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                grid[face][i][j] = copym[i][j];
            }
        }
        if (num > 1)
        {
            transpose(face, --num);
        }
    }

    void makeBackUp()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    backup[i][j][k] = grid[i][j][k];
                }
            }
        }
    }

    void undoFromBackup()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    grid[i][j][k] = backup[i][j][k];
                }
            }
        }
    }

    void initRandom()
    {
        srand((unsigned)time(NULL));
    }

    void moveR()
    {
        // front <- down
        grid[currface][0][2] = backup[downface][0][2];
        grid[currface][1][2] = backup[downface][1][2];
        grid[currface][2][2] = backup[downface][2][2];

        // down <- back (reversed)
        grid[downface][0][2] = backup[backface][2][0];
        grid[downface][1][2] = backup[backface][1][0];
        grid[downface][2][2] = backup[backface][0][0];

        // back <- up (reversed)
        grid[backface][0][0] = backup[upface][2][2];
        grid[backface][1][0] = backup[upface][1][2];
        grid[backface][2][0] = backup[upface][0][2];

        // up <- temp (orig front)
        grid[upface][0][2] = backup[currface][0][2];
        grid[upface][1][2] = backup[currface][1][2];
        grid[upface][2][2] = backup[currface][2][2];

        transpose(rightface, 1);
    }

    void moveR_()
    {
        // front <- up
        grid[currface][0][2] = backup[upface][0][2];
        grid[currface][1][2] = backup[upface][1][2];
        grid[currface][2][2] = backup[upface][2][2];

        // up <- back (reversed)
        grid[upface][0][2] = backup[backface][2][0];
        grid[upface][1][2] = backup[backface][1][0];
        grid[upface][2][2] = backup[backface][0][0];

        // back <- down (reversed)
        grid[backface][0][0] = backup[downface][2][2];
        grid[backface][1][0] = backup[downface][1][2];
        grid[backface][2][0] = backup[downface][0][2];

        // down <- temp (orig front)
        grid[downface][0][2] = backup[currface][0][2];
        grid[downface][1][2] = backup[currface][1][2];
        grid[downface][2][2] = backup[currface][2][2];

        transpose(rightface, 3);
    }

    void moveL()
    {
        // front <- up
        grid[currface][0][0] = backup[upface][0][0];
        grid[currface][1][0] = backup[upface][1][0];
        grid[currface][2][0] = backup[upface][2][0];

        // up <- back (reversed)
        grid[upface][0][0] = backup[backface][2][2];
        grid[upface][1][0] = backup[backface][1][2];
        grid[upface][2][0] = backup[backface][0][2];

        // back <- down (reversed)
        grid[backface][0][2] = backup[downface][2][0];
        grid[backface][1][2] = backup[downface][1][0];
        grid[backface][2][2] = backup[downface][0][0];

        // down <- temp (orig front)
        grid[downface][0][0] = backup[currface][0][0];
        grid[downface][1][0] = backup[currface][1][0];
        grid[downface][2][0] = backup[currface][2][0];

        transpose(leftface, 1);
    }

    void moveL_()
    {
        // front <- down
        grid[currface][0][0] = backup[downface][0][0];
        grid[currface][1][0] = backup[downface][1][0];
        grid[currface][2][0] = backup[downface][2][0];

        // down <- back (reversed)
        grid[downface][0][0] = backup[backface][2][2];
        grid[downface][1][0] = backup[backface][1][2];
        grid[downface][2][0] = backup[backface][0][2];

        // back <- up (reversed)
        grid[backface][0][2] = backup[upface][2][0];
        grid[backface][1][2] = backup[upface][1][0];
        grid[backface][2][2] = backup[upface][0][0];

        // up <- temp (orig front)
        grid[upface][0][0] = backup[currface][0][0];
        grid[upface][1][0] = backup[currface][1][0];
        grid[upface][2][0] = backup[currface][2][0];

        transpose(leftface, 3);
    }

    void moveU()
    {
        // front <- right (row 0)
        grid[currface][0][0] = backup[rightface][0][0];
        grid[currface][0][1] = backup[rightface][0][1];
        grid[currface][0][2] = backup[rightface][0][2];

        // right <- back (row 0)
        grid[rightface][0][0] = backup[backface][0][0];
        grid[rightface][0][1] = backup[backface][0][1];
        grid[rightface][0][2] = backup[backface][0][2];

        // back <- left (row 0)
        grid[backface][0][0] = backup[leftface][0][0];
        grid[backface][0][1] = backup[leftface][0][1];
        grid[backface][0][2] = backup[leftface][0][2];

        // left <- temp (orig front row 0)
        grid[leftface][0][0] = backup[currface][0][0];
        grid[leftface][0][1] = backup[currface][0][1];
        grid[leftface][0][2] = backup[currface][0][2];

        transpose(upface, 1);
    }

    void moveU_()
    {
        // front <- left (row 0)
        grid[currface][0][0] = backup[leftface][0][0];
        grid[currface][0][1] = backup[leftface][0][1];
        grid[currface][0][2] = backup[leftface][0][2];

        // left <- back (row 0)
        grid[leftface][0][0] = backup[backface][0][0];
        grid[leftface][0][1] = backup[backface][0][1];
        grid[leftface][0][2] = backup[backface][0][2];

        // back <- right (row 0)
        grid[backface][0][0] = backup[rightface][0][0];
        grid[backface][0][1] = backup[rightface][0][1];
        grid[backface][0][2] = backup[rightface][0][2];

        // right <- temp (orig front row 0)
        grid[rightface][0][0] = backup[currface][0][0];
        grid[rightface][0][1] = backup[currface][0][1];
        grid[rightface][0][2] = backup[currface][0][2];

        transpose(upface, 3);
    }

    void moveD_()
    {
        // front <- right (row 2)
        grid[currface][2][0] = backup[rightface][2][0];
        grid[currface][2][1] = backup[rightface][2][1];
        grid[currface][2][2] = backup[rightface][2][2];

        // right <- back (row 2)
        grid[rightface][2][0] = backup[backface][2][0];
        grid[rightface][2][1] = backup[backface][2][1];
        grid[rightface][2][2] = backup[backface][2][2];

        // back <- left (row 2)
        grid[backface][2][0] = backup[leftface][2][0];
        grid[backface][2][1] = backup[leftface][2][1];
        grid[backface][2][2] = backup[leftface][2][2];

        // left <- temp (orig front row 2)
        grid[leftface][2][0] = backup[currface][2][0];
        grid[leftface][2][1] = backup[currface][2][1];
        grid[leftface][2][2] = backup[currface][2][2];

        transpose(downface, 3);
    }

    void moveD()
    {
        // front <- left (row 2)
        grid[currface][2][0] = backup[leftface][2][0];
        grid[currface][2][1] = backup[leftface][2][1];
        grid[currface][2][2] = backup[leftface][2][2];

        // left <- back (row 2)
        grid[leftface][2][0] = backup[backface][2][0];
        grid[leftface][2][1] = backup[backface][2][1];
        grid[leftface][2][2] = backup[backface][2][2];

        // back <- right (row 2)
        grid[backface][2][0] = backup[rightface][2][0];
        grid[backface][2][1] = backup[rightface][2][1];
        grid[backface][2][2] = backup[rightface][2][2];

        // right <- temp (orig front row 2)
        grid[rightface][2][0] = backup[currface][2][0];
        grid[rightface][2][1] = backup[currface][2][1];
        grid[rightface][2][2] = backup[currface][2][2];

        transpose(downface, 1);
    }

    void moveF()
    {
        // right col (col 2) <- up bottom row reversed->col
        grid[rightface][0][0] = backup[upface][2][0];
        grid[rightface][1][0] = backup[upface][2][1];
        grid[rightface][2][0] = backup[upface][2][2];

        // up bottom row <- left col (col 2)
        grid[upface][2][0] = backup[leftface][2][2];
        grid[upface][2][1] = backup[leftface][1][2];
        grid[upface][2][2] = backup[leftface][0][2];

        // left col (col 2) <- orig curr bottom row (from backup)
        grid[leftface][0][2] = backup[downface][0][0];
        grid[leftface][1][2] = backup[downface][0][1];
        grid[leftface][2][2] = backup[downface][0][2];

        // curr bottom row <- right col (col 2) reversed
        grid[downface][0][0] = backup[rightface][2][0];
        grid[downface][0][1] = backup[rightface][1][0];
        grid[downface][0][2] = backup[rightface][0][0];

        transpose(currface, 1);
    }

    void moveF_()
    {
        // save orig curr bottom row implicitly by reading from backup as needed
        // curr bottom row <- left col (col 2) reversed orientation
        grid[downface][0][0] = backup[leftface][0][2];
        grid[downface][0][1] = backup[leftface][1][2];
        grid[downface][0][2] = backup[leftface][2][2];

        // right col (col 2) <- orig curr bottom row reversed orientation (read from backup)
        grid[rightface][0][0] = backup[downface][0][2];
        grid[rightface][1][0] = backup[downface][0][1];
        grid[rightface][2][0] = backup[downface][0][0];

        // up bottom row <- right col (col 2) reversed mapping
        grid[upface][2][0] = backup[rightface][0][0];
        grid[upface][2][1] = backup[rightface][1][0];
        grid[upface][2][2] = backup[rightface][2][0];

        // left col (col 2) <- up bottom row
        grid[leftface][0][2] = backup[upface][2][2];
        grid[leftface][1][2] = backup[upface][2][1];
        grid[leftface][2][2] = backup[upface][2][0];

        transpose(currface, 3);
    }

    void moveB_()
    {
        // right col (col 2) <- up bottom row reversed->col
        grid[rightface][0][2] = backup[upface][0][0];
        grid[rightface][1][2] = backup[upface][0][1];
        grid[rightface][2][2] = backup[upface][0][2];

        // up bottom row <- left col (col 2)
        grid[upface][0][0] = backup[leftface][2][0];
        grid[upface][0][1] = backup[leftface][1][0];
        grid[upface][0][2] = backup[leftface][0][0];

        // left col (col 2) <- orig curr bottom row (from backup)
        grid[leftface][0][0] = backup[downface][2][0];
        grid[leftface][1][0] = backup[downface][2][1];
        grid[leftface][2][0] = backup[downface][2][2];

        // curr bottom row <- right col (col 2) reversed
        grid[downface][2][0] = backup[rightface][2][2];
        grid[downface][2][1] = backup[rightface][1][2];
        grid[downface][2][2] = backup[rightface][0][2];

        transpose(backface, 3);
    }

    void moveB()
    {
        // save orig curr bottom row implicitly by reading from backup as needed
        // curr bottom row <- left col (col 2) reversed orientation
        grid[downface][2][0] = backup[leftface][0][0];
        grid[downface][2][1] = backup[leftface][1][0];
        grid[downface][2][2] = backup[leftface][2][0];

        // right col (col 2) <- orig curr bottom row reversed orientation (read from backup)
        grid[rightface][0][2] = backup[downface][2][2];
        grid[rightface][1][2] = backup[downface][2][1];
        grid[rightface][2][2] = backup[downface][2][0];

        // up bottom row <- right col (col 2) reversed mapping
        grid[upface][0][0] = backup[rightface][0][2];
        grid[upface][0][1] = backup[rightface][1][2];
        grid[upface][0][2] = backup[rightface][2][2];

        // left col (col 2) <- up bottom row
        grid[leftface][0][0] = backup[upface][0][2];
        grid[leftface][1][0] = backup[upface][0][1];
        grid[leftface][2][0] = backup[upface][0][0];

        transpose(backface, 1);
    }

    void scrambleCubeImpl(int moves = 25)
    {
        const char dirs[12] = {'r', 'R', 'l', 'L', 'u', 'U', 'd', 'D', 'f', 'F', 'b', 'B'};

        cout << "scramble is : ";
        for (int i = 0; i < moves; i++)
        {
            char dir = dirs[rand() % 12];

            cout << notaion(dir) << " ";
            applyMoveInternal(dir);
        }
        cout << endl;
    }

    void reset()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    grid[i][j][k] = i;
                }
            }
        }
    }

    // returns inverse move char
    char invertCmd(char c)
    {
        switch (c)
        {
        case 'r':
            return 'R';
        case 'R':
            return 'r';
        case 'l':
            return 'L';
        case 'L':
            return 'l';
        case 'u':
            return 'U';
        case 'U':
            return 'u';
        case 'd':
            return 'D';
        case 'D':
            return 'd';
        case 'f':
            return 'F';
        case 'F':
            return 'f';
        case 'b':
            return 'B';
        case 'B':
            return 'b';

        default:
            return 0;
        }
    }

    // apply a single-char move internally (assumes makeBackUp() was called already)
    void applyMoveInternal(char cmd)
    {
        makeBackUp();

        switch (cmd)
        {
        case 'r':
            moveR();
            break;
        case 'R':
            moveR_();
            break;
        case 'l':
            moveL();
            break;
        case 'L':
            moveL_();
            break;
        case 'u':
            moveU();
            break;
        case 'U':
            moveU_();
            break;
        case 'd':
            moveD();
            break;
        case 'D':
            moveD_();
            break;
        case 'f':
            moveF();
            break;
        case 'F':
            moveF_();
            break;
        case 'b':
            moveB();
            break;
        case 'B':
            moveB_();
            break;
        }
    }

    void convertFace(int face, string &kociamba)
    {

        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int colour = grid[face][j][k];

                switch (colour)
                {
                case 0:
                    kociamba.push_back('F');
                    break;
                case 1:
                    kociamba.push_back('R');
                    break;
                case 2:
                    kociamba.push_back('B');
                    break;
                case 3:
                    kociamba.push_back('L');
                    break;
                case 4:
                    kociamba.push_back('U');
                    break;
                case 5:
                    kociamba.push_back('D');
                    break;
                default:
                    break;
                }
            }
        }
    }

    string convertToFacelet()
    {
        int currface = 0;
        int leftface = 3;
        int rightface = 1;
        int upface = 4;
        int downface = 5;
        int backface = 2;

        string kociamba;

        convertFace(4, kociamba);
        convertFace(1, kociamba);
        convertFace(0, kociamba);
        convertFace(5, kociamba);
        convertFace(3, kociamba);
        convertFace(2, kociamba);

        return kociamba;
    }

    static string shellEscapeDoubleQuoted(const string &s)
    {
        string out;
        out.reserve(s.size() + 8);
        for (char c : s)
        {
            if (c == '"' || c == '\\')
            {
                out.push_back('\\');
                out.push_back(c);
            }
            else
            {
                out.push_back(c);
            }
        }
        return out;
    }

public:
    // Runs "~/.local/bin/kociemba \"<facelets>\"" and returns {output, exit_code}
    pair<string, int> runKociembaWithFacelets(const string &facelets)
    {
        // build executable path
        const char *home = getenv("HOME");
        string exe = (home ? string(home) + "/.local/bin/kociemba" : string("kociemba"));

        // escape facelets for placement inside double quotes
        string esc = shellEscapeDoubleQuoted(facelets);

        // command: <exe> "<esc>" 2>&1
        string cmd = exe + " \"" + esc + "\" 2>&1";

        FILE *pipe = popen(cmd.c_str(), "r");
        if (!pipe)
        {
            return {string("popen failed"), -1};
        }

        string output;
        char buf[512];
        while (fgets(buf, sizeof(buf), pipe) != nullptr)
            output += buf;

        int status = pclose(pipe);
        int exit_code = -1;

        if (status == -1)
        {
            exit_code = -1;
        }
        else
        {
            if (WIFEXITED(status))
                exit_code = WEXITSTATUS(status);
            else
                exit_code = status; // e.g. signaled
        }

        // trim trailing newlines/spaces
        while (!output.empty() && isspace((unsigned char)output.back()))
            output.pop_back();

        return {output, exit_code};
    }

    void solve()
    {
        string facelets = convertToFacelet();

        // cout << facelets << endl;

        auto [out, code] = runKociembaWithFacelets(facelets);
        if (code != 0)
        {
            cout << "exit code: " << code << "\n";
        }
        cout << "solution:\n"
             << out << "\n";
    }

    Cube() { initRandom(); }

    vector<char> convertUniversalToLocal(const string &universal)
    {
        vector<char> result;
        auto isFace = [](char c) -> bool
        {
            char u = (char)std::toupper((unsigned char)c);
            return u == 'R' || u == 'L' || u == 'U' || u == 'D' || u == 'F' || u == 'B';
        };

        size_t n = universal.size();
        size_t i = 0;
        while (i < n)
        {
            // skip whitespace
            if (std::isspace((unsigned char)universal[i]))
            {
                ++i;
                continue;
            }

            // expect a face letter
            char c = universal[i];
            if (!isFace(c))
            {
                // if it's not a face letter, skip it (tolerant parser)
                ++i;
                continue;
            }

            // build token: face + optional modifier (' or 2)
            char face = c;
            ++i;
            char mod = 0;
            if (i < n)
            {
                char nxt = universal[i];
                if (nxt == '\'')
                {
                    mod = '\'';
                    ++i;
                } // prime
                else if (nxt == '2')
                {
                    mod = '2';
                    ++i;
                } // double
                // else no modifier; leave i pointing at next char for next iteration
            }

            // map face to local characters
            char cw = (char)std::tolower((unsigned char)face); // clockwise local (lowercase)
            char ccw = (char)std::toupper((unsigned char)cw);  // counter-clockwise local (uppercase)

            if (mod == 0)
            {
                result.push_back(cw);
            }
            else if (mod == '\'')
            {
                result.push_back(ccw);
            }
            else if (mod == '2')
            {
                result.push_back(cw);
                result.push_back(cw);
            }
        }

        return result;
    }

    void display()
    {
        cout << "\n+--------------------------- CUBE ---------------------------+\n\n";

        // ---------- UP ----------
        for (int i = 0; i < 3; i++)
        {
            cout << "          ";
            for (int j = 0; j < 3; j++)
            {
                cout << emoji(grid[upface][i][j]) << " ";
            }
            cout << "\n";
        }
        cout << "\n";

        // LEFT - CURR - RIGHT - BACK
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                cout << emoji(grid[leftface][i][j]) << " ";
            cout << " ";

            for (int j = 0; j < 3; j++)
                cout << emoji(grid[currface][i][j]) << " ";
            cout << " ";

            for (int j = 0; j < 3; j++)
                cout << emoji(grid[rightface][i][j]) << " ";
            cout << " ";

            for (int j = 0; j < 3; j++)
                cout << emoji(grid[backface][i][j]) << " ";

            cout << "\n";
        }
        cout << "\n";

        // ---------- DOWN ----------
        for (int i = 0; i < 3; i++)
        {
            cout << "          ";
            for (int j = 0; j < 3; j++)
            {
                cout << emoji(grid[downface][i][j]) << " ";
            }
            cout << "\n";
        }

        cout << "\n";
        cout << "+-----------------------------------------------------------+\n";
    }

    void move(char cmd)
    {
        if (cmd == 's')
        {
            scrambleCubeImpl();
            return;
        }
        if (cmd == 'w')
        {
            reset();
            return;
        }
        if (cmd == 'a')
        {
            solve();
            return;
        }

        // normal cube face moves
        // record the move (for potential unscramble)
        applyMoveInternal(cmd);
    }

    bool isSolved()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    if (grid[i][j][k] != i)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    string getFacelets() { return convertToFacelet(); }

    void setFacelets(const std::string &facelets)
    {
        // Expect exactly 54 chars in the same order as convertToFacelet(): U,R,F,D,L,B (each 9)
        if (facelets.size() < 54)
            return;

        // mapping from convertToFacelet() order (U,R,F,D,L,B) to internal face indices
        const int faceOrder[6] = {4, 1, 0, 5, 3, 2};

        for (int fo = 0; fo < 6; ++fo)
        {
            int face = faceOrder[fo];
            int base = fo * 9;
            for (int i = 0; i < 9; ++i)
            {
                char ch = facelets[base + i];
                int r = i / 3;
                int c = i % 3;
                int color = 0; // default white
                switch (ch)
                {
                case 'F':
                    color = 0;
                    break; // white
                case 'R':
                    color = 1;
                    break; // red
                case 'B':
                    color = 2;
                    break; // yellow/back
                case 'L':
                    color = 3;
                    break; // orange
                case 'U':
                    color = 4;
                    break; // blue
                case 'D':
                    color = 5;
                    break; // green
                default:
                    color = 0;
                    break;
                }
                grid[face][r][c] = color;
            }
        }
    }
};

// ----------------- main (cammented so it can be used in ui file) -----------------

// int main()
// {
//     Cube cube;
//     cout << "\n=== Rubik's Cube CLI (Universal notation enabled) ===\n\n";
//     cout << "You may enter moves in universal/Kociemba notation or as local single-char commands.\n\n";
//     cout << "Universal examples:\n";
//     cout << "  - Spaced:  R U R' U' F2\n";
//     cout << "  - Compact: RUR'U'F2\n\n";
//     cout << "Local single-char commands (legacy supported):\n";
//     cout << "  R R'    : right face cw / ccw\n";
//     cout << "  L L'    : left face cw / ccw\n";
//     cout << "  U U'    : up face cw / ccw\n";
//     cout << "  D D'    : down face cw / ccw\n";
//     cout << "  F F'    : front face cw / ccw\n";
//     cout << "  B B'    : back face cw / ccw\n\n";
//     cout << "Utilities:\n";
//     cout << "  s      : scramble\n";
//     cout << "  a      : display solution\n";
//     cout << "  w      : reset\n";
//     cout << "  q/quit : exit\n\n";
//     // Initial display
//     cube.move('p');
//     string line;
//     while (true)
//     {
//         cube.display();
//         cout << "\ncmd> ";
//         if (!std::getline(cin, line))
//             break;
//         // trim
//         auto trim = [&](string &s)
//         {
//             size_t a = s.find_first_not_of(" \t\r\n");
//             if (a == string::npos)
//             {
//                 s.clear();
//                 return;
//             }
//             size_t b = s.find_last_not_of(" \t\r\n");
//             s = s.substr(a, b - a + 1);
//         };
//         trim(line);
//         if (line.empty())
//             continue;
//         // quick commands
//         if (line == "q" || line == "quit")
//         {
//             cout << "Exiting.\n";
//             break;
//         }
//         // If single-char utility commands are given alone, handle immediately
//         if ((line.size() == 1) && (line[0] == 'p' || line[0] == 's' || line[0] == 'w'))
//         {
//             cube.move(line[0]);
//             continue;
//         }
//         // Try convert universal -> local atomic moves
//         vector<char> atomic = cube.convertUniversalToLocal(line);
//         // If conversion produced nothing, fallback to parsing as compact local single-char commands
//         if (atomic.empty())
//         {
//             const string allowed = "rRlLuUdDfFbBsowoxyza";
//             for (char ch : line)
//             {
//                 if (std::isspace((unsigned char)ch))
//                     continue;
//                 if (allowed.find(ch) == string::npos)
//                 {
//                     cout << "Ignored unknown command char: '" << ch << "'\n";
//                     continue;
//                 }
//                 atomic.push_back(ch);
//             }
//         }
//         if (atomic.empty())
//         {
//             cout << "No valid moves parsed. Try: R U R' U' F2  OR  rRf\n";
//             continue;
//         }
//         // Apply atomic moves
//         for (char cmd : atomic)
//         {
//             cube.move(cmd);
//         }
//         // After whole sequence, display cube
//         cube.move('p');
//     }
//     return 0;
// }

