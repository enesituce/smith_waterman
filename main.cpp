/*Mehmet Enes Gümüş
        150170047      */

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>


using namespace std;

struct node{
    int value;
    node* next;
    string x,y;
    node(){
        value=0;
        next=NULL;
    }
};


node** createMatrix(string a, string b,int match,int missmatch,int gap){
    int rows=a.length()+1;
    int cols=b.length()+1;


    node** pathMatrix = new node*[rows];
    if (rows){
        pathMatrix[0] = new node[rows * cols];
        for (int i = 1; i < rows; ++i){
            pathMatrix[i] = pathMatrix[0] + i * cols;
            }
    }
    
    for(int i =0;i<rows;i++){
        for(int j=0;j<cols;j++){
            node newNode;
            if(i>0 && j>0){
                newNode.x=a[i-1];
                //cout<<a[i-1]<<endl;
                newNode.y=b[j-1];
            }
            pathMatrix[i][j]=newNode;
        }
    }

    for(int i =1;i<rows;i++){
        for(int j=1;j<cols;j++){
            int max=0;
            int wh;
            int addToUp, addToLeft,addToMiss,addToMatch;
            
            addToUp=pathMatrix[i-1][j].value+gap;
            max=addToUp;

            addToLeft=pathMatrix[i][j-1].value+gap;
            if(addToLeft>max){
                max=addToLeft;
                wh=0;
            }

            addToMiss=pathMatrix[i-1][j-1].value+missmatch;
            if(addToMiss>max){
                max=addToMiss;
                wh=1;
            }

            if(a[i-1]==b[j-1]){
                addToMatch = pathMatrix[i-1][j-1].value + match;
                if(addToMatch>max){
                    max=addToMatch;
                    wh=3;
                    
                }
            }
            if(max<0){
                max=0;
                wh=2;
            }
            pathMatrix[i][j].value=max;
            if(wh==0){
                if(pathMatrix[i][j-1].value!=0){
                    pathMatrix[i][j].next=&pathMatrix[i][j-1];
                }
                
            }
            else if(wh==1){
                if(pathMatrix[i-1][j-1].value!=0){
                    pathMatrix[i][j].next=&pathMatrix[i-1][j-1];
                }
                
            }
            

            else if(wh==3){
                pathMatrix[i][j].next=&pathMatrix[i-1][j-1];
            }

            if(max==0){
                pathMatrix[i][j].next=NULL;
            }
        }
    }
    return pathMatrix;
}

void printMatrix(node** matrix,string a, string b,int rows, int cols){
    cout<<"    ";
    for(int i=0;i<cols;i++){
        cout<<b[i]<<" ";
    }
    cout<<endl;
    for(int i =0;i<rows;i++){
        if(i){
            cout<<a[i-1]<<" ";
        }
        else{
            cout<<"  ";
        }
        for(int j=0;j<cols;j++){
            cout<<matrix[i][j].value<<" ";
        }
        cout<<endl;
    }
}

void deleteMatrix(node** matrix,int rows){
    if (rows) delete [] matrix[0];
        delete [] matrix;
}

int findMaxValue(node**matrix,int rows,int cols){
    int max=0;

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(matrix[i][j].value>max){
                max=matrix[i][j].value;
                //cout<<"new max value is: "<<max<<endl;   
            }
        }
    }
    return max;
}


void printSubs(node** matrix,int rows,int cols,ofstream &file){
    vector <string> subs;
    int biggestValue=findMaxValue(matrix,rows,cols);
    //cout<<temp->value<<endl;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(matrix[i][j].value==biggestValue){
                string substring="";
                node* temp = &matrix[i][j];
                while(temp->next){
                    substring=substring+temp->x;
                    temp=temp->next;
                }
                reverse(substring.begin(),substring.end());
                subs.push_back(substring);
                //delete temp;
            }
        }
    }
    sort( subs.begin(), subs.end() );
    subs.erase( unique( subs.begin(), subs.end() ), subs.end() );
    int n=subs.size();
    int i =0;
    if(score==0){
        file<<endl;
        return;
    }
    while(i<n){
        file<<' "'<<subs[i]<<'"';
        i++;
        if(i!=n){
            file<<" ";
        }
    }
    file<<endl;
}

int main(int argc, char* argv[]){
    int match,missmatch,gap;
    //cin>>match>>missmatch>>gap;
    
    vector <string> allStrings;
    
    fstream file;
    if(argc<3){
        file.open("strings.txt");
        match=1;
        missmatch=-2;
        gap=-4;
    }
    else if(argc==3){
        file.open(argv[1]);
        match=1;
        missmatch=-2;
        gap=-4;
    }
    else{
        file.open(argv[1]);
        match=stoi(argv[3]);
        missmatch=stoi(argv[4]);
        gap=stoi(argv[5]);
    }
    
    while(!file.eof()){
        string s;
        getline(file, s);
        allStrings.push_back(s);
    }
    
    sort(allStrings.begin(),allStrings.end());
    file.close();

    ofstream outFile;
    if(argc<3){
        outFile.open("output.txt");
    }
    else{
        outFile.open(argv[2]);
    }
    string a;
    string b;
    int x = allStrings.size();
    for(int i =0;i<x;i++){
        int j = i+1;
        a=allStrings[i];
        while(j<x){
            b=allStrings[j];
            outFile<<a<<" - "<<b<<endl;
            int rows=a.length()+1;
            int cols=b.length()+1;
            node** pathMatrix = createMatrix(a,b,match,missmatch,gap);
            int score=findMaxValue(pathMatrix,rows,cols);
            outFile<<"Score: "<<score<<" Sequence(s):";
            printSubs(pathMatrix,rows,cols,outFile);
            //deleteMatrix(pathMatrix,rows);
            j++;
        }
    }
    
    outFile.close();
    return 0;
}