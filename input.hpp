#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include "trapezoidalMap.hpp"
#include "input.hpp"
#include <random>

void makeInputRandom(std::vector<Line>& l);
void makeInputAdversarial_sorting(std::vector<Line>& l);
void genInput(int size, std::vector<Line>& l);


//stdin/stdout
void scanInput(std::vector<Line>& l);
void printInput(std::vector<Line>& l);
#endif