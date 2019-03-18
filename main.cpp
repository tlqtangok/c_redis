#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
#include <iostream>
#include <time.h>
#include <cassert>




using namespace std; 

int main(int argc, char **argv) {

	timeval timeout = {2,0}; 
	auto hostname = "localhost"; 
	auto port = 6379; 

	auto c = redisConnectWithTimeout(hostname, port, timeout);
	cout << string(c->errstr) << endl;


    auto reply = (redisReply*)redisCommand(c,"SET t0 t0_val");
	cout << (string(reply->str) == string("OK")) << endl;  
	freeReplyObject(reply);

    reply = (redisReply*)redisCommand(c, "GET %s" , "t0");
	cout << (string(reply->str) == string("t0_val")) << endl;  
	freeReplyObject(reply);

    freeReplyObject(redisCommand(c,"del arr"));

    freeReplyObject(redisCommand(c,"RPUSH arr e0"));
    freeReplyObject(redisCommand(c,"RPUSH arr e1"));
    freeReplyObject(redisCommand(c,"RPUSH arr e2"));

    reply = (redisReply*)redisCommand(c,"LRANGE arr 0 -1");
    cout << (reply->type == REDIS_REPLY_ARRAY &&
              reply->elements == 3 &&
              !memcmp(reply->element[0]->str,"e0",3) &&
              !memcmp(reply->element[1]->str,"e1",3)) << endl;

	freeReplyObject(reply);

#endif 


	redisFree(c); 

	return 0;
}
