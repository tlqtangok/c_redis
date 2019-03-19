#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
#include <iostream>
#include <time.h>
#include <cassert>
#include <vector>




using namespace std; 

int main(int argc, char **argv) {

	timeval timeout = {2,0}; 
	auto hostname = "localhost"; 
	auto port = 6379; 

	auto c = redisConnectWithTimeout(hostname, port, timeout);
	cout << string(c->errstr) << endl;

#if 1 
// {

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
              !memcmp(reply->element[1]->str,"e1",3)) << endl;;
		freeReplyObject(reply);



		vector<int> v(11, 0xaabbccdd); 
    reply = (redisReply*)redisCommand(c,"SET t0 %b", v.data(), v.size() * sizeof(int));
		freeReplyObject(reply);

		reply = (redisReply*)redisCommand(c,"GET t0");
		cout << (reply->len ==  v.size() * sizeof(int) )<< endl; 
		std::vector<int> v_r1(v.size(),0); 

		int *v_ = (int*)reply->str; 

		memcpy(v_r1.data(), v_, v.size() * sizeof(int)  ); 
		int cnt = 0; 
		for(auto &e: v_r1)
		{
			//e = v_[cnt]; 
			cout << (e == v[cnt]) << endl; 
			cnt++; 
		}

		cnt = 0; 
		for(auto &e: v_r1)
		{
			e = v_[cnt]; 
			cout << (e == v[cnt]) << endl; 
			cnt++; 
		}
		freeReplyObject(reply);






	



// }
#endif 


	redisFree(c); 


#if 0
	unsigned int j, isunix = 0;
	redisContext *c;
	redisReply *reply;
	const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";

	if (argc > 2) {
		if (*argv[2] == 'u' || *argv[2] == 'U') {
			isunix = 1;
			/* in this case, host is the path to the unix socket */
			printf("Will connect to unix socket @%s\n", hostname);
		}
	}

	int port = (argc > 2) ? atoi(argv[2]) : 6379;

	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	if (isunix) {
		c = redisConnectUnixWithTimeout(hostname, timeout);
	} else {
		c = redisConnectWithTimeout(hostname, port, timeout);
	}

	if (c == NULL || c->err) {
		if (c) {
			printf("Connection error: %s\n", c->errstr);
			redisFree(c);
		} else {
			printf("Connection error: can't allocate redis context\n");
		}
		exit(1);
	}

	/* PING server */
	reply = (redisReply*)redisCommand(c,"PING");
	printf("PING: %s\n", reply->str);
	freeReplyObject(reply);



	/* Set a key */
	reply = (redisReply*)redisCommand(c,"SET %s %s", "foo", "hello world");
	printf("SET: %s\n", reply->str);
	freeReplyObject(reply);

	/* Set a key using binary safe API */
	reply = (redisReply*)redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
	printf("SET (binary API): %s\n", reply->str);
	freeReplyObject(reply);

	/* Try a GET and two INCR */
	reply = (redisReply*)redisCommand(c,"GET foo");
	printf("GET foo: %s\n", reply->str);
	freeReplyObject(reply);

	reply = (redisReply*)redisCommand(c,"INCR counter");
	printf("INCR counter: %lld\n", reply->integer);
	freeReplyObject(reply);
	/* again ... */
	reply = (redisReply*)redisCommand(c,"INCR counter");
	printf("INCR counter: %lld\n", reply->integer);
	freeReplyObject(reply);

	/* Create a list of numbers, from 0 to 9 */
	reply = (redisReply*)redisCommand(c,"DEL mylist");
	freeReplyObject(reply);
	for (j = 0; j < 10; j++) {
		char buf[64];

		snprintf(buf,64,"%u",j);
		reply = (redisReply*)redisCommand(c,"LPUSH mylist element-%s", buf);
		freeReplyObject(reply);
	}

	/* Let's check what we have inside the list */
	reply = (redisReply*)redisCommand(c,"LRANGE mylist 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY) {
		for (j = 0; j < reply->elements; j++) {
			printf("%u) %s\n", j, reply->element[j]->str);
		}
	}
	freeReplyObject(reply);

	/* Disconnects and frees the context */
	redisFree(c);
#endif 
	return 0;
}
