#!/bin/bash

send_commands() {
  echo "PASS 42"
  echo "USER user_name * * :real_name"
  echo "NICK nick__name"
}

(
  send_commands
  cat  
) | nc localhost 6667
