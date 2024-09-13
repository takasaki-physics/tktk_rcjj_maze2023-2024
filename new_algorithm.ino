int zahyou[90][90];
int x = 50;
int y = 50;
int right_weight = 0;
int front_weight = 0;
int left_weight = 0;
int go_to = 0;
int i = 1;//1:east 2:north 3:west 4:south
int 

void reach_count(){

    return zahyou[][];
}

void judge(){
    right_weight = right_reached * 5 + 1;
    front_weight = front_reached * 5 + 2;
    left_weight = left_reached * 5 + 3;

    //kabe ga aru baai sarani plus suru
    

    if (right_weight < front_weight){
        n = right_weight;
        go_to = 1;
    }else{
        n = front_weight;
        go_to = 2;
    }
    if (left_weight < n){
        n = left_weight;
        go_to = 3;
    }
    //go_to 1:right 2:front 3:left
    return go_to
}




void setup(){
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        zahyou[t][j] = 0;  //make it to begining 
        }
    }
}

void loop(){
    

}