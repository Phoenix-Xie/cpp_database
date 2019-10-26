#include "cmdui.h"
#include <algorithm>
using namespace std;

void CmdUI::drawSplitLine(const vector<int> & width){
    int m = width.size();
    for(int j = 0; j < m; j++){
        if(j == 0) cout << "+-";
        else cout << "-";

        for(int k = 0; k < width[j]; k++){
            cout << "-";
        }
        cout << "-+";
    }
    cout << endl;
}

void CmdUI::drawDataLine(const vector<string> & data, const vector<int> & width){
    int m = data.size();
    for(int j = 0; j < m; j++){
        if(j == 0) cout << "| ";
        else cout << " ";
        cout << data[j];
        int blankNum = width[j] - data[j].length();
        for(int k = 0; k < blankNum; k++){
            cout << " ";
        }
        cout << " |";
    }
    cout << endl;
}

void CmdUI::drawTable(const vector< vector<string> > & data){
    int n = data.size();
    if(n == 0) return;
    int m = data[0].size();
    vector<int> width(m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            width[j] = max(int(data[i][j].length()), width[j]);
        }
    }
    for(int i = 0; i < n; i++){
        if(i == 0)
            drawSplitLine(width);

        drawDataLine(data[i], width);
        drawSplitLine(width);
    }
}


void CmdUI::drawTable(const vector< vector<string> > & data, const vector<string> & head){
    int n = data.size();
    if(n == 0) return;
    int m = data[0].size();
    vector<int> width(m);
    //计算宽度
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            width[j] = max(int(data[i][j].length()), width[j]);
        }
    }
    int headn = head.size();
    for(int i = 0; i < headn; i++){
        width[i] = max(int(head[i].length()), width[i]);
    }

    //画头
    drawSplitLine(width);
    drawDataLine(head, width);
    //画表
    for(int i = 0; i < n; i++){
        if(i == 0)
            drawSplitLine(width);

        drawDataLine(data[i], width);
        drawSplitLine(width);
    }
}