#include <Algorithm\move.h>
#include <C:\Users\hinat\Documents\repository\tktk_rcjj_maze\algorithm\Algorithm\include\sensor\sensor.h>

move::move()
{
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        toutatu_zahyou[t][j] = 0; 
        }
    }
}



int8_t move::judge(int8_t x,int8_t y,int8_t i)
{
    
    switch (i){//向きによって重みをつける
        case East:
            RightWeight = toutatu_zahyou[x][y+1] * 5 + 1;
            FrontWeight = toutatu_zahyou[x+1][y] * 5 + 2;
            LeftWeight = toutatu_zahyou[x][y-1] * 5 + 3;
            break;

        case North:
            RightWeight = toutatu_zahyou[x+1][y] * 5 + 1;
            FrontWeight = toutatu_zahyou[x][y-1] * 5 + 2;
            LeftWeight = toutatu_zahyou[x-1][y] * 5 + 3;
            break;

        case West:
            RightWeight = toutatu_zahyou[x][y-1] * 5 + 1;
            FrontWeight = toutatu_zahyou[x-1][y] * 5 + 2;
            LeftWeight = toutatu_zahyou[x][y+1] * 5 + 3;
            break;

        case South:
            RightWeight = toutatu_zahyou[x-1][y] * 5 + 1;
            FrontWeight = toutatu_zahyou[x][y+1] * 5 + 2;
            LeftWeight = toutatu_zahyou[x+1][y] * 5 + 3;
            break;

    }

    //壁がある場合更にプラスする
    if (S.right_wall){
        RightWeight += 100;
    }
    if (S.front_wall){
        FrontWeight += 100;
    }
    if (S.left_wall){
        LeftWeight += 100;
    }

    int8_t GoTo = 0;

    //どこへ行くか決める
    if(RightWeight < FrontWeight){
        GoTo = Right;
    }else{
        GoTo = Front;
    }
    if(LeftWeight < min(RightWeight,FrontWeight)){
        GoTo = Left;
    }

    return GoTo;

    if ((RightWeight > 100) && (FrontWeight > 100) && (LeftWeight > 100)){//if all wall
        GoTo = Back;
    }

    RightWeight = 0;//怖いから初期化
    FrontWeight = 0;
    LeftWeight = 0;
}

void move::MoveTo(int8_t x,int8_t y,int8_t i,int8_t GoTo)
{
switch (i){
        case East:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    y += 1;
                    i = South;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    x += 1;
                    i = East;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    y += -1;
                    i = North;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        S.Slope = false;
                    }
                    break;

                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    i = West;
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        S.Slope = false;
                    }
                    break;
                }
            break;
        
        case North:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += 1;
                    i = East;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    y += -1;
                    i = North;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        S.Slope = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    x += -1;
                    i = West;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        S.Slope = false;
                    }
                    break;

                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = South;
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        S.Slope = false;
                    }
                    break;
                }
            break;
        
        case West:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    y += -1;
                    i = North;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        S.Slope = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    x += -1;
                    i = West;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        S.Slope = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    y += 1;
                    i = South;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += 1;
                    i = East;
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        S.Slope = false;
                    }
                    break;
                }
            break;
        
        case South:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += -1;
                    i = West;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        S.Slope = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    y += 1;
                    i = South;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    x += 1;
                    i = East;
                    if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        S.BlackTile = false;
                    }
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        S.Slope = false;
                    }
                    break;
                
                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = North;
                    if(S.Slope){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        S.Slope = false;
                    }
                    break;
                }
            break;
    }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている

    returnX = x;
    returnY = y;
    returnI = i;
    
}
