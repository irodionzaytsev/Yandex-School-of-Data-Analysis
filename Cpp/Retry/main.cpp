//
//  main.cpp
//  LAYER_RETRY
//
//  Created by Shabanov Lev on 16/11/2016.
//  Copyright © 2016 Shabanov Lev. All rights reserved.
//

#include <iostream>
#include "retry.h"
#include <string>

int main() {
    srand(time(nullptr));
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    NetworkLayer dick;
    LogicLayer cock(dick, 5);
    std::cerr << "wtf\n";
    std::cout<< cock.read();
    return 0;
}
