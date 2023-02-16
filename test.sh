#!/bin/bash

assert(){
  expected="$1"
  input="$2"
  ./minicc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1;
  fi
}

assert 42 "12 + 34- 4 "
assert 21 " 5+20- 4"
assert 45 " (1+2)* 3 * 5"
assert 4 " (3+5) / 2"

echo OK

