// A cute recursive print function for positive decimal integers.
// Uses a routine printDigit that prints a single digit.

#include<iostream>

void printDigit(int);

void printOut(int num) {
  if (num>=10)
    printOut(num/10);
  //printDigit(num%10);
  printDigit(num-(num/10)*10); // faster than mod
}

void printDigit(int digit) {
    std::cout << digit;
}
