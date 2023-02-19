#!/bin/bash

assert(){
  expected="$1"
  input="$2"
  ./main "$input" > tmp.s
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

assert 5 "5;"
assert 1 "+1;"
assert 4 "2*2;"
assert 2 "4/2;"
assert 42 "12 + 34- 4;"
assert 21 " 5+20- 4;"
assert 45 " (1+2)* 3 * 5;"
assert 4 " (3+5) / 2;"
assert 1 "-5+6;"
assert 24 "(22-10)*2;" 
assert 0 "4==5;"
assert 1 "5 == 2+3;"
assert 1 "3< 4*4;"
assert 1 "5>=5;"
assert 1 "a = b = 1;"
assert 1 "1;"
assert 11 "a = 11;"

assert 1 "abc = 1;"
assert 2 "a = bc =2;"
assert 3 "a=1; b=a+2;"
assert 3 "a=1; b= 2;return a+b;"

echo OK

