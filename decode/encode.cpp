//Joe Mayer
#include <iostream>
#include "LinkedBinaryTree.h"
#include "BitStreams.h"
#include <queue>
#include <utility>
#include <vector>
using namespace std;
//The global array was a choice.
string CodeTable[257];

void FillCodeTable(LinkedBinaryTree<int>::Position& p,string& code){
  if(p.hasRightChild()==true && p.hasLeftChild()==true){
    string tempL=code;
    string tempR=code;
    tempL.append(1,'0');
    tempR.append(1,'1');
    LinkedBinaryTree<int>::Position pl,pr;
    pl=p.left();
    pr=p.right();
    FillCodeTable(pl,tempL);
    FillCodeTable(pr,tempR);
  }
  else{
    int i;
    i=*p;
    CodeTable[i]=code;
  }
}

void insert(vector<pair<int,LinkedBinaryTree<int> > >& pq,pair<int,LinkedBinaryTree<int> > pr){
  pq.push_back(pr);
  int cindex=pq.size()-1;
  while(cindex!=0){
    pair<int,LinkedBinaryTree<int> >  tc, tp;
    int pindex;
    pindex=((cindex+1)/2)-1;
    tc=pq[cindex];
    tp=pq[pindex];
    if(tc.first>=tp.first){
      cindex=0;
      break;}
    else if(tc.first<tp.first){
      pq[cindex]=tp;
      pq[pindex]=tc;
      cindex=pindex;}
  }
}

pair<int,LinkedBinaryTree<int> > removeMin(vector<pair<int,LinkedBinaryTree<int> > >& pq){//c1=left,c2=right
  pair<int,LinkedBinaryTree<int> >  rt,temp;
  rt=pq[0];
  int back=pq.size()-1;
  temp=pq[back];
  pq.pop_back();
  pq[0]=temp;
  int pindex=0;
  int size;
  size=pq.size()-1;
  while(pindex!=size){
    int c1index,c2index;
    c1index=(2*pindex)+1;
    c2index=(2*pindex)+2;
    if(c1index<=size && c2index<=size){
      pair<int,LinkedBinaryTree<int> >  tc1,tc2,tp;
      tp=pq[pindex];
      tc1=pq[c1index];
      tc2=pq[c2index];
      if(tp.first<=tc1.first && tp.first<=tc2.first){
	pindex=size;
	break;}
      else if(tp.first>tc1.first && tc1.first<=tc2.first){
	pq[pindex]=tc1;
	pq[c1index]=tp;
	pindex=c1index;}
      else if(tp.first>tc2.first && tc2.first<tc1.first){
	pq[pindex]=tc2;
	pq[c2index]=tp;
	pindex=c2index;}
    }
    else if(c1index<=size && c2index>size){
      pair<int,LinkedBinaryTree<int> >  tc1,tp;
      tp=pq[pindex];
      tc1=pq[c1index];
      if(tp.first<=tc1.first){
        pindex=size;
        break;}
      else if(tp.first>tc1.first){
        pq[pindex]=tc1;
        pq[c1index]=tp;
        pindex=c1index;}
    }
    else{pindex=size;
      break;}
  }
  return rt;}

void printvector(vector<pair<int,LinkedBinaryTree<int> > >& pq){
  int s=pq.size();
  for(int x(0);x<s;x++){
    pair<int,LinkedBinaryTree<int> > pr;
    LinkedBinaryTree<int> t;
    LinkedBinaryTree<int>::Position p;
    pr=pq[x];
    t=pr.second;
    p=t.root();
    cout<<"Freq: "<<pr.first<<" Node: "<<*p<<" isRoot?: "<<p.isRoot()<<endl;}
  cout<<endl;}

void printtree(LinkedBinaryTree<int>::Position& p){
  int i;
  i=*p;
  cout<<i<<endl;
  if(p.hasLeftChild()==true && p.hasRightChild()==true){
    LinkedBinaryTree<int>::Position pl,pr;
    pl=p.left();
    pr=p.right();
    printtree(pl);
    printtree(pr);}
}

