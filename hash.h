#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
      int stringLength = k.size();
      int base36Form[stringLength];
      for(int i = 0; i < stringLength; i++){
        base36Form[i] = letterDigitToNumber(k[i]);
      }



      unsigned long long int decimalForm[5] = {0};
      unsigned long long int accumulator = 0;
      unsigned long long int multiplier = 1;
      int groupCnts = 0;
      int valueCnt = 0;

      if(stringLength < 6){
        //for strings less than length of 6
        for(int j = stringLength - 1; j >= 0; j--){
          accumulator = accumulator + (multiplier * base36Form[j]);
          multiplier *= 36;
        }
        return rValues[4] * accumulator;
      }
      else{
          for(int j = stringLength - 1; j >= 0; j--){
            //traverse in reverse thru index, compute every 6 values into a single value
            //after every 6 vaues, store the computed value in the corresponding group
            //repeat
            accumulator = accumulator + (multiplier * base36Form[j]);
            multiplier *= 36;
            if(valueCnt%6 == 5){
              decimalForm[4-groupCnts] = accumulator;
              multiplier = 1;
              accumulator = 0;
              groupCnts++;
            }
            valueCnt++;
          }
          if(accumulator != 0){
            //if there are values left over in final grouping, store in last group
            decimalForm[4-  groupCnts] = accumulator;
          } 
      }


      unsigned long long int returnVal = 0; 
      for(int k = 0; k < 5; k++){
        returnVal += (rValues[k] * decimalForm[k]); 
      }

      return returnVal;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        //check if integer val first
        if(letter >= 48 && letter <= 57){
          return static_cast<int>(letter) - 22;
        }
        else{
          //else, check if capital letter, conv if true, then return corresponding val
          if(letter  >=  65 && letter <= 90 ){
            letter = letter + 32;
          }
          return static_cast<int>(letter) - 97;
        }
        

    }
    

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
