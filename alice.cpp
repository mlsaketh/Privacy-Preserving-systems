#include <iostream>
#include <fstream>
#include <string>
#include <tomcrypt.h>
#include <cstdlib>
#include <iomanip>
#include <bitset>
#include <bits/stdc++.h>
#include <zmq.hpp>
using namespace std;
unsigned char* hashSHA2(const string&); //prototype of SHA256
unsigned char* hashSHA2(const string& input)
{
        unsigned char* hash_res = new unsigned char[sha256_desc.hashsize];
        hash_state md;
        sha256_init(&md);
        sha256_process(&md, (const unsigned char*) input.c_str(), input.size());
        sha256_done(&md, hash_res);
        return hash_res;
}

int main () {
  string plainText;
  string sharedKey;
  string text;
  string key;
//reading files
  ifstream ReadText("HW1PlaintextTest.txt");
  ifstream ReadKey("SharedSecretKey.txt");
  while (getline(ReadText, plainText)){ 
	  text = plainText;
	  cout <<"Text present in file: "<< text <<endl;
  } //read plain text
  ReadText.close();
  while (getline(ReadKey, sharedKey)){
          key = sharedKey;
	  //cout << key << endl;
  } // read shared Key
  ReadKey.close();

  int a = text.length();
//  cout << a << endl;

  if(a%32 != 0) {
	  cout<<  "Provided text is not a multiple of 32 bytes!" <<endl;
  	return 0;
  }

  int b = key.length();
  //cout << b << endl;
  
  // hashing text 
  unsigned char* hash_result1 = hashSHA2(text);
  string hash1 = "";
  stringstream ss;
  for(int i=0; i<int(sha256_desc.hashsize); i++)
  {
	  ss <<hex<< (int)hash_result1[i];
	  hash1 = ss.str();
  }
  cout << "hash of text: " << hash1 << endl;
    int d = a / b; //number of digits to be concatenated
  //cout << d << endl; 
 
  //concatenation of keys
  string changedKey;
  string hash2 = "";
  char tHash[a];
  stringstream sa;
  for(int i=1; i<= d; i++){
	 
	  stringstream sb;
	  sb << i; 
	  string x = sb.str(); 
	  changedKey = key + x; 
 	  
   //hashing concatenated key
  unsigned char* hash_result2 = hashSHA2(changedKey);
  for(int j=0; j<int(sha256_desc.hashsize); j++)
  {
	
          tHash[(i-1)*32 + j] = hash_result2[j];
	// cout<< (i-1)*32 + j <<":" << hash_result2[j]<<endl; 
  }
}

//cout << "hash of concatenated key: " << string(tHash) << endl;
// converting into cyphertext using XOR
string cypher;
string cypherText;
stringstream sc;
for(int i=0; i<text.length(); i++){
	cypher = text[i] ^ tHash[i];
	sc << cypher;
	cypherText = sc.str();
}
//cout << "cypher text: " << cypherText << endl;
//cout << cypherText.length() << endl;



std::stringstream sd;
for(int i=0; i<text.length(); ++i){
    sd << std::hex <<" " << (unsigned int)(unsigned char)cypherText[i];

}
std::string cypherInt = sd.str();

cout << "Hex of converted Text : " << cypherInt <<endl;


// initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // set up CypherText data to send
    const std::string data{cypherInt};

  
        // send the request message
        std::cout << "Sending Hex..." << std::endl;
        socket.send(zmq::buffer(data), zmq::send_flags::none);

        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received cyphertext by bob " << reply.to_string();
        std::cout << std::endl;

	//send hash
	cout << "Sending hash .... " << endl;
	socket.send(zmq::buffer(hash1), zmq::send_flags::none);

	

  
return 0;
}


