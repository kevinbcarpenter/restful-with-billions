# Being RESTful with billions of dollars in transactions, thanks to C++, JSON and HTTP. 
Repository for abstract and code related to a talk.

## Submission Details
**Tags**: finance, JSON, HTTP, cpp17, cpp20  
**Session Type**: Case Study  
**Level**: Beginning to Intermediate  
**Audience**: application developers, library authors 
**Session Material**: slides, example code  

## Abstract
In this talk I provide my experience on the last three years of building restful interfaces in C++ using nlohmann::json and cpp-httplib. I will present a case study showing you the basics of how each of these header only libraries work, teasing out the pros and cons for our companies needs. Some insights into how Restful APIs like Stripe are created will be provided, and we’ll look at how using, and converting to strong types using proper design patterns can make your life easier if you go down the same route. Throughout the talk, I’ll specifically share in the experience gained from implementing RESTful APIs in credit card transaction systems with Mastercard/Visa. Join this session to learn how you can use JSON and/or HTTP to interface with almost anything. 

## Outline
Overview 
- REST APIs
    - What is a basic REST API
    - Example of a Simple REST API call.
- JSON 
- HTTP 

Background: 
- Replacing existing XML and legacy systems with modern JSON/REST API 
- nlohmann::json – Pros and Cons 
- cpp-httplib – Pros and Cons 
- Previous use of ZeroMQ, did we change? 
- Why do our company/team like header only? 
- Performance considerations vs others. 

Approach: 
- Overview of the design and patterns we used. 
- Start with the basic HTTP Server 
    - Add in endpoints 
        - GETS and POSTS most know, but what about PUT, DELETE… 
        - Is SSL easy? 
- Toss in some security 
    - IP blocking 
    - Rate limiting 
    - Consistency with error reporting. 
- Create the data structures and public facing API 
    - Considering high level interface vs lower-level data handling. 
- Converting types 
    - Some gotcha’s to consider when converting to/from json. 
- Creating a client 
    - Connecting with other RESTFUL services. 
    - Testing our own service. 
- Bridging between the client and the server. 
    - Consuming one service, transform, provide for our own services. 
- Best practices with REST and lessons learned overall. 

Findings: 
- Ease of integration. 
- Speed and Performance. 

Conclusion: What does it mean? 
- Serving or Consuming JSON and REST interfaces in C++ is simple. 
- Cross platform and efficient. 
- JSON allows easy sharing with other applications. 
- Useful design does not paint yourself into a corner. 

Time permitting… 
- JWT’s as they are used more often to help with securing transactions. 
- cpp-jwt 

## Previous Versions
A variation of this was given in 2018 when we focused on using ZeroMQ for our internal systems. It was about using ZeroMQ for socket communications in our framework with JSON. Here I will focus more on JSON/HTTP integrating with any RESTFUL service, with our experience on the customer application side. 

[Scaling Financial Transactions using 0MQ and JSON](https://www.youtube.com/watch?v=XLSckGMyzbs&t=2s) - CppCon 2018