#!/bin/bash

send_commands() {
  echo "USER user * * :real"
  echo "NICK nock"
}

(
  send_commands
  cat  
) | nc irc.libera.chat 6667
