int isAlive(int i,int j, int** board, int boardSize, int colSize){
    if(i < 0 || j < 0 || i == boardSize || j==colSize|| board[i][j] == 0)
        return 0;
    return 1;
}

bool checkIfRevive(int i, int j, int** board, int n, int m){
    int count = 0;
    for(int k = i-1; k<=i+1; k++)
        for(int l = j-1; l<=j+1; l++)
            count+=isAlive(k,l,board,n,m);
    return count == 3;
}

bool checkIfStillLiving(int i, int j, int** board,int n, int m){
    int count = 0;
    for(int k = i-1; k<=i+1; k++)
        for(int l = j-1; l<=j+1; l++)
            if(k!=i || l!=j)
                count+=isAlive(k,l,board,n,m);
    return count == 3 || count == 2;
}

void gameOfLife(int** board, int boardSize, int* boardColSize) {
    int n = boardSize;
    int m = *boardColSize;
    int resBoard[n][m];
    for(int i=0;i<n;i++)
        for(int j =0;j<m;j++)
            if(board[i][j]==0){
                if(checkIfRevive(i,j,board,n,m))
                    resBoard[i][j]=1;
                else
                    resBoard[i][j]=0;
                }
            else{
                if(checkIfStillLiving(i,j,board,n,m))
                    resBoard[i][j]=1;
                else
                    resBoard[i][j]=0;
                }
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            board[i][j]=resBoard[i][j];
}

// TODO - add main