#include <Algorithm\move.h>
#include <sensor/move_2024_2025.h>
extern uint8_t x;
extern uint8_t y;
extern uint8_t Direction;
extern uint8_t Status;
extern move_2024_2025 S;

move::move()
{
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        toutatu_zahyou[t][j] = 0; 
        }
    }
}



int8_t move::judge()
{
    
    switch (Direction){//向きによって重みをつける
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

    

    if ((RightWeight > 100) && (FrontWeight > 100) && (LeftWeight > 100)){//if all wall
        GoTo = Back;
    }

    RightWeight = 0;//怖いから初期化
    FrontWeight = 0;
    LeftWeight = 0;

    return GoTo;
}

void move::MoveTo(int8_t GoTo)//Status=3：直進、4：右折、5：左折、6：後進
{
switch (Direction){
        case East:
            switch (GoTo){
                case Right:
                    Status = 4;
                    y += 1;
                    Direction = South;
                    break;
                
                case Front:
                    Status = 3;
                    x += 1;
                    Direction = East;
                    break;
                
                case Left:
                    Status = 5;
                    y += -1;
                    Direction = North;
                    break;

                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    Direction = West;
                    break;
                }
            break;
        
        case North:
            switch (GoTo){
                case Right:
                    Status = 4;
                    x += 1;
                    Direction = East;
                    break;
                
                case Front:
                    Status = 3;
                    y += -1;
                    Direction = North;
                    break;
                
                case Left:
                    Status = 5;
                    x += -1;
                    Direction = West;
                    break;

                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    Direction = South;
                    break;
                }
            break;
        
        case West:
            switch (GoTo){
                case Right:
                    Status = 4;
                    y += -1;
                    Direction = North;
                    break;
                
                case Front:
                    Status = 3;
                    x += -1;
                    Direction = West;
                    break;
                
                case Left:
                    Status = 5;
                    y += 1;
                    Direction = South;
                    break;
                
                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += 1;
                    Direction = East;
                    
                    break;
                }
            break;
        
        case South:
            switch (GoTo){
                case Right:
                    Status = 4;
                    x += -1;
                    Direction = West;
                    break;
                
                case Front:
                    Status = 3;
                    y += 1;
                    Direction = South;
                    break;
                
                case Left:
                    Status = 5;
                    x += 1;
                    Direction = East;
                    break;
                
                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    Direction = North;
                    break;
                }
            break;
    }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている
    
}

void move::TileColor(){
    switch (Direction)
    {
    case North:
        if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
            y += 1;
            S.BlackTile = false;
        }
        if(S.Slope){//坂の信号が送られていた場合座標を更に進める
            y += -1;
            S.Slope = false;
        }
        break;

    case East:
        if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
            x += -1;
            S.BlackTile = false;
        }
        if(S.Slope){//坂の信号が送られていた場合座標を更に進める
            x += 1;
            S.Slope = false;
        }
        break;
    
    case West:
        if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
            x += 1;
            S.BlackTile = false;
        }
        if(S.Slope){//坂の信号が送られていた場合座標を更に進める
            x += -1;
            S.Slope = false;
        }
        break;

    case South:
        if(S.BlackTile){//黒タイルの信号が送られていた場合戻る
            y += -1;
            S.BlackTile = false;
        }
        if(S.Slope){//坂の信号が送られていた場合座標を更に進める
            y += 1;
            S.Slope = false;
        }
        break;
    
    
    }
}