#!/bin/bash

send_commands() {
  echo "PASS 42"
	echo "NICK nock"
	echo "USER user * * :real"
}

(
  send_commands
  cat  
) | nc localhost 6667