void WriteHeader(LinkedBinaryTree<int>::Position& p,OutBitStream& outputfile){
  int i;
  i=*p;
  if(p.hasRightChild()==true && p.hasLeftChild()==true){
    LinkedBinaryTree<int>::Position pl,pr;
    outputfile.write(0);
    pl=p.left();
    pr=p.right();
    WriteHeader(pl,outputfile);
    WriteHeader(pr,outputfile);}
  else if(i==256){
    outputfile.write(1);
    outputfile.write(256,9);}
  else if(i!=0 && i!=256){
    outputfile.write(1);
    outputfile.write(i,9);}
}

int main(){
  OutBitStream outputfile;
  string input,output;
  InBitStream inputfile;
  int frequency[257][2];
  for(int x(0);x<257;x++){
    frequency[x][0]=x;
    frequency[x][1]=0;}
  cout<<"Text File: ";
  cin>>input;
  cout<<"Name of zip file?: ";
  cin>>output;
  inputfile.open(input);
  outputfile.open(output);
  while(inputfile.eof()==false){
    int i;
    int temp;
    i=inputfile.read(8);
    temp=frequency[i][1];
    temp++;
    frequency[i][1]=temp;
  }
  frequency[256][1]=1;
  LinkedBinaryTree<int> ET,pLt,pRt;
  ET.addRoot();
  LinkedBinaryTree<int>::Position rt,pl,pr,walk;
  rt=ET.root();
  ET.expandExternal(rt);
  pl=rt.left();
  pr=rt.right();
  vector<pair<int,LinkedBinaryTree<int> > > pq;
  for(int x(0);x<257;x++){
    int f;
    f=frequency[x][1];
    if(f>=1){
      LinkedBinaryTree<int> t;
      LinkedBinaryTree<int>::Position pt;
      pair<int,LinkedBinaryTree<int> > pr;
      t.addRoot();
      pt=t.root();
      *pt=frequency[x][0];
      pr.first=f;
      pr.second=t;
      insert(pq,pr);
    }
  }
    while(pq.size()!=2){
      LinkedBinaryTree<int> t,p1t,p2t;
      t.addRoot();
      LinkedBinaryTree<int>::Position rt,left,right;
      rt=t.root();
      t.expandExternal(rt);
      left=rt.left();
      right=rt.right();
      pair<int,LinkedBinaryTree<int> > p1,p2,pf;
      p1=removeMin(pq);
      p2=removeMin(pq);
      pf.first=(p1.first)+(p2.first);
      p1t=p1.second;
      p2t=p2.second;
      t.replaceExternalWithSubtree(left,p1t);
      t.replaceExternalWithSubtree(right,p2t);
      pf.second=t;
      insert(pq,pf);
    }
    pair<int,LinkedBinaryTree<int> > p1,p2;
    p1=pq[0];
    p2=pq[1];
    if(p1.first<=p2.first){
      pLt=p1.second;
      pRt=p2.second;}
    else{
      pLt=p2.second;
      pRt=p1.second;}
    ET.replaceExternalWithSubtree(pl,pLt);
    ET.replaceExternalWithSubtree(pr,pRt);
    WriteHeader(rt,outputfile);
    inputfile.close();
    inputfile.open(input);
    string code;
    rt=ET.root();
    FillCodeTable(rt,code);
    while(inputfile.eof()==false){
      int i;
      i=inputfile.read(8);
      string binary;
      binary=CodeTable[i];
      string::iterator it;
      it=binary.begin();
      while(it!=binary.end()){
	char a;
	int t;
	a=*it;
	t=a;
	outputfile.write(t);
	it++;}
    }
    //Necessary to write eof.
    string binary;
    binary=CodeTable[256];
    string::iterator it;
    it=binary.begin();
    while(it!=binary.end()){
      char a;
      int t;
      a=*it;
      t=a;
      outputfile.write(t);
      it++;}
    inputfile.close();
    outputfile.close();
    return 0;}
