#include <iostream>
#include <fstream>
#include <string>
#include <tomcrypt.h>
#include <cstdlib>
#include <iomanip>
#include <bitset>
#include <bits/stdc++.h>
#include <zmq.hpp>
#include <chrono>
#include <thread>
#define NBASE 16
using namespace std::chrono_literals;
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

int main() {

//recieving 
	// initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    // prepare some static data for responses
    const std::string data{"Send Hash: "};

    
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Recieved Hex: " << request.to_string() << std::endl;

        // simulate work
        std::this_thread::sleep_for(1s);

        // send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);

	zmq::message_t req1;
	socket.recv(req1, zmq::recv_flags::none);
	cout << "Recieved Hash: " << req1.to_string() << endl;
string hash_server = "";
hash_server = req1.to_string();


// converting hex into cyphertext
std::string msg = request.to_string();
std::istringstream convert(msg);
std::vector<unsigned char> b;
unsigned int c;
while (convert >> std::hex >> c)
{
    b.push_back(c);
 
}
unsigned char* cypherText = b.data();
int a = b.size();
//cout << cypherText << endl;

//reading shared keys
string sharedKey;
string key;
ifstream ReadKey("SharedSecretKey.txt");
while (getline(ReadKey, sharedKey)){
          key = sharedKey;
      //    cout << key << endl;
  } // read shared Key
  ReadKey.close();

  int y = key.length();
  //cout << y << endl;
  int d = a / y;
  //cout << d <<endl;
  //cout << "a: " << a <<endl;
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
        //  cout<< (i-1)*32 + j <<":" << hash_result2[j]<<endl;
  }
}

//cout << "key: " << string(tHash).length() << endl;
//cout << "hash of concatenated key: " << string(tHash) << endl;

// converting into cyphertext using XOR
string cypher;
string convertedText;
stringstream sc;
for(int i=0; i<a; i++){
        cypher = cypherText[i] ^ tHash[i];
        sc << cypher;
        convertedText = sc.str();
}
cout << "converted into Text: " << convertedText << endl;
//cout << convertedText.length() << endl;



// hashing recieved text 
  unsigned char* hash_result1 = hashSHA2(convertedText);
  string hash_client = "";
  stringstream ss;
  for(int i=0; i<int(sha256_desc.hashsize); i++)
  {
          ss <<hex<< (int)hash_result1[i];
          hash_client = ss.str();
  }
  cout << "hash of recived text: " << hash_client << endl;

  if ( hash_client == hash_server ){
  	cout << "Hashes successfully matches" << endl;
  }

//saving bob plain text
ofstream file("BobPlaintext.txt");
file << convertedText;

//saving bob plain text
ofstream file1("Bob_h.txt");
file1 << hash_client << endl;

}

