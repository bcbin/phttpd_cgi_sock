# Part 2 - simple http server

A simple http server implementation wich support CGI

### Requirements

* This must work with TA's CGI programs (samples will be posted) and the previous
  CGI you wrote.

* Only parameters in GET method should be processed.

* All files name of which ends with ".cgi" should be interpreted as a executable file and should be invoked.

* It is not cared if requested file does not exist.

* You have to setup all the environmant variables (totally 9) in page 8 of http.pdf(Professor's course slides) when you execute CGI program, and the content of these environmant variables except "QUERY_STRING" can be anything you want.(Content of QUERY_STRING MUST be correct.)
  Ex: REMOTE_ADDR="140.113.185.117" or "remote address" or "anything I want"

* Do NOT use the http server provided by CS workstation, use the NP workstation.
