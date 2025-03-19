#!/bin/bash

send_commands() {
  echo "USER user_name * * :real_name"
  echo "NICK nick__name"
}

(
  send_commands
  cat  
) | nc irc.libera.chat 6667
