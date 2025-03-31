#!/bin/bash

send_commands() {
  echo "PASS 42"
  echo "NICK nick__name"
 echo "USER user_name * * :real_name"
}

(
  send_commands
  cat  
) | nc localhost 6667
