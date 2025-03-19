#!/bin/bash

send_commands() {
  echo "USER uuuu * * :rrrr"
  echo "NICK nnnn"
}

(
  send_commands
  cat  
) | nc irc.libera.chat 6667
