//Joe Mayer
#include <iostream>
#include "LinkedBinaryTree.h"
#include "BitStreams.h"
#include <fstream>
#include <string>
using namespace std;

void BuildTree(LinkedBinaryTree<char>& code,LinkedBinaryTree<char>::Position& p,InBitStream& inputfile){
  int i;
  i=inputfile.read();
  if(i==0){
    code.expandExternal(p);
    LinkedBinaryTree<char>::Position pL,pR;
    pL=p.left();
    pR=p.right();
    BuildTree(code,pL,inputfile);
    BuildTree(code,pR,inputfile);}
  else if(i==1){
    int ascii;
    ascii=inputfile.read(9);
    char add=static_cast<char>(ascii);
    *p=add;}
}
void WriteToFile(LinkedBinaryTree<char>::Position& walk,InBitStream& inputfile){
  if(walk.hasLeftChild()==true && walk.hasRightChild()==true){  
     int i;
     i=inputfile.read();
     LinkedBinaryTree<char>::Position temp;
     temp=walk;
     if(i==0){
       walk=temp.left();
       WriteToFile(walk,inputfile);}
     else if(i==1){
       walk=temp.right();
       WriteToFile(walk,inputfile);}
  }
}
int main(){
  string input,output;
  InBitStream inputfile;
  cout<<"What file would you like to decode?: ";
  cin>>input;
  cout<<"What file would you like to write it in?: ";
  cin>>output;  
  OutBitStream outputfile;
  outputfile.open(output);
  inputfile.open(input);
  LinkedBinaryTree<char> code;
  code.addRoot();
  LinkedBinaryTree<char> :: Position p,walk;
  p=code.root();
  BuildTree(code,p,inputfile);
  char write;
  int end(1);
  while(end!=0){
    walk=code.root();
    WriteToFile(walk,inputfile);
    write=*walk;
    end=(int)write;
    if(end!=0){    
      outputfile.write(end,8);}
    }
  inputfile.close();
  outputfile.close();
  return 0;}
