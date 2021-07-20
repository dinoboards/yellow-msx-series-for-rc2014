#!/usr/bin/env bash

case "${1}" in
  aust)
    echo 11
    ;;

  japan)
    echo 0
    ;;

  usa)
    echo 1
    ;;

  international)
    echo 2
    ;;

  uk)
    echo 3
    ;;

  france)
    echo 4
    ;;

  germany)
    echo 5
    ;;

  italy)
    echo 6
    ;;

  spain)
    echo 7
    ;;

  arabic)
    echo 8
    ;;

  korea)
    echo 9
    ;;

  russia)
    echo 10
    ;;

  *)
    echo 99
    ;;
esac
